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
#include "G4ExtrudedSolid.hh"
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
#include "G4TwoVector.hh"
#include "G4Polycone.hh"
#include "G4IntersectionSolid.hh"
#include <iostream>
#include <fstream>
using namespace std;

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
//Position scan
	fQuartzPolish = 0.97;	    
        fDetAngle = 0*deg;
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
    
    quartz_zPos = 0*cm; //-.9*cm;//-1.1*cm; //-.9*cm; //-.6*cm;
    
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
    // the real contruction comes from here 
    
    // Define materials
    
    G4double a, z, density;
    G4int nelements;
    
    
    G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
    G4Element* C = new G4Element("Carbon"  , "C", z=6 , a=12.01*g/mole);
    G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.00*g/mole);
    
    //--- Air 
    G4Material* Air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
    Air->AddElement(N, 70.*perCent);
    Air->AddElement(O, 30.*perCent);
    
    //--- Quartz
    G4Element* Si = new G4Element("Silicon", "Si", z=14 , a=28*g/mole);
    
    G4Material* Quartz = new G4Material("Quartz", density= 2.203*g/cm3, nelements=2);
    Quartz->AddElement(Si, 1);
    Quartz->AddElement(O, 2);
    
    // Aluminum for mirror and stand (need separate materials so that mirror can be reflective)
    G4Element* Al = new G4Element("Aluminum", "Al", z=13 , a=27*g/mole);
    
   
    //---Mylar
    G4Material* Mylar = new G4Material("Mylar",density = 1.38*g/cm3, nelements = 3);
    Mylar->AddElement(C,10);
    Mylar->AddElement(H,8);
    Mylar->AddElement(O,4);

    //---Aluminized-Mylar
    G4Material* AlMylar = new G4Material("AlMylar", density = 0.999999*g/cm3, nelements=4);
    AlMylar->AddElement(C,10);
    AlMylar->AddElement(H,8);
    AlMylar->AddElement(O,4);
    AlMylar->AddElement(Al,3);
    
    //---Paper
    G4Material* Paper = new G4Material("Paper", density = 1.5*g/cm3, nelements = 3);
    Paper->AddElement(C,6);
    Paper->AddElement(H,10);
    Paper->AddElement(O,5);

 
    //---CATH
   
    // Let us make cathode from a special metal (reflectivity 0, efficiency of photoelectrons 25%)
    G4Material* CATH = new G4Material("CATH", density= 2.7*g/cm3, nelements=1);
    CATH->AddElement(Al, 1);
    
      
    
    // Define optical property tables
    
const G4int nEntries = 548;


ifstream myfile;
myfile.open("../../../qsim_code/photon_angle/qsim/src/Simulation.txt");

G4double PhotonEnergy[nEntries];
G4double Quantum_Efficiency[nEntries];
G4double reflectivity[nEntries];

    
    if(myfile.is_open())
{
for(int j = 0; j  < nEntries; j++)
{
myfile >> PhotonEnergy[j] >> reflectivity[j] >> Quantum_Efficiency[j]; 
}
}
  
    
    // Array of photon energies
  /*  G4double PhotonEnergy[nEntries] =
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
        22.6,22.6,22.3,22.1,22.1,22.1,22.0,21.8,21.7,21.3, // 100 entries 4.58 eV 
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
    }; */                                                    // 6.18 eV
   
    // cathode
    G4double Efficiency_cathode[nEntries];
    G4double Reflectivity_cathode[nEntries];       // cathode reflectivity
    
    //quartz
    G4double RefractiveIndex_quartz[nEntries];    // quartz refractive index
    G4double Absorption_quartz[nEntries];         // quartz absorption length
    
    //air
    G4double RefractiveIndex_air[nEntries];    // air refractive index
    
   //reflector- Aluminized Mylar
   G4double Reflectivity_Mirror[nEntries];
   
   //reflectivity for light guide
   G4double Reflect_LG[nEntries];
    
   //reflectivity paper
   G4double Reflect_Paper[nEntries];


 
    for (int i = 0; i < nEntries; i++) {
	
        PhotonEnergy[i] = PhotonEnergy[i]*eV;
        
  	
	//---cathode
	  
	Efficiency_cathode[i] = Quantum_Efficiency[i] ; //0.01*EfficiencyArrayPercent[i]; //QE, defined in %
        
        

        Reflectivity_cathode[i] = 0.;
	
	//---quartz
	RefractiveIndex_quartz[i]= 
	1.438 + (.01197*PhotonEnergy[i]/eV) - (.001955*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV) + (.0004793*PhotonEnergy[i]*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV/eV);
        
        // *** need to update this
        Absorption_quartz[i] 
	= (exp(4.325)*exp(1.191*PhotonEnergy[i]/eV)*exp(-.213*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV)*exp(-.04086*PhotonEnergy[i]*PhotonEnergy[i]*PhotonEnergy[i]/eV/eV/eV))*m;
       
        //--reflector 
        Reflectivity_Mirror[i] = reflectivity[i];
       
        //LG
        Reflect_LG[i] = 0;
        
        //Paper
        Reflect_Paper[i] = 0;

	if (Absorption_quartz[i] > 25*m) {
            Absorption_quartz[i] = 25*m;
        }
        
        //---air
        RefractiveIndex_air[i] = 1.;
    }
    
    
    // Quartz material property table
    G4MaterialPropertiesTable* table_material_quartz = new G4MaterialPropertiesTable();
    table_material_quartz->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex_quartz,nEntries);
    table_material_quartz->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption_quartz,     nEntries);
    
    Quartz->SetMaterialPropertiesTable(table_material_quartz);
    
    // Air material properties table
    // *** need to add absorption length?
    G4MaterialPropertiesTable* table_material_air = new G4MaterialPropertiesTable();
    table_material_air->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex_air, nEntries);
    Air->SetMaterialPropertiesTable(table_material_air);
   

    //Paper material property table
    G4MaterialPropertiesTable* table_material_paper = new G4MaterialPropertiesTable();
    table_material_paper->AddProperty("REFLECTIVITY(P)",PhotonEnergy,Reflect_Paper, nEntries);
    Paper->SetMaterialPropertiesTable(table_material_paper); 
   
    //sensitive detector manager
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    
    
    
    ////////////////////////////////////////////
    // Defining geometry
    ////////////////////////////////////////////
    
    // Volumes
    
    // World and detector box
    
    G4double world_x, world_y, world_z;
    G4double det_x, det_y, det_z;
    
    world_x = world_y = world_z = 275*cm;
    
    det_x = 100*cm;
    det_y = 100*cm;
    det_z = 100*cm;
    
    //---world
    G4Box* world_box = new G4Box("World",world_x,world_y,world_z);
    
    G4LogicalVolume* world_log = new G4LogicalVolume(world_box,Air,"World_log",0,0,0);
    world_log->SetVisAttributes(G4VisAttributes::Invisible);
    
    G4VPhysicalVolume* world_phys = new G4PVPlacement(0,G4ThreeVector(),world_log,"World",0,false,0);
    
    //---det box
    G4Box* det_box = new G4Box("Detector",det_x,det_y,det_z);
    
    G4LogicalVolume* det_log = new G4LogicalVolume(det_box,Air,"Detector_log",0,0,0);
    det_log->SetVisAttributes(G4VisAttributes::Invisible);
    
  ///-------------------single cut quartz   
  ///---Moller style quartz


   //---quartz
   G4double q_lz=8.4*cm; //moller quartz
   G4double q_ly=2.0*cm;  // 1.5cm thick nominal
   G4double q_lx_long=16.0*cm;
   G4double q_lx_short=q_lx_long-q_ly; // we are doing 45 degree single cut
   G4Trap* quartz_box = new G4Trap("Quartz", q_lz, q_ly, q_lx_long, q_lx_short);
   G4LogicalVolume* quartz_log = new G4LogicalVolume(quartz_box,Quartz,"Quartz_log",0,0,0);
   
   //make quartz sensitive detector
   qsimScintDetector* quartzSD = new qsimScintDetector("quartzSD", 10);   // detector ID is 10
   SDman->AddNewDetector(quartzSD);
   quartz_log->SetSensitiveDetector(quartzSD);

   //put quartz in det box  
   G4RotationMatrix* rotate_quartz = new G4RotationMatrix;
   rotate_quartz->rotateX(-M_PI/2.0*rad);  // cut part is facing the beam dump
   G4VPhysicalVolume* quartz_phys = new G4PVPlacement(rotate_quartz,G4ThreeVector(0,0,0),quartz_log,"Quartz", det_log,false,0); //quartz center is on (0,0,0)


 
  ///-------------------single cut quartz   
  ///---Super-elastic style quartz
/*

   //---quartz
   G4double q_lz=17.1*cm; //moller quartz
   G4double q_ly=2.0*cm;  // 1.5cm thick nominal
   G4double q_lx_long=4.0*cm;
   G4double q_lx_short=q_lx_long-q_ly; // we are doing 45 degree single cut
   G4Trap* quartz_box = new G4Trap("Quartz", q_lz, q_ly, q_lx_long, q_lx_short);
   G4LogicalVolume* quartz_log = new G4LogicalVolume(quartz_box,Quartz,"Quartz_log",0,0,0);
   
   //make quartz sensitive detector
   qsimScintDetector* quartzSD = new qsimScintDetector("quartzSD", 10);   // detector ID is 10
   SDman->AddNewDetector(quartzSD);
   quartz_log->SetSensitiveDetector(quartzSD);

   //put quartz in det box
   G4RotationMatrix* rotate_quartz = new G4RotationMatrix;
   rotate_quartz->rotateX(-M_PI/2.0*rad);  // cut part is facing the beam dump
   G4VPhysicalVolume* quartz_phys = new G4PVPlacement(rotate_quartz,G4ThreeVector(0,0,0),quartz_log,"Quartz", det_log,false,0); //quartz center is on (0,0,0)

*/



/*


  ///--------------double cut quartz
  ///---Moller style quartz
  
  //---quartz
  //define the poligon
   G4double thickness=1.0*cm;
   G4double x1=8.0*cm;
   G4double y1=0.0*cm;
   
   G4double x2=8.0*cm - thickness/2;
   G4double y2=thickness/2;
 
   G4double x3=-8.0*cm;
   G4double y3=y2;
 
   G4double x4=-8.0*cm;
   G4double y4=-1.0*y3;
 
   G4double x5=x2;
   G4double y5=-1.0*y2;
 
   std::vector<G4TwoVector> bar_poligon(5);
   bar_poligon[0]= G4TwoVector(x1,y1);
   bar_poligon[1]= G4TwoVector(x2,y2);
   bar_poligon[2]= G4TwoVector(x3,y3);
   bar_poligon[3]= G4TwoVector(x4,y4);
   bar_poligon[4]= G4TwoVector(x5,y5);
 
   G4ExtrudedSolid *quartz_box=new G4ExtrudedSolid("Quartz",bar_poligon, 4.2*cm, G4TwoVector(0,0),1.0, G4TwoVector(0,0), 1.0 );

   G4LogicalVolume* quartz_log = new G4LogicalVolume(quartz_box,Quartz,"Quartz_log",0,0,0);
   
   //make quartz sensitive detector
   qsimScintDetector* quartzSD = new qsimScintDetector("quartzSD", 10);   // detector ID is 10
   SDman->AddNewDetector(quartzSD);
   quartz_log->SetSensitiveDetector(quartzSD);

   //put quartz in det box
   G4RotationMatrix* rotate_quartz = new G4RotationMatrix;
   rotate_quartz->rotateX(-M_PI/2.0*rad);  // cut part is facing the beam dump
   G4VPhysicalVolume* quartz_phys = new G4PVPlacement(rotate_quartz,G4ThreeVector(0,0,0),quartz_log,"Quartz", det_log,false,0); //quartz center is on (0,0,0)

*/

/*
  ///--------------double cut quartz
  ///---super elastic style quartz
  
  //---quartz
  //define the poligon
   G4double thickness=2.0*cm;
   G4double x1=2.0*cm;
   G4double y1=0.0*cm;
   
   G4double x2=2.0*cm - thickness/2;
   G4double y2=thickness/2;
 
   G4double x3=-2.0*cm;
   G4double y3=y2;
 
   G4double x4=-2.0*cm;
   G4double y4=-1.0*y3;
 
   G4double x5=x2;
   G4double y5=-1.0*y2;
 
   std::vector<G4TwoVector> bar_poligon(5);
   bar_poligon[0]= G4TwoVector(x1,y1);
   bar_poligon[1]= G4TwoVector(x2,y2);
   bar_poligon[2]= G4TwoVector(x3,y3);
   bar_poligon[3]= G4TwoVector(x4,y4);
   bar_poligon[4]= G4TwoVector(x5,y5);
 
   G4ExtrudedSolid *quartz_box=new G4ExtrudedSolid("Quartz",bar_poligon, 17.2/2.0*cm, G4TwoVector(0,0),1.0, G4TwoVector(0,0), 1.0 );

   G4LogicalVolume* quartz_log = new G4LogicalVolume(quartz_box,Quartz,"Quartz_log",0,0,0);
   
   //make quartz sensitive detector
   qsimScintDetector* quartzSD = new qsimScintDetector("quartzSD", 10);   // detector ID is 10
   SDman->AddNewDetector(quartzSD);
   quartz_log->SetSensitiveDetector(quartzSD);

   //put quartz in det box
   G4RotationMatrix* rotate_quartz = new G4RotationMatrix;
   rotate_quartz->rotateX(-M_PI/2.0*rad);  // cut part is facing the beam dump
   G4VPhysicalVolume* quartz_phys = new G4PVPlacement(rotate_quartz,G4ThreeVector(0,0,0),quartz_log,"Quartz", det_log,false,0); //quartz center is on (0,0,0)

*/
// FIXME this is where things change -currently 2cm thick Moller single cut. the comments below is for 1cm thick
G4double in = 2.54*cm;

G4double thick_ness = 0.47625*cm;

G4double x_1 = 7*cm;
G4double y_1 = -1*cm;// -0.5*cm;

G4double x_2 = -1*cm;
G4double y_2 =  -1*cm; //-0.5*cm; 

G4double x_3 = -1*cm;
G4double y_3 = 1*cm; //0.5*cm;

G4double x_4 = 7*cm;
G4double y_4 = 1*cm; //0.5*cm;

G4double x_5 = 7*cm + 8*cos(11.5*M_PI/180*rad)*cm;     //7*cm + 8*cos(11.5*M_PI/180*rad)*cm;
G4double y_5 = 1*cm + 8*sin(11.5*M_PI/180*rad)*cm;     //0.5*cm + 8*sin(11.5*M_PI/180*rad)*cm;

G4double x_6 = x_5 + 3.0*cm;//+3.5*cm
G4double y_6 = y_5;

G4double x_7 = x_6;
G4double y_7 = y_6 -8.4*cm;

G4double x_8 = x_1 + 9.19*cos(36.4822801*deg)*cm;//x_1 + 9.75*cos(36.4822801*deg)*cm
G4double y_8 = y_1 - 9.19*sin(36.4822801*deg)*cm;//y_1 - 9.75*sin(36.4822801*deg)*cm

G4double poss = q_lz/2 + thick_ness/2;

std::vector<G4TwoVector> bar_polygon(8);
bar_polygon[0] = G4TwoVector(x_1,y_1);
bar_polygon[1] = G4TwoVector(x_2,y_2);
bar_polygon[2] = G4TwoVector(x_3,y_3);
bar_polygon[3] = G4TwoVector(x_4,y_4);
bar_polygon[4] = G4TwoVector(x_5,y_5);
bar_polygon[5] = G4TwoVector(x_6,y_6);
bar_polygon[6] = G4TwoVector(x_7,y_7);
bar_polygon[7] = G4TwoVector(x_8,y_8);



G4ExtrudedSolid* cone_side = new G4ExtrudedSolid("Cone_Side",bar_polygon,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* cone_log = new G4LogicalVolume(cone_side,AlMylar,"Cone_log",0,0,0);

G4RotationMatrix* rotcone_side = new G4RotationMatrix();
rotcone_side->rotateX(-M_PI/2*rad);
G4VPhysicalVolume* cone_phys1 = new G4PVPlacement(rotcone_side,G4ThreeVector(0,poss,0),cone_log,"Cone side1",det_log,false,0);


G4VPhysicalVolume* cone_phys2 = new G4PVPlacement(rotcone_side,G4ThreeVector(0,-poss,0),cone_log,"Cone side2",det_log,false,0);


G4VisAttributes *con = new G4VisAttributes();
con->SetColour(1.0,0.0,1.0);
cone_log->SetVisAttributes(con);

/*
//paper for quartz
G4double px1 = q_lx_short/2;
G4double py1 = q_lz/2;
G4double pz1 = in/1000;

G4Box* papert = new G4Box("papert",px1,py1,pz1);
G4LogicalVolume* papert_l = new G4LogicalVolume(papert,Paper,"PaperL",0,0,0);
G4VPhysicalVolume* papert_p = new G4PVPlacement(0,G4ThreeVector(0,0,q_ly/2+pz1),papert_l,"Paper Top",det_log,false,0);

G4VisAttributes *paper = new G4VisAttributes();
paper->SetColour(0.0,0.0,0.0);
papert_l->SetVisAttributes(paper);

G4double px2 = q_lx_long/2;
G4double py2 = q_lz/2;
G4double pz2 = in/1000;

G4Box* paperb = new G4Box("paperb",px2,py2,pz2);
G4LogicalVolume* paperb_l = new G4LogicalVolume(paperb,Paper,"PaperL1",0,0,0,true);
G4VPhysicalVolume* paperb_p = new G4PVPlacement(0,G4ThreeVector(0,0,-q_ly/2),paperb_l,"Paper Bot",det_log,false,0);

paperb_l->SetVisAttributes(paper);

G4double px3 = q_lz/2;
G4double py3 = q_ly/2;
G4double pz3 = pz2;

G4Box* paperba = new G4Box("paperba",px2,py2,pz2);
G4LogicalVolume* paperba_l = new G4LogicalVolume(paperba,Paper,"PaperL1",0,0,0,true);

G4VPhysicalVolume* paperba_p = new G4PVPlacement(0,G4ThreeVector(-q_lx_short/2,0,0),paperba_l,"Paper Bot",det_log,false,0);

paperba_l->SetVisAttributes(paper);

G4Box* papers = new G4Box("papers",px2,py3,pz3);
G4LogicalVolume* papers_l = new G4LogicalVolume(papers,Paper,"Papers",0,0,0,true);
G4VPhysicalVolume* papers_p1 = new G4PVPlacement(rotcone_side,G4ThreeVector(0,q_lz/2,0),papers_l,"PaperSide1",det_log,false,0);
G4VPhysicalVolume* papers_p2 = new G4PVPlacement(rotcone_side,G4ThreeVector(0,-q_lz/2,0),papers_l,"PaperSide2",det_log,false,0);

papers_l->SetVisAttributes(paper);
*/


//reflector
G4double xone = x_4;
G4double yone = q_lz/2 + thick_ness;

G4double xtwo = x_5;
G4double ytwo = q_lz/2 + thick_ness;

G4double xthr = x_5;
G4double ythr = -q_lz/2 - thick_ness;

G4double xfo =  x_4;
G4double yfo =  -q_lz/2 - thick_ness;


std::vector<G4TwoVector> barpolygon(4);
barpolygon[0] = G4TwoVector(xone,yone);
barpolygon[1] = G4TwoVector(xtwo,ytwo);
barpolygon[2] = G4TwoVector(xthr,ythr);
barpolygon[3] = G4TwoVector(xfo,yfo);

G4ExtrudedSolid*  ref_box = new G4ExtrudedSolid("REF",barpolygon,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);


G4LogicalVolume* ref_log = new G4LogicalVolume(ref_box,AlMylar,"Ref_log",0,0,0);

G4RotationMatrix* rot_ref = new G4RotationMatrix();
rot_ref->rotateY(11.5*M_PI/180*rad);

G4VPhysicalVolume* refl_phy = new G4PVPlacement(rot_ref,G4ThreeVector(0,0,-thick_ness),ref_log,"Reflector",det_log,false,0);


G4VisAttributes *reflect = new G4VisAttributes();
reflect->SetColour(0.0,1.0,1.0);
ref_log->SetVisAttributes(reflect);
/*
//cone bottom
G4double xuno = x_1;
G4double yuno = q_lz/2 + thick_ness;

G4double xdos = x_8;
G4double ydos = q_lz/2 +  thick_ness;

G4double xtres = x_8;
G4double ytres = -q_lz/2 -thick_ness;

G4double xcuat = x_1;
G4double ycuat = -q_lz/2 -thick_ness;

std::vector<G4TwoVector> barrpolygon(4);
barrpolygon[0] = G4TwoVector(xuno,yuno);
barrpolygon[1] = G4TwoVector(xdos,ydos);
barrpolygon[2] = G4TwoVector(xtres,ytres);
barrpolygon[3] = G4TwoVector(xcuat,ycuat);


G4ExtrudedSolid* cone_bot = new G4ExtrudedSolid("Bottom",barrpolygon,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
*/

G4double xbox =  (9.35/2)*cm;
G4double ybox =  (9.19/2)*cm; //(9.75/2)*cm;
G4double zbox = thick_ness/2;

G4Box* cone_bot = new G4Box("bottom",xbox,ybox,zbox);
G4LogicalVolume* coneb_log = new G4LogicalVolume(cone_bot,AlMylar,"Coneb_log",0,0,0,true);


G4RotationMatrix* rot_coneb = new G4RotationMatrix();
rot_coneb->rotateY(-36.4822801*M_PI/180*rad);


G4VPhysicalVolume* coneb_phy = new G4PVPlacement(rot_coneb,G4ThreeVector(x_1+ ybox*cos(36.4822801*deg),0,-q_ly-ybox*sin(36.4822801*deg)),coneb_log,"coneback",det_log,false,0);



coneb_log->SetVisAttributes(con);




//another cone top part

std::vector<G4TwoVector> bpol1(4);
bpol1[0] = G4TwoVector(x_5,q_lz/2+ thick_ness);
bpol1[1] = G4TwoVector(x_6,q_lz/2+thick_ness);
bpol1[2] = G4TwoVector(x_6,-(q_lz/2+thick_ness));
bpol1[3] = G4TwoVector(x_5, -(q_lz/2+thick_ness));


G4ExtrudedSolid* con_par1 = new G4ExtrudedSolid("Par",bpol1,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* con_log1 = new G4LogicalVolume(con_par1,AlMylar,"Conepar",0,0,0,true);
G4VPhysicalVolume* con_phy11 = new G4PVPlacement(0,G4ThreeVector(0,0,y_5+thick_ness/2),con_log1, "cone part11",det_log,false,0);
G4VPhysicalVolume* con_phy22 = new G4PVPlacement(0,G4ThreeVector(0,0,y_7-thick_ness),con_log1, "cone part22",det_log,false,0);

con_log1->SetVisAttributes(con);



//Light guide
//Light guide side
G4double  lx1 = x_6;
G4double  ly1 = y_6;

G4double  lx2 = x_6 + 2.94*cm;
G4double  ly2 = y_6;

G4double  lx3 = lx2 + 39.43217011394904*cm;
G4double  ly3 = y_6 - 8.022568094008*cm;

G4double  lx4 = lx1 + 2*cm + (39.3*cos(11.5*M_PI/180*rad))*cm;
G4double  ly4 = ly1 -9.35*cm - (39.3*sin(11.5*M_PI/180*rad))*cm;

G4double  lx5 = lx1 + 2*cm;
G4double  ly5 = ly1 - 9.35*cm;

G4double lx6  = lx1;
G4double ly6  = ly1 - 9.35*cm;

std::vector<G4TwoVector> poly(6);
poly[0] = G4TwoVector(lx1,ly1);
poly[1] = G4TwoVector(lx2,ly2);
poly[2] = G4TwoVector(lx3,ly3);
poly[3] = G4TwoVector(lx4,ly4);
poly[4] = G4TwoVector(lx5,ly5);
poly[5] = G4TwoVector(lx6,ly6);

G4ExtrudedSolid* l_g_side = new G4ExtrudedSolid("LGS",poly,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* l_g_slog = new G4LogicalVolume(l_g_side,AlMylar,"LGSL",0,0,0,true);

G4RotationMatrix* rot_l = new G4RotationMatrix();
rot_l->rotateX(-M_PI/2*rad);

G4VPhysicalVolume* l_g_phy1 = new G4PVPlacement(rot_l,G4ThreeVector(-(x_6 - x_5),q_lz/2 + 1.5*thick_ness,thick_ness),l_g_slog,"LGS1",det_log,false,0);
G4VPhysicalVolume* l_g_phy2 = new G4PVPlacement(rot_l,G4ThreeVector(-(x_6 - x_5),-q_lz/2 - 1.5*thick_ness,thick_ness),l_g_slog,"LGS2",det_log,false,0);

G4VisAttributes* lg = new G4VisAttributes();
lg->SetColour(0.0,0.0,1.0);
l_g_slog->SetVisAttributes(lg);


//Light guide top- part 1
G4double xxuno = lx1;
G4double yyuno = poss + 1.5*thick_ness;

G4double xxdos = lx2;
G4double yydos = poss + 1.5*thick_ness;

G4double xxtres = lx2;
G4double yytres = -poss - 1.5*thick_ness;

G4double xxcuat = lx1;
G4double yycuat = -poss - 1.5*thick_ness;


std::vector<G4TwoVector> topp(4);
topp[0] = G4TwoVector(xxuno,yyuno);
topp[1] = G4TwoVector(xxdos,yydos);
topp[2] = G4TwoVector(xxtres,yytres);
topp[3] = G4TwoVector(xxcuat,yycuat);


G4ExtrudedSolid* lg_top = new G4ExtrudedSolid("LGT",topp,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lgt_log = new G4LogicalVolume(lg_top,Mylar,"LGTL",0,0,0,true);
G4VPhysicalVolume* ltb_phy = new G4PVPlacement(0,G4ThreeVector(-(x_6-x_5+thick_ness),0,y_5+thick_ness),lgt_log,"LGBP",det_log,false,0);


G4VisAttributes* lgt = new G4VisAttributes();
lgt->SetColour(0.0,0.0,1.0);
lgt_log->SetVisAttributes(lgt);


//Light guide top - part 2
G4double xxun = lx3 +2*thick_ness;
G4double yyun = poss + 1.5*thick_ness;

G4double xxdeux = lx2;
G4double yydeux = poss + 1.5*thick_ness;

G4double xxtrois = lx2;
G4double yytrois = -poss - 1.5*thick_ness;

G4double xxquatre = lx3 + 2*thick_ness;
G4double yyquatre = -poss - 1.5*thick_ness;


std::vector<G4TwoVector> topb(4);
topb[0] = G4TwoVector(xxun,yyun);
topb[1] = G4TwoVector(xxdeux,yydeux);
topb[2] = G4TwoVector(xxtrois,yytrois);
topb[3] = G4TwoVector(xxquatre,yyquatre);

G4ExtrudedSolid* lg_top1 = new G4ExtrudedSolid("LGT",topb,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lgt_log1 = new G4LogicalVolume(lg_top1,AlMylar,"LGTL",0,0,0,true);


G4RotationMatrix *rot_ll = new G4RotationMatrix();
rot_ll->rotateY(-11.5*M_PI/180*rad);

G4VPhysicalVolume* ltb_phy1 = new G4PVPlacement(rot_ll,G4ThreeVector(-2*cm -1.75*thick_ness,0,poss+ 5.25*thick_ness),lgt_log1,"LGBP",det_log,false,0);



lgt_log1->SetVisAttributes(lgt);



//Light guide bottom- Part 1
G4double xx1 = lx5;
G4double yy1 = -poss - 1.5*thick_ness;

G4double xx2 = lx4 + 1.5*thick_ness;
G4double yy2 = -poss - 1.5*thick_ness;

G4double xx3 = lx4 + 1.5*thick_ness;
G4double yy3 = poss + 1.5*thick_ness;

G4double xx4 = lx5;
G4double yy4 = poss + 1.5*thick_ness;


std::vector<G4TwoVector> botp(4);
botp[0] = G4TwoVector(xx1,yy1);
botp[1] = G4TwoVector(xx2,yy2);
botp[2] = G4TwoVector(xx3,yy3);
botp[3] = G4TwoVector(xx4,yy4);

G4ExtrudedSolid* lg_bot = new G4ExtrudedSolid("LGB",botp,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lgb_log = new G4LogicalVolume(lg_bot,AlMylar,"LGBL",0,0,0,true);

G4VPhysicalVolume* lgb_phy = new G4PVPlacement(rot_ll,G4ThreeVector((-2*cm - thick_ness),0,y_7 + 7*thick_ness),lgb_log,"LGB",det_log,false,0); 


G4VisAttributes* lgb = new G4VisAttributes();
lgb->SetColour(0.0,0.0,1.0);
lgb_log->SetVisAttributes(lgb);



//Light guide bottom- Part 2
G4double xx11 = lx5;
G4double yy11 = -poss - 1.5*thick_ness;

G4double xx22 = lx6;
G4double yy22 = -poss - 1.5*thick_ness;

G4double xx33 = lx6;
G4double yy33 = poss + 1.5*thick_ness;

G4double xx44 = lx5;
G4double yy44 = poss + 1.5*thick_ness;


std::vector<G4TwoVector> botp1(4);
botp1[0] = G4TwoVector(xx11,yy11);
botp1[1] = G4TwoVector(xx22,yy22);
botp1[2] = G4TwoVector(xx33,yy33);
botp1[3] = G4TwoVector(xx44,yy44);

G4ExtrudedSolid* lg_bot2 = new G4ExtrudedSolid("LGB",botp1,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lgb_log2 = new G4LogicalVolume(lg_bot2,AlMylar,"LGBL",0,0,0,true);

G4VPhysicalVolume* lgb_phy2 = new G4PVPlacement(0,G4ThreeVector(-(x_6-x_5),0,-q_lz+ 4*thick_ness),lgb_log2,"LGBP",det_log,false,0);


lgb_log2->SetVisAttributes(lgb);

/*
G4double pos_s = 4.2*cm + thick_ness;

//Double cut 1 cm Moller cone sides
G4double x_11 = 7.0*cm;
G4double y_11 = -0.5*cm;

G4double x_22 = -1.0*cm;
G4double y_22 = -0.5*cm;

G4double x_33 = -1.0*cm;
G4double y_33 = 0.5*cm;

G4double x_44 = 7.0*cm;
G4double y_44 = 0.5*cm;

G4double x_55 = x_44 + 8*cos(13.5*M_PI/180*rad)*cm;
G4double y_55 = y_44 + 8*sin(13.5*M_PI/180*rad)*cm -0.5*cm;

G4double x_66 = x_55;
G4double y_66 = y_55 + 0.5*cm;

G4double x_77 = x_66 + 2.0*cm;
G4double y_77 = y_66;

G4double x_88 = x_77;
G4double y_88 = y_77 - 8.4*cm;

G4double x_99 = x_88 - 2.0*cm;
G4double y_99 = y_88;

G4double x_1010 = x_11 + 8*cos(21*M_PI/180*rad)*cm;
G4double y_1010 = y_11 - 8*sin(21*M_PI/180*rad)*cm;

std::vector <G4TwoVector> polyd(10);
polyd[0] = G4TwoVector(x_11,y_11); 
polyd[1] = G4TwoVector(x_22,y_22);
polyd[2] = G4TwoVector(x_33,y_33);
polyd[3] = G4TwoVector(x_44,y_44);
polyd[4] = G4TwoVector(x_55,y_55);
polyd[5] = G4TwoVector(x_66,y_66);
polyd[6] = G4TwoVector(x_77,y_77);
polyd[7] = G4TwoVector(x_88,y_88);
polyd[8] = G4TwoVector(x_99,y_99);
polyd[9] = G4TwoVector(x_1010,y_1010);

G4RotationMatrix* rot_l = new G4RotationMatrix();
rot_l->rotateX(-90*deg);


G4ExtrudedSolid* doucone = new G4ExtrudedSolid("DC",polyd,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* doucone_log = new G4LogicalVolume(doucone,AlMylar,"DCL",0,0,0,true);
G4VPhysicalVolume* doucone_phy1 = new G4PVPlacement(rot_l,G4ThreeVector(0,pos_s,0),doucone_log,"DCL1",det_log,false,0);
G4VPhysicalVolume* doucone_phy2 = new G4PVPlacement(rot_l,G4ThreeVector(0,-pos_s,0),doucone_log,"DCL2",det_log,false,0);

G4VisAttributes* dc_c = new G4VisAttributes();
dc_c->SetColour(1.0,0.0,1.0);
doucone_log->SetVisAttributes(dc_c);



//mirror
G4double xmir1 = x_44;
G4double ymir1 = pos_s;

G4double xmir2 = x_55;
G4double ymir2 = pos_s;

G4double xmir3 = x_55;
G4double ymir3 = -pos_s;

G4double xmir4 = x_44;
G4double ymir4 = -pos_s - thick_ness;

std::vector <G4TwoVector> polyd1(4);
polyd1[0] = G4TwoVector(xmir1,ymir1);
polyd1[1] = G4TwoVector(xmir2,ymir2);
polyd1[2] = G4TwoVector(xmir3,ymir3);
polyd1[3] = G4TwoVector(xmir4,ymir4);

G4ExtrudedSolid* doum = new G4ExtrudedSolid("DM",polyd1,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* doum_log = new G4LogicalVolume(doum,AlMylar,"DML",0,0,0,true);

G4RotationMatrix* rot_mir = new G4RotationMatrix();
rot_mir->rotateY(13.5*deg);

G4VPhysicalVolume* doum_phy = new G4PVPlacement(rot_mir,G4ThreeVector(thick_ness/2,0,y_11-2*thick_ness),doum_log,"DMP",det_log,false,0);

G4VisAttributes* mir_r = new G4VisAttributes();
mir_r->SetColour(0.0,1.0,1.0);
doum_log->SetVisAttributes(mir_r);



//bottom
G4double xbox1 = 4*cm;
G4double ybox1 = (9.35/2)*cm;
G4double zbox1 = thick_ness/2;

G4Box* bott = new G4Box("Bott",xbox1,ybox1,zbox1);
G4LogicalVolume* bott_log = new G4LogicalVolume(bott,AlMylar,"DCB",0,0,0,true);

G4RotationMatrix* rot_bott = new G4RotationMatrix();
rot_bott->rotateY(-21*deg);

G4VPhysicalVolume* bott_phy = new G4PVPlacement(rot_bott,G4ThreeVector(x_11+ybox1*cos(21*deg),0,-0.5*cm -ybox1*sin(21*deg)),bott_log,"BCP",det_log,false,0);

bott_log->SetVisAttributes(dc_c);


//dconetopbottom
G4double xtop1 = x_66; 
G4double ytop1 = pos_s;

G4double xtop2 = x_77;
G4double ytop2 = pos_s;

G4double xtop3 = x_77;
G4double ytop3 = -pos_s;

G4double xtop4 = x_66;
G4double ytop4 = -pos_s;

std::vector <G4TwoVector> polyd2(4);
polyd2[0] = G4TwoVector(xtop1,ytop1);
polyd2[1] = G4TwoVector(xtop2,ytop2);
polyd2[2] = G4TwoVector(xtop3,ytop3);
polyd2[3] = G4TwoVector(xtop4,ytop4);

G4ExtrudedSolid* dctop = new G4ExtrudedSolid("DCTB",polyd2,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* dctop_log = new G4LogicalVolume(dctop,AlMylar,"DCTBL",0,0,0,true);
G4VPhysicalVolume* dctop_phys1 = new G4PVPlacement(0,G4ThreeVector(0,0,y_55 -thick_ness/2),dctop_log,"DCTBP1",det_log,false,0);
G4VPhysicalVolume* dctop_phys2 = new G4PVPlacement(0,G4ThreeVector(0,0,y_88 +thick_ness/2),dctop_log,"DCTBP2",det_log,false,0);

dctop_log->SetVisAttributes(dc_c);


//cone rear
G4double xrear1 = 1.10*cm; 
G4double yrear1 = (9.35/2)*cm;
G4double zrear1 = thick_ness/2;

G4Box* dcrear = new G4Box("DCREAR",xrear1,yrear1,zrear1);
G4LogicalVolume* dcrear_log = new G4LogicalVolume(dcrear,AlMylar,"DCRL1",0,0,0,true);

G4RotationMatrix* rot_re = new G4RotationMatrix();
rot_re->rotateY(90*deg);

G4VPhysicalVolume* dcrear_phys = new G4PVPlacement(rot_re,G4ThreeVector(11*cm+xbox1*cos(21*deg),0,-7-xbox1*cos(21*deg)),dcrear_log,"DCRP",det_log,false,0);



dcrear_log->SetVisAttributes(dc_c);



//Double cut 2cm Moller cone sides
G4double X1 = 3.0*cm; 
G4double Y1 = -1.0*cm;

G4double X2 = -5.0*cm;
G4double Y2 = -1.0*cm;

G4double X3 = X2;
G4double Y3 = -Y2;

G4double X4 = X1;
G4double Y4 = Y3;

G4double X5 = X4 + 8*cos(13.5*M_PI/180*rad)*cm;
G4double Y5 = Y4 + 8*sin(13.5*M_PI/180*rad)*cm;

G4double X6 = X5 + 2.5*cm; 
G4double Y6 = Y5;

G4double X7 = X6;
G4double Y7 = Y5 - 8.4*cm;

G4double X8 = X5;
G4double Y8 = Y7;

G4double X9 = X1 + 8*cos(20.5*M_PI/180*rad)*cm;
G4double Y9 = Y1 - 8*sin(20.5*M_PI/180*rad)*cm;








//LGDC
G4double lgsx1 = x_66; //X6;
G4double lgsy1 = y_66; //Y6;

G4double lgsx2 = x_66 + 2.53*cm;
G4double lgsy2 = y_66;

G4double lgsx3 = lgsx2 + 40.13*cos(6.5*deg)*cm;
G4double lgsy3 = lgsy2 - 40.13*sin(6.5*deg)*cm;

G4double lgsx4 = x_66 + 2*cm + 39.6*cos(6.5*deg)*cm; 
G4double lgsy4 = lgsy1 - 9.35*cm - 39.6*sin(6.5*deg)*cm; 
 
G4double lgsx5 = x_66 + 2*cm;
G4double lgsy5 = lgsy1 - 9.35*cm; 
  
G4double lgsx6 = lgsx1;
G4double lgsy6 = lgsy1 - 9.35*cm;

std::vector <G4TwoVector> polyd4(6);
polyd4[0] = G4TwoVector(lgsx1,lgsy1);
polyd4[1] = G4TwoVector(lgsx2,lgsy2);
polyd4[2] = G4TwoVector(lgsx3,lgsy3);
polyd4[3] = G4TwoVector(lgsx4,lgsy4);
polyd4[4] = G4TwoVector(lgsx5,lgsy5);
polyd4[5] = G4TwoVector(lgsx6,lgsy6);

G4ExtrudedSolid* lgdc = new G4ExtrudedSolid("LGDC", polyd4,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lgdc_log = new G4LogicalVolume(lgdc, AlMylar, "LGDCL", 0,0,0,true);

G4RotationMatrix* rot = new G4RotationMatrix();
rot->rotateX(-90*deg);

G4VPhysicalVolume* lgdc_phys1 = new G4PVPlacement(rot,G4ThreeVector(0,pos_s+thick_ness,0),lgdc_log, "LGDCP1", det_log,false,0);
G4VPhysicalVolume* lgdc_phys2 = new G4PVPlacement(rot,G4ThreeVector(0,-pos_s-thick_ness,0),lgdc_log, "LGDCP2", det_log,false,0);

G4VisAttributes* lg = new G4VisAttributes();
lg->SetColour(0.0,0.0,1.0);
lgdc_log->SetVisAttributes(lg);


std::vector<G4TwoVector>polyd5(4);
polyd5[0] = G4TwoVector(lgsx2,pos_s+1.5*thick_ness);
polyd5[1] = G4TwoVector(lgsx2, -pos_s-1.5*thick_ness);
polyd5[2] = G4TwoVector(lgsx3+2*thick_ness, -pos_s-1.5*thick_ness);
polyd5[3] = G4TwoVector(lgsx3+2*thick_ness, pos_s+1.5*thick_ness);

G4ExtrudedSolid* lgdct = new G4ExtrudedSolid("LGDCT",polyd5,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lgdct_log = new G4LogicalVolume(lgdct, AlMylar,"LGDCTL",0,0,0,true);

G4RotationMatrix* rot_lgbot = new G4RotationMatrix();
rot_lgbot->rotateY(-6.5*deg);

G4VPhysicalVolume* lgdct_phys = new G4PVPlacement(rot_lgbot, G4ThreeVector(-thick_ness,0,5*cm -1.5*thick_ness),lgdct_log,"LGDCTP",det_log,false,0);

lgdct_log->SetVisAttributes(lg);

std::vector<G4TwoVector>polyd6(4);
polyd6[0] = G4TwoVector(lgsx5,pos_s+1.5*thick_ness);
polyd6[1] = G4TwoVector(lgsx5, -pos_s-1.5*thick_ness);
polyd6[2] = G4TwoVector(lgsx4, -pos_s-1.5*thick_ness);
polyd6[3] = G4TwoVector(lgsx4, pos_s+1.5*thick_ness);

G4ExtrudedSolid* lgdcb = new G4ExtrudedSolid("LGDCB",polyd6,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lgdcb_log = new G4LogicalVolume(lgdcb, AlMylar,"LGDCTL",0,0,0,true);

G4VPhysicalVolume* lgdcb_phys = new G4PVPlacement(rot_lgbot, G4ThreeVector(thick_ness,0,y_88+2*thick_ness),lgdcb_log,"LGDCBP",det_log,false,0);

lgdct_log->SetVisAttributes(lg);
lgdcb_log->SetVisAttributes(lg);


std::vector<G4TwoVector>polyd7(4);
polyd7[0] = G4TwoVector(lgsx1,pos_s+1.5*thick_ness);
polyd7[1] = G4TwoVector(lgsx2, pos_s+1.5*thick_ness);
polyd7[2] = G4TwoVector(lgsx2, -pos_s-1.5*thick_ness);
polyd7[3] = G4TwoVector(lgsx1, -pos_s-1.5*thick_ness);

G4ExtrudedSolid* lt = new G4ExtrudedSolid("LT",polyd7,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lt_log = new G4LogicalVolume(lt, AlMylar,"LTL",0,0,0,true);

G4VPhysicalVolume* lt_phys = new G4PVPlacement(0, G4ThreeVector(0,0,y_55+thick_ness),lt_log,"LTP",det_log,false,0);

std::vector<G4TwoVector>polyd8(4);
polyd8[0] = G4TwoVector(lgsx5,pos_s+1.5*thick_ness);
polyd8[1] = G4TwoVector(lgsx5,-pos_s-1.5*thick_ness);
polyd8[2] = G4TwoVector(lgsx6,-pos_s-1.5*thick_ness);
polyd8[3] = G4TwoVector(lgsx6,pos_s+1.5*thick_ness);

G4ExtrudedSolid* lb = new G4ExtrudedSolid("LB",polyd8,thick_ness/2,G4TwoVector(0,0),1.0,G4TwoVector(0,0),1.0);
G4LogicalVolume* lb_log = new G4LogicalVolume(lb, AlMylar,"LGDCTL",0,0,0,true);

G4VPhysicalVolume* lb_phys = new G4PVPlacement(0, G4ThreeVector(0,0,y_88-thick_ness),lb_log,"LGDCBP",det_log,false,0);

lt_log->SetVisAttributes(lg);
lb_log->SetVisAttributes(lg);


//Double cut 2cm Moller cone sides
G4double X1 = 3.0*cm; 
G4double Y1 = -1.0*cm;

G4double X2 = -5.0*cm;
G4double Y2 = -1.0*cm;

G4double X3 = X2;
G4double Y3 = -Y2;

G4double X4 = X1;
G4double Y4 = Y3;

G4double X5 = X4 + 8*cos(13.5*M_PI/180*rad)*cm;
G4double Y5 = Y4 + 8*sin(13.5*M_PI/180*rad)*cm;

G4double X6 = X5 + 2.5*cm; 
G4double Y6 = Y5;

G4double X7 = X6;
G4double Y7 = Y5 - 8.4*cm;

G4double X8 = X5;
G4double Y8 = Y7;

G4double X9 = X1 + 8*cos(20.5*M_PI/180*rad)*cm;
G4double Y9 = Y1 - 8*sin(20.5*M_PI/180*rad)*cm;
*/


//--cathode

   G4double anini=0*deg;
      G4double anspan=360.0*deg;
         G4double cin = 0;
            G4double cout = 7*cm;   //big enough to collect all the photons
               G4double clngth = 3*mm;






    G4Tubs* cath = new G4Tubs("CATH",cin,cout,clngth,anini,anspan);
   //---cathode logical 
   G4LogicalVolume* cath_log = new G4LogicalVolume(cath,CATH,"cath_log",0,0,0);
    //---make it sensitive
   qsimDetector* cathSD = new qsimDetector("cathSD", 2); //---ID is 2
    
   SDman->AddNewDetector(cathSD);
   cath_log->SetSensitiveDetector(cathSD);
    
   G4VisAttributes *cathatt = new G4VisAttributes();
   cathatt->SetColour(1.0, 1.0, 0.2);
   cath_log->SetVisAttributes(cathatt);

 
   //put cathode in the det box
   G4RotationMatrix* rotate_cath =new G4RotationMatrix;
//   rotate_cath->rotateY(-96.5*deg);
   rotate_cath->rotateY(-101.5*M_PI/180*rad);
   
   //for moller quartz study
//   G4VPhysicalVolume* cath_phys = new G4PVPlacement(rotate_cath,G4ThreeVector(lgsx3 -thick_ness, thick_ness,-4.2*cm -6*thick_ness),cath_log,"Cathode", det_log,false,0);
 G4VPhysicalVolume* cath_phys = new G4PVPlacement(rotate_cath,G4ThreeVector(lx3-8*thick_ness,thick_ness,-q_lz - 4*thick_ness),cath_log,"Cathode",det_log,false,0);
// G4VPhysicalVolume* cath_phys = new G4PVPlacement(rotate_cath,G4ThreeVector(lx3-8*thick_ness,thick_ness,-q_lz- 4*thick_ness),cath_log,"Cathode",det_log,false,0);

//for super elastic quartz study
   // G4VPhysicalVolume* cath_phys = new G4PVPlacement(rotate_cath,G4ThreeVector(2.5*cm,0,0),cath_log,"Cathode", det_log,false,0);

   // last step....put det_log into world
   G4RotationMatrix* rotate_det =new G4RotationMatrix;
  
   
 
  //rotate_det->rotateZ(0*deg);   //no rotation corresponding to world system 
 rotate_det->rotateY(fDetAngle);
  G4VPhysicalVolume* det_phys = new G4PVPlacement(rotate_det,G4ThreeVector(0,0,0),det_log,"detector_phys",world_log,false,0);

// world is buit already

   // Surfaces
   // quartz optical surface
   G4OpticalSurface* OpQuartzSurface = new G4OpticalSurface("QuartzSurface");
   OpQuartzSurface->SetType(dielectric_dielectric);
   OpQuartzSurface->SetFinish(ground);
   OpQuartzSurface->SetModel(glisur);
   OpQuartzSurface->SetPolish(fQuartzPolish);
    
   const G4int num=2;
   G4double Ephoton[num] = {2.038*eV, 4.144*eV};
    
   //OpticalQuartzSurface 
   G4double RefractiveIndex[num] = {1.46, 1.46};
   G4double SpecularLobe[num]    = {0.3, 0.3};
   G4double SpecularSpike[num]   = {0.2, 0.2};
   G4double Backscatter[num]     = {0.2, 0.2};
    
   G4MaterialPropertiesTable* table_surface_quartz = new G4MaterialPropertiesTable();
    
   table_surface_quartz->AddProperty("RINDEX",                Ephoton, RefractiveIndex, num);
   table_surface_quartz->AddProperty("SPECULARLOBECONSTANT",  Ephoton, SpecularLobe,    num);
   table_surface_quartz->AddProperty("SPECULARSPIKECONSTANT", Ephoton, SpecularSpike,   num);
   table_surface_quartz->AddProperty("BACKSCATTERCONSTANT",   Ephoton, Backscatter,     num);
   
   OpQuartzSurface->SetMaterialPropertiesTable(table_surface_quartz);

   G4LogicalBorderSurface* QuartzSurface = new G4LogicalBorderSurface("QuartzSurface",quartz_phys,det_phys,OpQuartzSurface);
  
   //reflector surface
   G4OpticalSurface* OpReflectorSurface = new G4OpticalSurface("ReflectorSurface");
   OpReflectorSurface->SetType(dielectric_metal);
   OpReflectorSurface->SetFinish(polishedlumirrorair);
   OpReflectorSurface->SetModel(glisur);
   

   G4MaterialPropertiesTable* table_surface_mirror = new G4MaterialPropertiesTable();
   table_surface_mirror->AddProperty("REFLECTIVITY",PhotonEnergy,Reflectivity_Mirror,nEntries);

   OpReflectorSurface->SetMaterialPropertiesTable(table_surface_mirror);
   
  G4LogicalSkinSurface* MirrorSurface = new G4LogicalSkinSurface("ReflectorSurface",ref_log,OpReflectorSurface);
 //G4LogicalSkinSurface* MirrorSurface = new G4LogicalSkinSurface("ReflectorSurface",doum_log,OpReflectorSurface);
   
   G4OpticalSurface* OpMylarSurface = new G4OpticalSurface("MylarSurface");
   OpMylarSurface->SetType(dielectric_metal);
   OpMylarSurface->SetFinish(polishedlumirrorair);
   OpMylarSurface->SetModel(glisur);

   G4MaterialPropertiesTable* table_surface_mylar = new G4MaterialPropertiesTable();
   table_surface_mylar->AddProperty("REFLECTIVITY(M)",PhotonEnergy,Reflect_LG,nEntries); 

   OpMylarSurface->SetMaterialPropertiesTable(table_surface_mylar);


  G4LogicalSkinSurface* LGSurface1 = new G4LogicalSkinSurface("LGSurface1",l_g_slog,OpMylarSurface);
  G4LogicalSkinSurface* LGSurface2 = new G4LogicalSkinSurface("LGSurface2",lgt_log,OpMylarSurface);
  G4LogicalSkinSurface* LGSurface3 = new G4LogicalSkinSurface("LGSurface3",lgt_log1,OpMylarSurface);
  G4LogicalSkinSurface* LGSurface4 = new G4LogicalSkinSurface("LGSurface4",lgb_log,OpMylarSurface);   
  G4LogicalSkinSurface* LGSurface5 = new G4LogicalSkinSurface("LGSurface5",lgb_log2,OpMylarSurface); 
  
  G4LogicalSkinSurface* Cone_Side = new G4LogicalSkinSurface("ConeSide",cone_log,OpMylarSurface);
  G4LogicalSkinSurface* Cone_bott = new G4LogicalSkinSurface("ConeBottom",coneb_log,OpMylarSurface); 
    
  //G4LogicalSkinSurface* LGSurface1 = new G4LogicalSkinSurface("LGSurface1",lgdct_log,OpMylarSurface);
  //G4LogicalSkinSurface* LGSurface2 = new G4LogicalSkinSurface("LGSurface2",lgdcb_log,OpMylarSurface);
  //G4LogicalSkinSurface* LGSurface3 = new G4LogicalSkinSurface("LGSurface3",lb_log,OpMylarSurface);
  //G4LogicalSkinSurface* LGSurface4 = new G4LogicalSkinSurface("LGSurface4",lt_log,OpMylarSurface);   
  //4LogicalSkinSurface* LGSurface5 = new G4LogicalSkinSurface("LGSurface5",lgdc_log,OpMylarSurface); 
  
  //G4LogicalSkinSurface* Cone_Side = new G4LogicalSkinSurface("ConeSide",doucone_log,OpMylarSurface);
 // G4LogicalSkinSurface* Cone_bott = new G4LogicalSkinSurface("ConeBottom",bott_log,OpMylarSurface); 
 // G4LogicalSkinSurface* Cone_rear = new G4LogicalSkinSurface("ConeRear",dcrear_log,OpReflectorSurface);
 // G4LogicalSkinSurface* Cone_top = new G4LogicalSkinSurface("ConeTop",dctop_log,OpMylarSurface);


   // cathode surface
  
   G4OpticalSurface* OpCathodeSurface = new G4OpticalSurface("CathodeSurface");
   OpCathodeSurface->SetType(dielectric_metal);
   OpCathodeSurface->SetFinish(polishedlumirrorair);
   OpCathodeSurface->SetModel(glisur);
    
   G4MaterialPropertiesTable* table_surface_cathode = new G4MaterialPropertiesTable();
   table_surface_cathode->AddProperty("REFLECTIVITY", PhotonEnergy, Reflectivity_cathode, nEntries);
   table_surface_cathode->AddProperty("EFFICIENCY", PhotonEnergy, Efficiency_cathode, nEntries );

   OpCathodeSurface->SetMaterialPropertiesTable(table_surface_cathode);

   G4LogicalSkinSurface *cathodeSurface=new G4LogicalSkinSurface("CathodeOpsurface",cath_log,OpCathodeSurface);
  //G4LogicalBorderSurface is a class for surfaces defined by the boundary of two physical volumes
//G4LogicalSkinSurface is a class for the surface surronding a single logical volume


    //always return the physical World
    return world_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    
    
    
    


