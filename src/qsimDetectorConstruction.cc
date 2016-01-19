#include "qsimDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "qsimDetector.hh"
#include "qsimScintDetector.hh"
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4OpticalSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void qsimDetectorConstruction::DetModeSet(G4int detMode = 1) {
	
	fDetMode = detMode;
	// 0 is PREX-I design
	// 1 is PREX-II prototype (so-called "design 3")
	
}

void qsimDetectorConstruction::StandModeSet(G4int standMode = 0) {
    
    fStandMode = standMode;
    // 1 cosmic setup (detector, lead, scintillators)
    // 0 beam setup (detector only)
    
}


qsimDetectorConstruction::qsimDetectorConstruction() {
    
    DetModeSet();
    StandModeSet();

	fQuartzPolish = 0.97;	    
	fDetAngle = 0.*deg;
	fDetPosX = 0.*cm;
	fDetPosY = 0.*cm;
	  
    // fNewStand = false; // Default setting is for the setup to reflect to old cosmic stand. True will go to the new design. Messenger has commands to switch between these at command line or at batch mode run time as well.
    // fAccBeamStand = false; // Only affects stand components: true deletes the lead block.
    
    quartz_x = 1.75*cm; // CSC measures in SolidWorks 0.689 x 2.952 x 0.197 cm
    quartz_y = 7.5*cm;  //2.5
    //Half cm
    quartz_z = 0.5*cm;
    //One cm
    //  quartz_z = 0.5*cm;
    
    quartz_zPos = 0.0*cm; //-.9*cm;//-1.1*cm; //-.9*cm; //-.6*cm;
    
    cone_rmin1 = 2.1*cm;
    cone_rmax1 = cone_rmin1+.05*cm;
    cone_rmin2 = 2.5*cm;  // normally 2.5*cm;
    cone_rmax2 = cone_rmin2+.05*cm;
    cone_z = quartz_y+.5*cm;    //3
    cone_sphi = 0.;
    cone_fphi = 2*3.1415;
    
    rin = cone_rmin2;  // normally 2.5*cm;
    rout = rin+.05*cm;
    lngth = 1.6*cm;  // PMT dist. = 2*lngth +1cm  (10.4 == 4.5, 6.8 == 2.9)
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

qsimDetectorConstruction::~qsimDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* qsimDetectorConstruction::Construct() {
    
    // Define materials
    
    G4double a, z, density;
    G4int nelements;
    
    // Air
    G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
    
    G4Material* Air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
    Air->AddElement(N, 70.*perCent);
    Air->AddElement(O, 30.*perCent);
    
    // Quartz
    G4Element* Si = new G4Element("Silicon", "Si", z=14 , a=28*g/mole);
    
    G4Material* Quartz = new G4Material("Quartz", density= 2.203*g/cm3, nelements=2);
    Quartz->AddElement(Si, 1);
    Quartz->AddElement(O, 2);
    
    // Aluminum for mirror and stand (need separate materials so that mirror can be reflective)
    G4Element* Al = new G4Element("Aluminum", "Al", z=13 , a=27*g/mole);
    G4Material* Alu_Mat = new G4Material("Alu_Mat", 2.7*g/cm3, nelements=1);
    Alu_Mat->AddElement(Al, 1);
    G4Material* Mirror = new G4Material("Mirror", density= 2.7*g/cm3, nelements=1);
    Mirror->AddElement(Al, 1);
    
    // Lead
    G4Element* Pb = new G4Element("Lead", "Pb", z=82 , a=207.2*g/mole);
    G4Material* Pb_Mat = new G4Material("Pb_Mat", 11.34*g/cm3, nelements=1);
    Pb_Mat->AddElement(Pb, 1);
    
    // Let us make cathode from a special metal (reflectivity 0, efficiency of photoelectrons 25%)
    G4Material* CATH = new G4Material("CATH", density= 2.7*g/cm3, nelements=1);
    CATH->AddElement(Al, 1);
    
    
    // Define optical property tables
    
    const G4int nEntries = 205;
    
    // Array of photon energies
    G4double PhotonEnergy[nEntries] =
    {  2.4,2.42,2.44,2.46,2.48,2.5,2.52,2.54,2.56,2.58,
        2.6,2.62,2.64,2.66,2.68,2.7,2.72,2.74,2.76,2.78,
        2.8,2.82,2.84,2.86,2.88,2.9,2.92,2.94,2.96,2.98,
        3,3.02,3.04,3.06,3.08,3.1,3.12,3.14,3.16,3.18,
        3.2,3.22,3.24,3.26,3.28,3.3,3.32,3.34,3.36,3.38,
        3.4,3.42,3.44,3.46,3.48,3.5,3.52,3.54,3.56,3.58,
        3.6,3.62,3.64,3.66,3.68,3.7,3.72,3.74,3.76,3.78,
        3.8,3.82,3.84,3.86,3.88,3.9,3.92,3.94,3.96,3.98,
        4,4.02,4.04,4.06,4.08,4.1,4.12 ,4.14,4.16,4.18,         //Glass cuts off above 4.135eV, 87 entries
        4.2,4.22,4.24,4.26,4.28,4.3,4.32,4.34,4.36,4.38,
        4.4,4.42,4.44,4.46,4.48,4.5,4.52,4.54,4.56,4.58,
        4.6,4.62,4.64,4.66,4.68,4.7,4.72,4.74,4.76,4.78,
        4.8,4.82,4.84,4.86,4.88,4.9,4.92,4.94,4.96,4.98,        // Cut off -> 4.96eV ~ 250nm
        5,5.02,5.04   ,   5.06,5.08,5.1,5.12,5.14,5.16,5.18,    // 5.04eV = 246 nm is the 30% cutoff, 133 entries
        5.2,5.22,5.24,5.26,5.28,5.3,5.32,5.34,5.36,5.38,
        5.4,5.42,5.44,5.46,5.48,5.5,5.52,5.54,5.56,5.58,
        5.6,5.62,5.64,5.66,5.68,5.7,5.72,5.74,5.76,5.78,
        5.8,5.82,5.84,5.86,5.88,5.9,5.92,5.94,5.96,5.98,
        6,6.02,6.04,6.06,6.08,6.1,6.12,6.14,6.16,6.18,
        6.21,6.29,6.38,6.48,6.57,6.67,6.78,6.87,6.98,
        7.08,7.20,7.32,7.44,7.56,7.69
    };
    
    // Cathode quantum efficiency
    // Response obtained from the plot of the quantum efficiency as a function of wavelength and then changed to eV for the Bialkali photocathode (synthetic silica)
    
    G4double EfficiencyArrayPercent[nEntries] =
    {  11.0,12.0,12.5,13.1,13.5,14.5,15.2,16.0,16.5,17.0, // percentages here
        17.5,18.0,18.5,19.0,19.2,19.7,20.1,20.7,20.9,21.1,
        21.6,22.0,22.5,22.7,23.0,23.5,23.7,24.0,24.0,24.2,
        24.2,24.5,25.0,25.0,25.3,25.5,25.5,25.5,25.5,25.5,
        25.5,25.5,25.5,25.7,26.1,26.1,26.1,26.1,25.6,25.6,
        25.6,25.6,25.6,25.6,25.6,25.6,25.6,25.6,25.6,25.6,
        25.6,25.6,25.6,26.1,26.1,26.1,26.1,26.1,26.1,26.1,
        25.6,25.0,25.0,25.0,25.0,24.5,24.5,24.5,24.5,24.3,
        24.0,24.0,24.0,24.0,24.0,24.0,23.5,23.5,23.5,23.5,
        23.5,23.5,23.5,23.3,23.1,22.8,22.6,22.6,22.6,22.6, // 4.38 eV
        22.6,22.6,22.3,22.1,22.1,22.1,22.0,21.8,21.7,21.3, //100 entries 4.58 eV
        21.2,21.0,20.8,20.8,20.8,20.8,20.8,20.8,20.8,20.8, // 4.78 eV
        20.4,20.4,20.4,20.4,20.4,20.4,20.4,20.4,20.2,20.0, // 4.98 eV
        20.0,20.0,20.0,20.0,20.0,20.0,20.0,19.5,19.5,19.5, // 5.18 eV
        19.5,19.5,19.5,19.5,19.1,19.1,19.1,19.1,19.1,19.1, // 5.38 eV
        19.1,19.1,19.1,19.0,18.8,18.8,18.8,18.8,18.8,18.8, // 5.58 eV
        18.8,18.4,18.4,18.4,18.4,18.4,18.4,18.4,18.4,18.4, // 5.78 eV
        18.4,18.4,18.4,18.4,18.4,18.4,18.4,18.4,18.4,18.4, // 5.98eV
        18.4,18.2,18.0,18.0,18.0,18.0,18.0,18.0,18.0,18.0,
        18.0,17.6,17.6,17.6,17.6,17.2,16.5,16.2,15.9,
        15.2,14.9,14.3,12.1,10.2,9.6
    };                                                      // 6.18 eV
    
    G4double EfficiencyArray[nEntries];

    G4double RefractiveIndex1[nEntries];    // quartz refractive index
    G4double Absorption1[nEntries];         // quartz absorption length
    G4double RefractiveIndex2[nEntries];    // air refractive index
    G4double Reflectivity1[nEntries];       // mirror reflectivity
    G4double Reflectivity2[nEntries];       // cathode reflectivity

    
    for (int i = 0; i < nEntries; i++) {
        
        PhotonEnergy[i] = PhotonEnergy[i]*eV;
	EfficiencyArray[i] = 0.01*EfficiencyArrayPercent[i];
        RefractiveIndex1[i]= 1.438 + (.01197*PhotonEnergy[i]/eV) - (.001955*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV) + (.0004793*PhotonEnergy[i]*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV/eV);
        
        // *** need to update this
        Absorption1[i] = (exp(4.325)*exp(1.191*PhotonEnergy[i]/eV)*exp(-.213*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV)*exp(-.04086*PhotonEnergy[i]*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV/eV))*m;
        if (Absorption1[i] > 25*m) {
            Absorption1[i] = 25*m;
        }
        
        // *** need to update this
        if (PhotonEnergy[i] < 4.135*eV) {
        } else if (PhotonEnergy[i] >= 4.135*eV && PhotonEnergy[i] < 6.203*eV) {
            Reflectivity1[i] = .6;  // .7
        } else {
            Reflectivity1[i] = .5;  // .6
        }
        
        RefractiveIndex2[i] = 1.;
        Reflectivity2[i] = 0.;
    }
    
    
    // Quartz material property table
    
    G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();
    myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries);
    myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries);
    Quartz->SetMaterialPropertiesTable(myMPT1);
    
    // Air material properties table
    // *** need to add absorption length?
    G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
    myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);
    Air->SetMaterialPropertiesTable(myMPT2);
    
    
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    
    
    
    ////////////////////////////////////////////
    // Defining geometry
    ////////////////////////////////////////////
    
    // Volumes
    
    // World and detector box
    
    double world_x, world_y, world_z;
    double det_x, det_y, det_z;
    
    world_x = world_y = world_z = 275*cm;
    det_x = 15*cm;
    det_y = 6*cm;
    det_z = 6*cm;
    
    G4Box* world_box = new G4Box("World",world_x,world_y,world_z);
    
    G4LogicalVolume* world_log
    = new G4LogicalVolume(world_box,Air,"World",0,0,0);
    
    world_log->SetVisAttributes(G4VisAttributes::Invisible);
    
    G4VPhysicalVolume* world_phys
    = new G4PVPlacement(0,G4ThreeVector(),world_log,"World",0,false,0);
    
    G4Box* det_box = new G4Box("Detector",det_x,det_y,det_z);
    
    G4LogicalVolume* det_log
    = new G4LogicalVolume(det_box,Air,"Detector_log",0,0,0);
    
    det_log->SetVisAttributes(G4VisAttributes::Invisible);
    

    // First, create solids and logical volumes
    
    // Quartz (defined for all modes)
    
    G4double q_yLB = quartz_y - (quartz_z);
    
    G4Trap* quartz_box = new G4Trap("Quartz", 2*quartz_x, 2*quartz_z, 2*quartz_y, 2*q_yLB);

    G4LogicalVolume* quartz_log
    = new G4LogicalVolume(quartz_box,Quartz,"Quartz",0,0,0);
    
    qsimScintDetector* quartzSD = new qsimScintDetector("QuartzSD", 10);
    
    SDman->AddNewDetector(quartzSD);
    quartz_log->SetSensitiveDetector(quartzSD);
    
    G4RotationMatrix* rotQ = new G4RotationMatrix;
    
    rotQ->rotateX(M_PI/2.*rad);
    if(fDetMode == 0) {
        rotQ->rotateZ(M_PI*rad);
    }
    
    G4VPhysicalVolume* quartz_phys
    = new G4PVPlacement(rotQ,G4ThreeVector(0,0,quartz_zPos),quartz_log,"Quartz", det_log,false,0);

    // Light guide and tube mirror (only for PREX-I design)
  
    G4Trap *lightguide_big = new G4Trap("lightguide_big",11.21*cm,2.97456*deg,90.0*deg,3.15*cm+0.05*cm,3.25*cm+0.05*cm,3.25*cm+0.05*cm,0.0*deg,0.225*cm+0.05*cm,2.0*cm+0.05*cm,2.0*cm+0.05*cm,0.0*deg);
    G4Trap *lightguide_small = new G4Trap("lightguide_small",11.22*cm,2.97456*deg,90.0*deg,3.15*cm,3.25*cm,3.25*cm,0.0*deg,0.225*cm,2.0*cm,2.0*cm,0.0*deg);
    
    G4VSolid *lightguide_virt = new G4SubtractionSolid("lightguide_virt", lightguide_big, lightguide_small);
    
    G4LogicalVolume *lightguide_log = new G4LogicalVolume(lightguide_virt, Mirror, "LG_log",0,0,0);
    
    G4Cons* mirror_tube = new G4Cons("TMirror",cone_rmin2,cone_rmax2,2.5*cm,2.55*cm,lngth,cone_sphi,cone_fphi);
    
    G4LogicalVolume* tmirror_log = new G4LogicalVolume(mirror_tube,Mirror,"TMirror",0,0,0);
    

   
    // PMT and cathode
   
	G4double anini = 0.*deg;
	G4double anspan = 360.*deg;
 
    G4double prin = 0;
    G4double prout = 2.6*cm;
    G4double plngth = 1.5*mm;
    
    G4Tubs* pmt = new G4Tubs("PMT",prin,prout,plngth,anini,anspan);
    
    G4LogicalVolume* pmt_log
    = new G4LogicalVolume(pmt,Air,"PMT",0,0,0);

    G4String DetSDname = "tracker1";
    
    G4double cin = 0;
    G4double cout = 2.6*cm;
    G4double clngth = 0.1*mm;
    
    G4Tubs* cath = new G4Tubs("CATH",cin,cout,clngth,anini,anspan);
    
    G4LogicalVolume* cath_log
    = new G4LogicalVolume(cath,CATH,"CATH",0,0,0);
    
    qsimDetector* cathSD = new qsimDetector("cath", 2);
    
    SDman->AddNewDetector(cathSD);
    cath_log->SetSensitiveDetector(cathSD);
    
    G4VisAttributes *cathatt = new G4VisAttributes();
    cathatt->SetColour(1.0, 1.0, 0.2);
    cath_log->SetVisAttributes(cathatt);
    
    
    // Scintillators
    // Coincidence volumes **** NOTE: Upper scint is below the quartz (First coincidence w/ e-)
    
    
    G4Box* upperScint = new G4Box("upperScint",4.5*cm,4.5*cm,0.75*cm);
    G4LogicalVolume* uScint_log = new G4LogicalVolume(upperScint,Air,"upperScint",0,0,0);
    
    // Make sensitive
    DetSDname = "tracker2";
    
    qsimScintDetector* upScintSD = new qsimScintDetector(DetSDname, 1);
    
    SDman->AddNewDetector(upScintSD);
    uScint_log->SetSensitiveDetector(upScintSD);

    G4double upScint_pos;
    
    upScint_pos = quartz_z-50*cm; //45*cm; // changed to 45 cm from 50 cm as a rough estimate based on CAD measurements of the PMT model 1 + quartz design on the new stand design.
    
    
    
    
    G4Box* lowScint = new G4Box("lowScint",4.5*cm,4.5*cm,0.75*cm);
    G4LogicalVolume* lScint_log = new G4LogicalVolume(lowScint,Air,"lowScint",0,0,0);
    
    // Make sensitive
    DetSDname = "tracker3";
    
    qsimScintDetector* loScintSD = new qsimScintDetector(DetSDname, 2);
    
    SDman->AddNewDetector(loScintSD);
    lScint_log->SetSensitiveDetector(loScintSD);

    G4double loScint_pos;
    loScint_pos = upScint_pos+1.006*m; //new setup is 1.02874*m; // measured to be 1.02874 m between the two scintillators in the CAD drawings. Previously was just 1.0 m

    
    
    // LEAD BLOCK
    ///////////////////////////////////////////////////////////////////////////////////////
    
    G4Box* Pb_blox = new G4Box("Pb_blox", 10.16*cm,7.62*cm, 10.16*cm);
    //   expanded to ensure nothing
    //   can hit the scint. w/o the lead.
    G4LogicalVolume* Pb_log = new G4LogicalVolume(Pb_blox,Pb_Mat,"Lead",0,0,0);
    
    G4double Pb_pos;
    Pb_pos = loScint_pos-15.35*cm; // new setup is = loScint_pos-18.554*cm; //(-1*quartz_z)+(30.0*cm-(quartz_y*sin(scintAngle)))*sin(scintAngle);
    // If fAccBeamStand == true then remove the lead bricks, else leave them

    // Detector

    
	
    
    // Place physical volumes
    
    	G4RotationMatrix* detrot = new G4RotationMatrix;
	G4RotationMatrix* rot_pmt = new G4RotationMatrix;
	if (fDetMode == 0) {
		
    		detrot->rotateY(45.*deg);
		G4RotationMatrix* rotlg = new G4RotationMatrix;
		rotlg->rotateY(M_PI/2.*rad);
		rotlg->rotateZ(-M_PI/2.*rad);
		    
		rot_pmt->rotateY(M_PI/2.*rad);
		G4VPhysicalVolume* tmirror_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(7.25*cm+lngth+2.*cm,0.,.9*cm),tmirror_log,"TMirror",det_log,false,0);
		G4VPhysicalVolume* lightguide_phys = new G4PVPlacement(rotlg,G4ThreeVector(0.*cm,0,-0.375*cm+.9*cm),lightguide_log,"lightguide_phys", det_log,false,0);
		G4VPhysicalVolume* pmt_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(7.25*cm+plngth+7.*cm,0.,.9*cm),pmt_log,"PMT",det_log,false,0);
		G4VPhysicalVolume* cath_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(7.25*cm+2.*plngth+7.*cm,0.,.9*cm),cath_log,"CATH",det_log,false,0);
	}

	if (fDetMode == 1) {
		    
    		detrot->rotateY(fDetAngle);
		rot_pmt -> rotateY(M_PI/4.*rad);

		G4VPhysicalVolume* pmt_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(7.5*cm,0.*cm,0.*mm),pmt_log,"PMT",det_log,false,0);
		G4VPhysicalVolume* cath_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(7.5*cm+plngth*cos(M_PI/4*rad)*mm,0.*cm,-plngth*cos(M_PI/4.*rad)*mm),cath_log,"CATH",det_log,false,0);
	}


    G4VPhysicalVolume* det_phys
    = new G4PVPlacement(detrot,G4ThreeVector(fDetPosX,fDetPosY,0.*cm),det_log,"detector_phys",world_log,false,0);
	
    
    if (fStandMode == 1) {
        G4PVPlacement* uScint_phys = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,upScint_pos-1.*cm),uScint_log,"upperScint",world_log,false,0);
        G4PVPlacement* lScint_phys = new G4PVPlacement(0,G4ThreeVector(0.0,0.0,loScint_pos),lScint_log,"lowerScint",world_log,false,0);
        G4PVPlacement* Pb_phys = new G4PVPlacement(0,G4ThreeVector(0.0,0.0*cm,Pb_pos),Pb_log,"Pb",world_log,false,0);
    }

    
    
    
    
    // Surfaces
    
    
    // quartz
    G4OpticalSurface* OpQuartzSurface = new G4OpticalSurface("QuartzSurface");
    OpQuartzSurface->SetType(dielectric_dielectric);
    OpQuartzSurface->SetFinish(ground);
    OpQuartzSurface->SetModel(glisur);
    OpQuartzSurface->SetPolish(fQuartzPolish);
    
    G4LogicalBorderSurface* QuartzSurface =
    new G4LogicalBorderSurface("QuartzSurface",quartz_phys,det_phys,OpQuartzSurface);
    
    // mirrors and cathode
    G4OpticalSurface* MOpSurface = new G4OpticalSurface("MirrorOpSurface");
    G4OpticalSurface* CTHOpSurface = new G4OpticalSurface("CathodeOpSurface");
    
    MOpSurface -> SetType(dielectric_metal);
    MOpSurface -> SetFinish(polishedlumirrorair);
    MOpSurface -> SetModel(glisur);
    
    CTHOpSurface -> SetType(dielectric_metal);
    CTHOpSurface -> SetFinish(polishedlumirrorair);
    CTHOpSurface -> SetModel(glisur);
    
    const G4int num = 2;
    G4double Ephoton[num] = {2.038*eV, 4.144*eV};
    
    G4MaterialPropertiesTable* MOpSurfaceProperty = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* COpSurfaceProperty = new G4MaterialPropertiesTable();
    G4MaterialPropertiesTable* TubeSurfaceProperty = new G4MaterialPropertiesTable();
    
    MOpSurfaceProperty -> AddProperty("REFLECTIVITY",PhotonEnergy,Reflectivity1,nEntries);
    
    MOpSurface -> SetMaterialPropertiesTable(MOpSurfaceProperty);
    
    COpSurfaceProperty -> AddProperty("REFLECTIVITY",PhotonEnergy,Reflectivity2,nEntries);
    COpSurfaceProperty -> AddProperty("EFFICIENCY",PhotonEnergy,EfficiencyArray,nEntries);
    
    
    CTHOpSurface -> SetMaterialPropertiesTable(COpSurfaceProperty);
    
    if (fDetMode == 0) {
        G4LogicalSkinSurface* TubeSurface_1 = new
        G4LogicalSkinSurface("TubeMirrorOpS_1",tmirror_log,MOpSurface);
        G4LogicalSkinSurface* lightguideSurface = new
        G4LogicalSkinSurface("lightguideOps",lightguide_log,MOpSurface);
    }
    
    G4LogicalSkinSurface* CathSurface = new
    G4LogicalSkinSurface("CathOpS1", cath_log,CTHOpSurface);
    
    // Generate & Add Material Properties Table attached to the optical surfaces
    
    //OpticalQuartzSurface 
    G4double RefractiveIndex[num] = {1.46, 1.46};
    G4double SpecularLobe[num]    = {0.3, 0.3};
    G4double SpecularSpike[num]   = {0.2, 0.2};
    G4double Backscatter[num]     = {0.2, 0.2};
    
    G4MaterialPropertiesTable* myST1 = new G4MaterialPropertiesTable();
    
    myST1->AddProperty("RINDEX",                Ephoton, RefractiveIndex, num);
    myST1->AddProperty("SPECULARLOBECONSTANT",  Ephoton, SpecularLobe,    num);
    myST1->AddProperty("SPECULARSPIKECONSTANT", Ephoton, SpecularSpike,   num);
    myST1->AddProperty("BACKSCATTERCONSTANT",   Ephoton, Backscatter,     num);
    
    OpQuartzSurface->SetMaterialPropertiesTable(myST1);
    
    //always return the physical World
    return world_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    
    
    
    


