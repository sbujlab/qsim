#include "qsimDetectorConstruction.hh"

#include "qsimGenericDetector.hh"


#include "qsimRun.hh"
#include "qsimRunData.hh"
#include "qsimIO.hh"
#include "qsimMessenger.hh"

#include "TGeoManager.h"

#include "G4GenericMessenger.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4NistManager.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "globals.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "G4ios.hh"

#include "G4UnitsTable.hh"

// GDML export
#include "G4GDMLParser.hh"

//visual
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#define __DET_STRLEN 200
#define __MAX_DETS 100000

#include "G4Threading.hh"
#include "G4AutoLock.hh"
namespace { G4Mutex qsimDetectorConstructionMutex = G4MUTEX_INITIALIZER; }

//G4ThreadLocal qsimGlobalField* qsimDetectorConstruction::fGlobalField = 0;

qsimDetectorConstruction::qsimDetectorConstruction()
: fGDMLParser(0),
  fMessenger(0),fGeometryMessenger(0),
  fVerboseLevel(0),
  fGDMLValidate(true),fGDMLOverlapCheck(true),
  fWorldVolume(0)
{
  // Create GDML parser
  fGDMLParser = new G4GDMLParser();

  // Default geometry file
  fDetFileName = "geometry_perl/qsimMother.gdml";

  // Create generic messenger
  fMessenger = new G4GenericMessenger(this,"/qsim/","qsim properties");
  fMessenger->DeclareProperty(
      "setgeofile",
      fDetFileName,
      "Set geometry GDML files")
      .SetStates(G4State_PreInit);
  fMessenger->DeclareMethod(
      "dumpgeometry",
      &qsimDetectorConstruction::DumpGeometry,
      "Dump the geometry tree")
      .SetStates(G4State_Idle)
      .SetDefaultValue("false");
  fMessenger->DeclareMethod(
      "dumpelements",
      &qsimDetectorConstruction::DumpElements,
      "Dump the elements")
      .SetStates(G4State_Idle);
  fMessenger->DeclareMethod(
      "dumpmaterials",
      &qsimDetectorConstruction::DumpMaterials,
      "Dump the materials")
      .SetStates(G4State_Idle);

  // Create geometry messenger
  fGeometryMessenger = new G4GenericMessenger(this,
      "/qsim/geometry/",
      "qsim geometry properties");
  fGeometryMessenger->DeclareProperty(
      "setfile",
      fDetFileName,
      "Set geometry GDML file")
      .SetStates(G4State_PreInit);
  fGeometryMessenger->DeclareProperty(
      "verbose",
      fVerboseLevel,
      "Set geometry verbose level")
          .SetStates(G4State_PreInit);
  fGeometryMessenger->DeclareProperty(
      "validate",
      fGDMLValidate,
      "Set GMDL validate flag")
          .SetStates(G4State_PreInit)
          .SetDefaultValue("true");
  fGeometryMessenger->DeclareProperty(
      "overlapcheck",
      fGDMLOverlapCheck,
      "Set GMDL overlap check flag")
          .SetStates(G4State_PreInit)
          .SetDefaultValue("true");
  fGeometryMessenger->DeclareMethod(
      "dumpelements",
      &qsimDetectorConstruction::DumpElements,
      "Dump the elements")
      .SetStates(G4State_Idle);
  fGeometryMessenger->DeclareMethod(
      "dumpmaterials",
      &qsimDetectorConstruction::DumpMaterials,
      "Dump the materials")
      .SetStates(G4State_Idle);
  fGeometryMessenger->DeclareMethod(
      "dumpgeometry",
      &qsimDetectorConstruction::DumpGeometry,
      "Dump the geometry tree")
      .SetStates(G4State_Idle)
      .SetDefaultValue("false");
}

qsimDetectorConstruction::~qsimDetectorConstruction() {
    delete fGDMLParser;
    delete fMessenger;
    delete fGeometryMessenger;
}

G4VPhysicalVolume* qsimDetectorConstruction::Construct()
{
    qsimIO* io = qsimIO::GetInstance();
    io->GrabGDMLFiles(fDetFileName);

    fGDMLParser->Clear();
    fGDMLParser->SetOverlapCheck(fGDMLOverlapCheck);

    G4cout << "Reading " << fDetFileName << G4endl;
    fGDMLParser->Read(fDetFileName,fGDMLValidate);

    fWorldVolume = fGDMLParser->GetWorldVolume();
    
    //====================================================
    // Associate target volumes with beam/target class
    // This has to match what is declared in the GDML volumes
    // We absolutely need some connection between the geometry
    // structure and having access to the physical volumes.
    // This could be made more general with a full treesearch
    //====================================================

    G4LogicalVolume *thislog = fWorldVolume->GetLogicalVolume();
    G4int vidx = 0;

    G4String targetmothername = "logicTarget";
    while( vidx < thislog->GetNoDaughters() ){
	if( thislog->GetDaughter(vidx)->GetName() == targetmothername.append("_PV")) break;
	vidx++; 
    }
    if( vidx == thislog->GetNoDaughters() ){
	G4cerr << "WARNING " << __PRETTY_FUNCTION__ << " line " << __LINE__ <<
	    ":  target definition structure in GDML not valid" << G4endl;
    } else {
        // Mutex lock before writing static structures in qsimBeamTarget
//        G4AutoLock lock(&qsimDetectorConstructionMutex);
//        qsimBeamTarget::ResetTargetVolumes();
//	      qsimBeamTarget::SetMotherVolume(thislog->GetDaughter(vidx));

	thislog = thislog->GetDaughter(vidx)->GetLogicalVolume();

	////////////////////////////////////////////////////////////////////////////////
	// List relevant target volumes here terminated by "" //////////////////////////
	// FIXME:  This could probably be done better with auxiliary information
	//         though that only gives us *logical* volumes and we need the physical
	//         volumes for placement information
	//
	//         *ORDERING IS IMPORTANT - MUST GO UPSTREAM TO DOWNSTREAM*
	//         FIXME:  can sort that on our own
	G4String targvolnames[] = {
	    "USAlTarg", "h2Targ", "DSAlTarg", ""
	};
	////////////////////////////////////////////////////////////////////////////////

	int nidx = 0;
/*	while( targvolnames[nidx] != "" ){
	    targvolnames[nidx].append("_PV");
	    vidx = 0;
	    while( vidx < thislog->GetNoDaughters() ){
	        if( thislog->GetDaughter(vidx)->GetName() == targvolnames[nidx]) break;
		vidx++;
	    }
	    if( vidx == thislog->GetNoDaughters() ){
		G4cerr << "Error " << __PRETTY_FUNCTION__ << " line " << __LINE__ <<
		    ":  target definition structure in GDML not valid.  Could not find volume " << targvolnames[nidx] << G4endl;
	    } else {
		qsimBeamTarget::AddTargetVolume(thislog->GetDaughter(vidx));
	    }

	    nidx++;
	}*/
    }

  //==========================
  // List auxiliary info
  //==========================

  const G4GDMLAuxMapType* auxmap = fGDMLParser->GetAuxMap();

  if (fVerboseLevel > 0)
      G4cout << "Found " << auxmap->size()
         << " volume(s) with auxiliary information."
	 << G4endl << G4endl;
  for (G4GDMLAuxMapType::const_iterator
	  iter  = auxmap->begin();
	  iter != auxmap->end(); iter++) {
      if (fVerboseLevel > 0)
          G4cout << "Volume " << ((*iter).first)->GetName()
	     << " has the following list of auxiliary information: "<< G4endl;
      for (G4GDMLAuxListType::const_iterator
	      vit  = (*iter).second.begin();
	      vit != (*iter).second.end(); vit++) {
        if (fVerboseLevel > 0)
            G4cout << "--> Type: " << (*vit).type
	       << " Value: "   << (*vit).value << std::endl;

        if ((*vit).type == "Visibility") {
          G4Colour colour(1.0,1.0,1.0);
          const G4VisAttributes* visAttribute_old = ((*iter).first)->GetVisAttributes();
          if (visAttribute_old)
            colour = visAttribute_old->GetColour();
          G4VisAttributes visAttribute_new(colour);
          if ((*vit).value == "true")
            visAttribute_new.SetVisibility(true);
          if ((*vit).value == "false")
            visAttribute_new.SetVisibility(false);
          if ((*vit).value == "wireframe")
            visAttribute_new.SetForceWireframe(false);
          ((*iter).first)->SetVisAttributes(visAttribute_new);
        }

        if ((*vit).type == "Color") {
          G4Colour colour(1.0,1.0,1.0);
          if (G4Colour::GetColour((*vit).value, colour)) {
            if (fVerboseLevel > 0)
              G4cout << "Setting color to " << (*vit).value << "." << G4endl;
            G4VisAttributes visAttribute(colour);
            ((*iter).first)->SetVisAttributes(visAttribute);
          } else {
            if (fVerboseLevel > 0)
              G4cout << "Colour " << (*vit).value << " is not known." << G4endl;
          }
        }

        if ((*vit).type == "Alpha") {
          G4Colour colour(1.0,1.0,1.0);
          const G4VisAttributes* visAttribute_old = ((*iter).first)->GetVisAttributes();
          if (visAttribute_old)
            colour = visAttribute_old->GetColour();
          G4Colour colour_new(
              colour.GetRed(), 
              colour.GetGreen(),
              colour.GetBlue(),
              std::atof((*vit).value.c_str()));
          G4VisAttributes visAttribute_new(colour_new);
          ((*iter).first)->SetVisAttributes(visAttribute_new);
        }
      }
  }
  if (fVerboseLevel > 0)
      G4cout << G4endl<< G4endl;

  //==========================
  // Visualization attributes
  //==========================

  G4VisAttributes* motherVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  motherVisAtt->SetVisibility(false);
  fWorldVolume->GetLogicalVolume()->SetVisAttributes(motherVisAtt);

  G4VisAttributes* daughterVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  daughterVisAtt->SetForceWireframe (true);
  for(int i=0;i<fWorldVolume->GetLogicalVolume()->GetNoDaughters();i++){
      fWorldVolume->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->SetVisAttributes(daughterVisAtt);
  }

  //==========================
  // Output geometry tree
  //==========================

  UpdateCopyNo(fWorldVolume,1);

  if (fVerboseLevel > 0)
    G4cout << G4endl << "###### Leaving qsimDetectorConstruction::Read() " << G4endl << G4endl;

  return fWorldVolume;
}

void qsimDetectorConstruction::ConstructSDandField()
{
  //==========================
  // Sensitive detectors
  //==========================
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  char detectorname[__DET_STRLEN];
  int retval;

  G4VSensitiveDetector* thisdet;

  G4int k=0;

  G4GDMLAuxListType::const_iterator vit, nit;

  if (fVerboseLevel > 0)
      G4cout << "Beginning sensitive detector assignment" << G4endl;

  G4bool useddetnums[__MAX_DETS];
  for (k = 0; k < __MAX_DETS; k++ ){useddetnums[k] = false;}
  k = 0;

  const G4GDMLAuxMapType* auxmap = fGDMLParser->GetAuxMap();
  for (G4GDMLAuxMapType::const_iterator iter  = auxmap->begin(); iter != auxmap->end(); iter++) {
      G4LogicalVolume* myvol = (*iter).first;
      if (fVerboseLevel > 0)
          G4cout << "Volume " << myvol->GetName() << G4endl;

      for (G4GDMLAuxListType::const_iterator
          vit  = (*iter).second.begin();
          vit != (*iter).second.end(); vit++) {

          if ((*vit).type == "SensDet") {
              G4String det_type = (*vit).value;

              // Also allow specification of det number ///////////////////
              int det_no = -1;
              for (G4GDMLAuxListType::const_iterator
                  nit  = (*iter).second.begin();
                  nit != (*iter).second.end(); nit++) {

                  if ((*nit).type == "DetNo") {
                      det_no= atoi((*nit).value.data());
                      if( det_no >= __MAX_DETS ){
                          G4cerr << __FILE__ << " line " << __LINE__ << ": ERROR detector number too high" << G4endl;
                          exit(1);
                      }
                      useddetnums[det_no] = true;
                  }
              }
              if( det_no <= 0 ){
                  k = 1;
                  while( useddetnums[k] == true && k < __MAX_DETS ){ k++; }
                  if( k >= __MAX_DETS ){
                      G4cerr << __FILE__ << " line " << __LINE__ << ": ERROR too many detectors" << G4endl;
                      exit(1);
                  }
                  det_no = k;
                  useddetnums[k] = true;
              }
              /////////////////////////////////////////////////////////////

              retval = snprintf(detectorname, __DET_STRLEN,"qsim/det_%d", det_no);

              assert( 0 < retval && retval < __DET_STRLEN ); // Ensure we're writing reasonable strings

              thisdet = SDman->FindSensitiveDetector(detectorname,(fVerboseLevel > 0));

              if( thisdet == 0 ) {
                  thisdet = new qsimGenericDetector(detectorname, det_no);
                  if (fVerboseLevel > 0)
                      G4cout << "  Creating sensitive detector " << det_type
                          << " for volume " << myvol->GetName()
                          <<  G4endl << G4endl;
                  SDman->AddNewDetector(thisdet);
              }

              myvol->SetSensitiveDetector(thisdet);
          }
      }
  }
  if (fVerboseLevel > 0)
      G4cout << "Completed sensitive detector assignment" << G4endl;

  //==========================
  // Magnetic fields
  //==========================

 // if (fGlobalField) delete fGlobalField;
 // fGlobalField = new qsimGlobalField();
}

G4int qsimDetectorConstruction::UpdateCopyNo(G4VPhysicalVolume* aVolume,G4int index){  

  //if (aVolume->GetLogicalVolume()->GetNoDaughters()==0 ){
      aVolume->SetCopyNo(index);
      index++;
      //}else {
    for(int i=0;i<aVolume->GetLogicalVolume()->GetNoDaughters();i++){
      index = UpdateCopyNo(aVolume->GetLogicalVolume()->GetDaughter(i),index);
    }
    //}

  return index;
}

void qsimDetectorConstruction::DumpElements() {
  G4cout << G4endl << "Element table: " << G4endl << G4endl;
  G4cout << *(G4Element::GetElementTable()) << G4endl;
}

void qsimDetectorConstruction::DumpMaterials() {
  G4cout << G4endl << "Material table: " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

void qsimDetectorConstruction::DumpGeometricalTree(
    G4VPhysicalVolume* aVolume,
    G4int depth,
    G4bool surfchk)
{
  // Null volume
  if (aVolume == 0) aVolume = fWorldVolume;

  // Print spaces
  for (int isp = 0; isp < depth; isp++) { G4cout << "  "; }
  // Print name
  G4cout << aVolume->GetName() << "[" << aVolume->GetCopyNo() << "] "
         << aVolume->GetLogicalVolume()->GetName() << " "
         << aVolume->GetLogicalVolume()->GetNoDaughters() << " "
         << aVolume->GetLogicalVolume()->GetMaterial()->GetName() << " "
	 << G4BestUnit(aVolume->GetLogicalVolume()->GetMass(true),"Mass");
  // Print sensitive detector
  if (aVolume->GetLogicalVolume()->GetSensitiveDetector())
  {
    G4cout << " " << aVolume->GetLogicalVolume()->GetSensitiveDetector()
                            ->GetFullPathName();
  }
  G4cout << G4endl;

  // Check overlapping volumes
  if (surfchk) aVolume->CheckOverlaps();

  // Descend down the tree
  for (int i = 0; i < aVolume->GetLogicalVolume()->GetNoDaughters(); i++) {
    DumpGeometricalTree(aVolume->GetLogicalVolume()->GetDaughter(i),depth+1,surfchk);
  }
}
