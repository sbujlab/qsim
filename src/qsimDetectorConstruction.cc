#include "qsimIO.hh"
#include "qsimDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

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
#include "G4ExtrudedSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4TwoVector.hh"
#include "G4Polycone.hh"
#include "G4Trap.hh"
#include "G4GenericTrap.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"

#include "G4NistManager.hh"

#include "G4GDMLParser.hh"


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
void qsimDetectorConstruction::SetDetectorGeomFile(const G4String& detFileName = ""){
    fDetFileName = detFileName;
}

qsimDetectorConstruction::qsimDetectorConstruction(){
   
    fDetFileName="geometry/qsimMother.gdml";
    fIO = NULL;
    fGDMLParser = NULL;  
}

G4VPhysicalVolume* qsimDetectorConstruction::Construct(){
    G4VPhysicalVolume *worldVolume;

    fIO->GrabGDMLFiles(fDetFileName);

    if (fGDMLParser){
        delete fGDMLParser;
    }

    fGDMLParser = new G4GDMLParser();
    fGDMLParser->Clear();
    fGDMLParser->SetOverlapCheck(false);

    fGDMLParser->Read(fDetFileName);

    worldVolume = fGDMLParser->GetWorldVolume();

    //========================================
    // Associate target volumes with beam/target class
    // This has to match what is declared in the GDML volumes
    //=========================================
    
    return worldVolume;
    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
