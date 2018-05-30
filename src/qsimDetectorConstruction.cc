#include "qsimDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "qsimDetector.hh"
#include "qsimMonitorDetector.hh"
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
#include "G4GenericTrap.hh"
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
// This is Dustin's config 1A (3-30-2018)

void qsimDetectorConstruction::DetModeSet(G4int detMode = 3) {
    fDetMode = detMode;
    // 0 is PREX-I design
    // 1 is PREX-II prototype (so-called "design 3")
    // 2 lumi
    // 3 showerMax
}

void qsimDetectorConstruction::QModeSet(G4int qMode = 2) {
    fQMode = qMode;
}

void qsimDetectorConstruction::StandModeSet(G4int standMode = 0) {
    fStandMode = standMode;
}


qsimDetectorConstruction::qsimDetectorConstruction() {
    
    DetModeSet();
    QModeSet();
    StandModeSet();

    fQuartzPolish = 0.981;        
    fDetAngle = 0.*deg;
    fDetPosX = 0.*cm;
    fDetPosY = 0.*cm;
   
//========== Quartz Dimensions ==========//

    quartz_x = 246*mm/2; // replaces lines 90-93
    quartz_y = 115*mm/2; // 117.5 (open) ---> 152.5 (close)
    //Change quartz thickness here.
    quartz_z = 10*mm/2; // REPLACE END
    quartz_zPos = 0.0*cm;

//========== Quartz Dimensions ==========//
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

qsimDetectorConstruction::~qsimDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* qsimDetectorConstruction::Construct() {
    
    // Define materials
    
    G4double a, z, density;
    G4int nelements;
    G4Material* Vacuum = new G4Material("Vacuum", z=7., density=14.007*g/mole, 0.000000*g/cm3);
    
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

    //Tungsten
    G4Element* W = new G4Element("Tungsten", "W", z=74 , a=183.84*g/mole);
    G4Material* Tungsten = new G4Material("Tungsten", density= 19.25*g/cm3, nelements=1);
    Tungsten->AddElement(W, 1);
        
    // Define optical property tables
    
    const G4int nEntries = 205;//140;//205
    
    // Array of photon energies
    G4double PhotonEnergy[nEntries] =
    {   2.4,2.42,2.44,2.46,2.48,2.5,2.52,2.54,2.56,2.58,
        2.6,2.62,2.64,2.66,2.68,2.7,2.72,2.74,2.76,2.78,
        2.8,2.82,2.84,2.86,2.88,2.9,2.92,2.94,2.96,2.98,
        3.0,3.02,3.04,3.06,3.08,3.1,3.12,3.14,3.16,3.18,
        3.2,3.22,3.24,3.26,3.28,3.3,3.32,3.34,3.36,3.38,
        3.4,3.42,3.44,3.46,3.48,3.5,3.52,3.54,3.56,3.58,
        3.6,3.62,3.64,3.66,3.68,3.7,3.72,3.74,3.76,3.78,
        3.8,3.82,3.84,3.86,3.88,3.9,3.92,3.94,3.96,3.98,
        4.0,4.02,4.04,4.06,4.08,4.1,4.12,4.14,4.16,4.18,        //Glass cuts off above 4.135eV, 87 entries
        4.2,4.22,4.24,4.26,4.28,4.3,4.32,4.34,4.36,4.38,
        4.4,4.42,4.44,4.46,4.48,4.5,4.52,4.54,4.56,4.58,
        4.6,4.62,4.64,4.66,4.68,4.7,4.72,4.74,4.76,4.78,
        4.8,4.82,4.84,4.86,4.88,4.9,4.92,4.94,4.96,4.98,        // Cut off -> 4.96eV ~ 250nm
        5,  5.02,5.04,5.06,5.08,5.1,5.12,5.14,5.16,5.18,	// 5.04eV = 246 nm is the 30% cutoff, 133 entries
        5.2,5.22,5.24,5.26,5.28,5.3,5.32,5.34,5.36,5.38,
        5.4,5.42,5.44,5.46,5.48,5.5,5.52,5.54,5.56,5.58,
        5.6,5.62,5.64,5.66,5.68,5.7,5.72,5.74,5.76,5.78,
        5.8,5.82,5.84,5.86,5.88,5.9,5.92,5.94,5.96,5.98,
        6,  6.02,6.04,6.06,6.08,6.1,6.12,6.14,6.16,6.18,
        6.21,6.29,6.38,6.48,6.57,6.67,6.78,6.87,6.98,
        7.08,7.20,7.32,7.44,7.56,7.69
    };
    
    // Cathode quantum efficiency
    // Response obtained from the plot of the quantum efficiency as a function of wavelength and then changed to eV for the Bialkali photocathode (synthetic silica)
    
    G4double EfficiencyArrayPercent[nEntries] =
    {   11.0,12.0,12.5,13.1,13.5,14.5,15.2,16.0,16.5,17.0, // percentages here
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
    };                                              // 6.18 eV

    /*G4double EfficiencyArrayPercent[nEntries] = 
    {24.9629, 24.7014, 24.7014, 24.7014, 24.4454, 
     24.1893, 24.4454, 24.7014, 24.7014, 24.7014,
     24.4454, 24.1893, 24.1893, 24.1893, 23.9386,
     23.9386, 23.6879, 23.6879, 23.6879, 23.4423,   //20
     23.4423, 23.1968, 23.1968, 23.1968, 22.9564,
     22.9564, 22.7159, 22.4805, 22.4805, 22.245,
     22.245,  22.0144, 22.0144, 21.7839, 21.7839, 
     21.7839, 21.5581, 21.5581, 21.3323, 21.3323,   //40
     21.3323, 21.1112, 21.1112, 20.89,   20.89,
     20.89,   20.6735, 20.6735, 20.457,  20.457,
     20.457,  20.457,  20.457,  20.2449, 20.2449,
     20.0329, 20.0329, 20.0329, 20.0329, 19.6176,   //80
     19.6176, 19.6176, 19.6176, 19.6176, 19.4143,
     19.4143, 19.2109, 19.2109, 19.2109, 19.2109, 
     19.0118, 19.0118, 19.0118, 18.8127, 18.8127, 
     18.8127, 18.8127, 18.8127, 18.4227, 18.4227,   //100 
     18.4227, 18.4227, 18.4227, 18.2317, 18.2317,
     18.0408, 18.0408, 18.0408, 18.0408, 18.0408, 
     18.0408, 17.8538, 17.8538, 17.8538, 17.6668, 
     17.6668, 17.6668, 17.4836, 17.4836, 17.4836,   //120
     17.3005, 17.3005, 17.3005, 16.9419, 16.9419,
     16.9419, 16.7663, 16.7663, 16.7663, 16.7663,
     16.4187, 16.4187, 16.4187, 16.2467, 16.2467, 
     16.2467, 16.2467, 15.9099, 15.9099, 15.9099,
     15.745,  15.745,  15.745,  15.745,  15.2571, 
     15.2571, 15.2571, 15.2571, 14.4794, 14.4794, 
     14.4794, 14.4794, 13.5975, 13.5975, 13.5975,
     13.5975, 13.0396, 13.0396, 13.0396,                         13.0396};*/                                                      
    
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
    
    det_x = 100*cm;
    det_y = 100*cm;
    det_z = 100*cm;
    
    G4Box* world_box = new G4Box("World",world_x,world_y,world_z);
    
    G4LogicalVolume* world_log = new G4LogicalVolume(world_box,Air,"World",0,0,0);
    
    world_log->SetVisAttributes(G4VisAttributes::Invisible);
    
    G4VPhysicalVolume* world_phys = new G4PVPlacement(0,G4ThreeVector(),world_log,"World",0,false,0);
    
    G4Box* det_box = new G4Box("Detector",det_x,det_y,det_z);
    
    G4LogicalVolume* det_log = new G4LogicalVolume(det_box,Air,"Detector_log",0,0,0);
    
    det_log->SetVisAttributes(G4VisAttributes::Invisible);
    
    
   // FIXME This ^^ detector could be a pre-existing implementation of the monitoring detector we just added below
   // but it is not included in the list of sensitive detectors, so it effectively doesn't exist (maybe also it is just the air volume around our showermax?) 
    
    
   
   
    // First, create solids and logical volumes
    
//========== Upstream Monitoring Vacuum Detector ==========//   
    G4Box* monitor_box = new G4Box("VirtualDetector", 200*cm, 200*cm, 1*cm);
    
    G4LogicalVolume* monitor_log = new G4LogicalVolume(monitor_box,Vacuum,"Vacuum",0,0,0);
   
    qsimMonitorDetector* monitorSD = new qsimMonitorDetector("monitor", 99);
    
    SDman->AddNewDetector(monitorSD);
    monitor_log->SetSensitiveDetector(monitorSD);
    
    G4VisAttributes *monitoratt = new G4VisAttributes();
    monitoratt->SetColour(1.0, 1.0, 0.6);
    monitor_log->SetVisAttributes(monitoratt);
 
    G4RotationMatrix* rotM = new G4RotationMatrix;
    
    rotM->rotateX(0*M_PI/2.*rad);
    rotM->rotateZ(0.*deg);

    G4VPhysicalVolume* monitor_phys = new G4PVPlacement(rotM,G4ThreeVector(0,0,-55*cm),monitor_log,"Vacuum", world_log,false,0);


//========== Quartz Volumes ==========//
    G4double q_yLB = quartz_y - (quartz_z);
    
    G4Trap* quartz_box = new G4Trap("Quartz", 2*quartz_x, 2*quartz_z, 2*quartz_y, 2*q_yLB);
    
    G4LogicalVolume* quartz_log = new G4LogicalVolume(quartz_box,Quartz,"Quartz",0,0,0);
    
    qsimScintDetector* quartzSD = new qsimScintDetector("QuartzSD", 10);
    
    SDman->AddNewDetector(quartzSD);
    quartz_log->SetSensitiveDetector(quartzSD);    
 
    G4RotationMatrix* rotQ = new G4RotationMatrix;
    
    rotQ->rotateX(M_PI/2.*rad);
    rotQ->rotateZ(0.*deg);
    if(fDetMode == 0) {
        rotQ->rotateZ(M_PI*rad);
    }

    G4VPhysicalVolume* quartz_phys = new G4PVPlacement(rotQ,G4ThreeVector(0,0,quartz_zPos),quartz_log,"Quartz", det_log,false,0);
    
    
    
 //========== TUNGSTEN Volumes ==========//
    Wthickness = 8*mm/2-0.01*mm/2; // REPLACES LINE 374 (this is new)

    G4Box* tungsten_box = new G4Box("tungsten_box",quartz_y-quartz_z,quartz_x,Wthickness);

    G4LogicalVolume* tungsten_box_log = new G4LogicalVolume(tungsten_box,Tungsten,"tungsten_box_log",0,0,0);

    G4VisAttributes *tungsten_boxx = new G4VisAttributes();
    tungsten_boxx->SetColour(0.2, 0.2, 0.2);
    tungsten_box_log->SetVisAttributes(tungsten_boxx);
    
    

//========== Quartz and Tungsten Placement ==========//

    G4RotationMatrix* rotTung = new G4RotationMatrix;
    
    rotTung->rotateX(0.*rad);

    G4VPhysicalVolume* tungsten_box_phys_1 // Replaces lines 390-400
    = new G4PVPlacement(rotTung,G4ThreeVector(-10*mm/4,0,quartz_zPos-quartz_z-8*mm/2),tungsten_box_log,"tungsten", det_log,false,0); 

    G4VPhysicalVolume* tungsten_box_phys_2
    = new G4PVPlacement(rotTung,G4ThreeVector(-10*mm/4,0,quartz_zPos-3*quartz_z-3*8*mm/2),tungsten_box_log,"tungsten", det_log,false,0); 

    G4VPhysicalVolume* tungsten_box_phys_3
    = new G4PVPlacement(rotTung,G4ThreeVector(-10*mm/4,0,quartz_zPos-5*quartz_z-5*8*mm/2),tungsten_box_log,"tungsten", det_log,false,0);

    G4VPhysicalVolume* tungsten_box_phys_4
    = new G4PVPlacement(rotTung,G4ThreeVector(-10*mm/4,0,quartz_zPos-7*quartz_z-7*8*mm/2-0.5*mm),tungsten_box_log,"tungsten", det_log,false,0); 

    G4RotationMatrix* rotQ_c1 = new G4RotationMatrix;
    rotQ_c1->rotateX(M_PI/2.*rad);
    rotQ_c1->rotateZ(0.*deg);
    rotQ_c1->rotateX(180.*deg);        

    G4VPhysicalVolume* quartz_phys_c1 
    = new G4PVPlacement(rotQ_c1,G4ThreeVector(0,0,quartz_zPos-2*quartz_z-8*mm),quartz_log,"Quartz", det_log,false,0);

    G4RotationMatrix* rotQ_c2 = new G4RotationMatrix;
    rotQ_c2->rotateX(M_PI/2.*rad);
    rotQ_c2->rotateZ(0.*deg);

    G4VPhysicalVolume* quartz_phys_c2
    = new G4PVPlacement(rotQ_c2,G4ThreeVector(0,0,quartz_zPos-4*quartz_z-2*8*mm),quartz_log,"Quartz", det_log,false,0);

    G4RotationMatrix* rotQ_c3 = new G4RotationMatrix;
    rotQ_c3->rotateX(M_PI/2.*rad);
    rotQ_c3->rotateZ(0.*deg);
    rotQ_c3->rotateX(180.*deg);

    G4VPhysicalVolume* quartz_phys_c3
    = new G4PVPlacement(rotQ_c3,G4ThreeVector(0,0,quartz_zPos-6*quartz_z-3*8*mm),quartz_log,"Quartz", det_log,false,0); 
//================================================================================================================================================



//========== Light Guide ==========//
  
    G4Trap* mirror_box_1 = new G4Trap("mirror_box_1",113.17*mm/2,64*mm/2,0.5*mm/2,0.5*mm/2,88.58*mm/2); 

    G4LogicalVolume* mirror_box_1_log = new G4LogicalVolume(mirror_box_1,Mirror,"mirror_box_1_log",0,0,0);

    G4VisAttributes *mirror_boxx_1 = new G4VisAttributes();
    mirror_boxx_1->SetForceWireframe(true);
    mirror_box_1_log->SetVisAttributes(mirror_boxx_1);

    G4Trap* mirror_box_2 = new G4Trap("mirror_box_2",70*mm/2,113.17*mm/2,0.5*mm/2,0.5*mm/2,184.33*mm/2);

    G4LogicalVolume* mirror_box_2_log = new G4LogicalVolume(mirror_box_2,Mirror,"mirror_box_1_1_log",0,0,0);

    G4VisAttributes *mirror_boxx_2 = new G4VisAttributes();
    mirror_boxx_2->SetForceWireframe(true);
    mirror_box_2_log->SetVisAttributes(mirror_boxx_2);

    G4Trap* mirror_box_4 = new G4Trap("mirror_box_4",246*mm/2,246*mm/2,0.5*mm/2,0.5*mm/2,91.93*mm/2);

     G4LogicalVolume* mirror_box_4_log = new G4LogicalVolume(mirror_box_4,Mirror,"mirror_box_4_log",0,0,0);

    G4VisAttributes *mirror_boxx_4 = new G4VisAttributes();
    mirror_boxx_4->SetForceWireframe(true);
    mirror_box_4_log->SetVisAttributes(mirror_boxx_4);

    G4Trap* mirror_box_6 = new G4Trap("mirror_box_6",68*mm/2,246*mm/2,0.5*mm/2,0.5*mm/2,162.85*mm/2);

    G4LogicalVolume* mirror_box_6_log = new G4LogicalVolume(mirror_box_6,Mirror,"mirror_box_6_log",0,0,0);

    G4VisAttributes *mirror_boxx_6 = new G4VisAttributes();
    mirror_boxx_6->SetForceWireframe(true);
    mirror_box_6_log->SetVisAttributes(mirror_boxx_6); 
        
                
// Stuff that doesn't need tweaking 
        
    
    G4Trap* mirror_box_3 = new G4Trap("mirror_box_3",94.42*mm/2,70.15/2*mm,0.5*mm/2,0.5*mm/2,59.24*mm/2);

    G4LogicalVolume* mirror_box_3_log = new G4LogicalVolume(mirror_box_3,Mirror,"mirror_box_3_log",0,0,0);

    G4VisAttributes *mirror_boxx_3 = new G4VisAttributes();
    mirror_boxx_3->SetForceWireframe(true);
    mirror_box_3_log->SetVisAttributes(mirror_boxx_3);

    G4Trap* mirror_box_5 = new G4Trap("mirror_box_5",131.31*mm/2,168.84*mm/2,0.5*mm/2,0.5*mm/2,67.48*mm/2);

    G4LogicalVolume* mirror_box_5_log = new G4LogicalVolume(mirror_box_5,Mirror,"mirror_box_4_log",0,0,0);

    G4VisAttributes *mirror_boxx_5 = new G4VisAttributes();
    mirror_boxx_5->SetForceWireframe(true);
    mirror_box_5_log->SetVisAttributes(mirror_boxx_5);
    
    

//========== SUITCASE MIRROR DIMENSIONS ==========//

    G4Box* mirror_box_7 = new G4Box("mirror_box_5",115*mm/2,0.5*mm/2,64*mm/2+1.0*mm); 

    G4LogicalVolume* mirror_box_7_log = new G4LogicalVolume(mirror_box_7,Mirror,"mirror_box_7_log",0,0,0);

    G4VisAttributes *mirror_boxx_7 = new G4VisAttributes();
    mirror_boxx_7->SetForceWireframe(true);
    mirror_box_7_log->SetVisAttributes(mirror_boxx_7);

    G4Box* mirror_box_8 = new G4Box("mirror_box_8",115*mm/2,246*mm/2,0.5*mm/2); 

    G4LogicalVolume* mirror_box_8_log = new G4LogicalVolume(mirror_box_8,Mirror,"mirror_box_8_log",0,0,0);

    G4VisAttributes *mirror_boxx_8 = new G4VisAttributes();
    mirror_boxx_8->SetForceWireframe(true);
    mirror_box_8_log->SetVisAttributes(mirror_boxx_8);

    G4Box* mirror_box_9 = new G4Box("mirror_box_6",0.5*mm/2,246*mm/2,64*mm/2); 
        
    G4LogicalVolume* mirror_box_9_log = new G4LogicalVolume(mirror_box_9,Mirror,"mirror_box_9_log",0,0,0);

    G4VisAttributes *mirror_boxx_9 = new G4VisAttributes();
    mirror_boxx_9->SetForceWireframe(true);
    mirror_box_9_log->SetVisAttributes(mirror_boxx_9);
//================================================================================================================================================
    
    
    
    // PMT and cathode
   
    G4double anini = 0.*deg;
    G4double anspan = 360.*deg;
 
    G4double prin = 0;
    G4double prout = 3.81*cm;
    G4double plngth = 1.5*mm;
    
    G4Tubs* pmt = new G4Tubs("PMT",prin,prout,plngth,anini,anspan);
    
    G4LogicalVolume* pmt_log = new G4LogicalVolume(pmt,Air,"PMT",0,0,0);

    G4String DetSDname = "tracker1";
    
    G4double cin = 0;
    G4double cout = 3.81*cm;
    G4double clngth = 0.1*mm;
    
    G4Tubs* cath = new G4Tubs("CATH",cin,cout,clngth,anini,anspan);
    
    G4LogicalVolume* cath_log = new G4LogicalVolume(cath,CATH,"CATH",0,0,0);
    
    qsimDetector* cathSD = new qsimDetector("cath", 2);
    
    SDman->AddNewDetector(cathSD);
    cath_log->SetSensitiveDetector(cathSD);
    
    G4VisAttributes *cathatt = new G4VisAttributes();
    cathatt->SetColour(1.0, 1.0, 0.2);
    cath_log->SetVisAttributes(cathatt);
    

    
    //========== ShowerMax Translations ==========//
    
    G4RotationMatrix* detrot = new G4RotationMatrix;
    G4RotationMatrix* rot_pmt = new G4RotationMatrix;

    G4RotationMatrix* rotM_1 = new G4RotationMatrix; 
    rotM_1->rotateY(90*deg);
    rotM_1->rotateX(0*deg); 

    G4ThreeVector zTrans_1(104.29*mm,123*mm,-27*mm);

    G4VPhysicalVolume* mirror_box_1_phys = new G4PVPlacement(rotM_1,zTrans_1,mirror_box_1_log,"mirror_box_1_phys",det_log,false,0);


    G4RotationMatrix* rotM_1_1 = new G4RotationMatrix;
    rotM_1_1->rotateY(90*deg);
    rotM_1_1->rotateX(0*deg); 

    G4ThreeVector zTrans_1_1(104.29*mm,-123*mm,-27*mm);

    G4VPhysicalVolume* mirror_box_1_1_phys = new G4PVPlacement(rotM_1_1,zTrans_1_1,mirror_box_1_log,"mirror_box_1_1_phys",det_log,false,0);


    G4RotationMatrix* rotM_2 = new G4RotationMatrix;
    rotM_2->rotateY(90*deg);
    rotM_2->rotateX(28.87*deg); 

    G4ThreeVector zTrans_2(229.29*mm,78.5*mm,-27*mm);

    G4VPhysicalVolume* mirror_box_2_phys = new G4PVPlacement(rotM_2,zTrans_2,mirror_box_2_log,"mirror_box_2_phys",det_log,false,0);


    G4RotationMatrix* rotM_2_1 = new G4RotationMatrix;
    rotM_2_1->rotateY(90*deg);
    rotM_2_1->rotateX(-28.87*deg); 

    G4ThreeVector zTrans_2_1(229.29*mm,-78.5*mm,-27*mm);

    G4VPhysicalVolume* mirror_box_2_1_phys = new G4PVPlacement(rotM_2_1,zTrans_2_1,mirror_box_2_log,"mirror_box_2_1_phys",det_log,false,0);


    G4RotationMatrix* rotM_4 = new G4RotationMatrix;
    rotM_4->rotateZ(90*deg);
    rotM_4->rotateX(-105.51*deg); 

    G4ThreeVector zTrans_4(104.29*mm,0*mm,17.29*mm);

    G4VPhysicalVolume* mirror_box_4_phys = new G4PVPlacement(rotM_4,zTrans_4,mirror_box_4_log,"mirror_box_4_phys",det_log,false,0);


    G4RotationMatrix* rotM_4_1 = new G4RotationMatrix;
    rotM_4_1->rotateZ(90*deg);
    rotM_4_1->rotateX(-74.49*deg); 

    G4ThreeVector zTrans_4_1(104.29*mm,0*mm,-71.29*mm);

    G4VPhysicalVolume* mirror_box_4_1_phys = new G4PVPlacement(rotM_4_1,zTrans_4_1,mirror_box_4_log,"mirror_box_4_1_phys",det_log,false,0);


    G4RotationMatrix* rotM_6 = new G4RotationMatrix;
    rotM_6->rotateZ(90*deg);
    rotM_6->rotateX(-82.38*deg); 

    G4ThreeVector zTrans_6(229.29*mm,0*mm,18.79*mm);

    G4VPhysicalVolume* mirror_box_6_phys = new G4PVPlacement(rotM_6,zTrans_6,mirror_box_6_log,"mirror_box_6_phys",det_log,false,0);


    G4RotationMatrix* rotM_6_1 = new G4RotationMatrix;
    rotM_6_1->rotateZ(90*deg);
    rotM_6_1->rotateX(-97.62*deg); 

    G4ThreeVector zTrans_6_1(229.29*mm,0*mm,-72.79*mm);

    G4VPhysicalVolume* mirror_box_6_1_phys = new G4PVPlacement(rotM_6_1,zTrans_6_1,mirror_box_6_log,"mirror_box_6_1_phys",det_log,false,0); 

 

// Stuff that doesn't need tweaked
        
        
    G4ThreeVector zTrans_3((289.77/*+17.5*/)*mm,49.79*mm,-27.75*mm);

    G4RotationMatrix* rotM_3 = new G4RotationMatrix;
    rotM_3->rotateY(90*deg);
    rotM_3->rotateX(32.38*deg+180*deg);


    G4ThreeVector zTrans_3_1((289.77/*+17.5*/)*mm,-49.79*mm,-27.75*mm);

    G4RotationMatrix* rotM_3_1 = new G4RotationMatrix;
    rotM_3_1->rotateY(90*deg);
    rotM_3_1->rotateX(-32.38*deg+180*deg);



    G4ThreeVector zTrans_5((231.96/*+17.5*/)*mm,0.0*mm,-82.91*mm);

    G4RotationMatrix* rotM_5 = new G4RotationMatrix;
    rotM_5->rotateZ(90*deg);
    rotM_5->rotateX(-90*deg-13.63*deg);

    
    G4ThreeVector zTrans_5_1((231.96/*+17.5*/)*mm,0.0*mm,27.41*mm);

    G4RotationMatrix* rotM_5_1 = new G4RotationMatrix;
    rotM_5_1->rotateZ(90*deg);
    rotM_5_1->rotateX(-90*deg+13.63*deg);

        
        
    //========== SUITCASE MIRROR TRANSLATIONS ==========//
    
    G4ThreeVector zTrans_7(5*mm/2,123*mm+0.5*mm/2+0.01*mm,-27*mm); 

    G4RotationMatrix* rotM_7 = new G4RotationMatrix;
    rotM_7->rotateY(0.0*deg);
    rotM_7->rotateZ(0.0*deg);

    G4VPhysicalVolume* mirror_box_7_phys = new G4PVPlacement(rotM_7,zTrans_7,mirror_box_7_log,"mirror_box_7_phys", det_log,false,0);


    G4ThreeVector zTrans_7_1(5*mm/2,-123*mm-0.5*mm/2-0.01*mm,-27*mm);

    G4RotationMatrix* rotM_7_1 = new G4RotationMatrix;
    rotM_7_1->rotateY(0.0*deg);
    rotM_7_1->rotateZ(0.0*deg);

    G4VPhysicalVolume* mirror_box_7_1_phys = new G4PVPlacement(rotM_7_1,zTrans_7_1,mirror_box_7_log,"mirror_box_7_1_phys", det_log,false,0);

    G4ThreeVector zTrans_8(5*mm/2,0.0*mm,10*mm/2+0.5*mm/2+0.01*mm);

    G4RotationMatrix* rotM_8 = new G4RotationMatrix;
    rotM_8->rotateY(0.0*deg);
    rotM_8->rotateZ(0.0*deg);

    G4VPhysicalVolume* mirror_box_8_phys = new G4PVPlacement(rotM_8,zTrans_8,mirror_box_8_log,"mirror_box_8_phys", det_log,false,0);

    G4ThreeVector zTrans_8_1(5*mm/2,0.0*mm,-7*10*mm/2-3*8*mm-0.5*mm/2-0.01*mm);

    G4RotationMatrix* rotM_8_1 = new G4RotationMatrix;
    rotM_8_1->rotateY(0.0*deg);
    rotM_8_1->rotateZ(0.0*deg);

    G4VPhysicalVolume* mirror_box_8_1_phys = new G4PVPlacement(rotM_8_1,zTrans_8_1,mirror_box_8_log,"mirror_box_8_1_phys", det_log,false,0);

    G4ThreeVector zTrans_9(-55*mm-0.5*mm/2-0.01*mm,0.0*mm,-27*mm); 
    G4RotationMatrix* rotM_9 = new G4RotationMatrix;
    rotM_9->rotateY(0.0*deg);
    rotM_9->rotateZ(0.0*deg);

    G4VPhysicalVolume* mirror_box_9_phys = new G4PVPlacement(rotM_9,zTrans_9,mirror_box_9_log,"mirror_box_9_phys", det_log,false,0);



//========== PMT Placement ==========//
    rot_pmt -> rotateY(M_PI/2.*rad);
    G4VPhysicalVolume* pmt_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(310*mm+plngth,0.0*mm,-27*mm),pmt_log,"PMT",det_log,false,0); // Replaces lines 1046-1047 
    G4VPhysicalVolume* cath_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(310*mm+2*plngth,0.0*mm,-27*mm),cath_log,"CATH",det_log,false,0); 

//========== PMT Placement ==========//

    G4VPhysicalVolume* det_phys = new G4PVPlacement(detrot,G4ThreeVector(fDetPosX,fDetPosY,0.*cm),det_log,"detector_phys",world_log,false,0);
    
    
    // Surfaces
        
    // quartz
    G4OpticalSurface* OpQuartzSurface = new G4OpticalSurface("QuartzSurface");
    OpQuartzSurface->SetType(dielectric_dielectric);
    OpQuartzSurface->SetFinish(ground);
    OpQuartzSurface->SetModel(glisur);
    OpQuartzSurface->SetPolish(fQuartzPolish);
    
    G4LogicalBorderSurface* QuartzSurface = new G4LogicalBorderSurface("QuartzSurface",quartz_phys,det_phys,OpQuartzSurface);

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
    
    

    G4LogicalSkinSurface* MSurface_1 = new G4LogicalSkinSurface("MirrorOpS_1",mirror_box_1_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_2 = new G4LogicalSkinSurface("MirrorOpS_2",mirror_box_2_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_3 = new G4LogicalSkinSurface("MirrorOpS_3",mirror_box_3_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_4 = new G4LogicalSkinSurface("MirrorOpS_4",mirror_box_4_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_5 = new G4LogicalSkinSurface("MirrorOpS_5",mirror_box_5_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_6 = new G4LogicalSkinSurface("MirrorOpS_6",mirror_box_6_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_7 = new G4LogicalSkinSurface("MirrorOpS_7",mirror_box_7_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_8 = new G4LogicalSkinSurface("MirrorOpS_8",mirror_box_8_log,MOpSurface);

    G4LogicalSkinSurface* MSurface_9 = new G4LogicalSkinSurface("MirrorOpS_9",mirror_box_9_log,MOpSurface);
    

    G4LogicalSkinSurface* CathSurface = new G4LogicalSkinSurface("CathOpS1", cath_log,CTHOpSurface);
    
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
