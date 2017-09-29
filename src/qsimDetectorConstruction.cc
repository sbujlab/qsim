
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

qsimDetectorConstruction::qsimDetectorConstruction()
{

  det_x = det_y = det_z = 275*cm;
  quartz_x = 1.75*cm; 
  quartz_y = 7.*cm; 
  //Change quartz thickness here. 
  quartz_z = 0.5*cm;

  quartz_zPos = -.0*cm;

    DetModeSet();
    StandModeSet();
//Position scan
    fQuartzPolish = 0.97;
    fDetAngle = 0.*deg;
    fDetPosX = 0.*cm;
    fDetPosY = 0.*cm;

    // fNewStand = false; // Default setting is for the setup to reflect to old cosmic stand. True will go to the new design. Messenger has commands to switch between these at command line or at batch mode run time as well.
    // fAccBeamStand = false; // Only affects stand components: true deletes the lead block.

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
	//Edited 12/16
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

qsimDetectorConstruction::~qsimDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* qsimDetectorConstruction::Construct()
{

//File 
	myfile.open("UVS_45total.txt");
//	outfile.open("points.txt");
//	------------- Materials -------------
  G4double a, z, density;
  G4int nelements;

// Gasses
// 
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Element* elAr = new G4Element("Argon"  , "Ar", z=18, a=39.95*g/mole);
    G4Element* C = new G4Element("Carbon"  , "C", z=6 , a=12.01*g/mole);
    G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.00*g/mole);

//  G4Material* Air = new G4Material("Air", density=1.29*mg/cm3, nelements=1);
//  Air->AddElement(N, 100.*perCent);
//  Air->AddElement(O, 30.*perCent);
  G4Material* Gas;
  G4NistManager* man = G4NistManager::Instance();
  G4Material* N2 = new G4Material("N2", density=.001251*g/cm3, nelements=1);
  N2->AddElement(N, 2); 
  G4Material* Ar = new G4Material("Ar", density=.001662*g/cm3, nelements=1);
  Ar->AddElement(elAr, 1);
  G4Material* CO2 = man->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
  G4Material* Air = man->FindOrBuildMaterial("G4_AIR");

// Quartz
// 
  G4Element* Si = new G4Element("Silicon", "Si", z=14 , a=28*g/mole);

  G4Material* Quartz = new G4Material("Quartz", density= 2.203*g/cm3, nelements=2);
  Quartz->AddElement(Si, 1);
  Quartz->AddElement(O, 2);

// Mirror
// 
  G4Element* Al = new G4Element("Aluminum", "Al", z=13 , a=27*g/mole);

  G4Element* Pb = new G4Element("Lead", "Pb", z=82 , a=207.2*g/mole);

  G4Material* Alu_Mat = new G4Material("Alu_Mat", 2.7*g/cm3, nelements=1);
  Alu_Mat->AddElement(Al, 1);

  G4Material* Pb_Mat = new G4Material("Pb_Mat", 11.34*g/cm3, nelements=1);
  Pb_Mat->AddElement(Pb, 1);

	//G4Material* Pb_Mat=Air; // To remove lead bricks, uncomment.
	
  G4Material* Mirror = new G4Material("Mirror", density= 2.7*g/cm3, nelements=1);
  Mirror->AddElement(Al, 1);

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

// Let us make cathode from a special metal (reflectivity 0, efficiency of photoelectrons 25%)
  G4Material* CATH = new G4Material("CATH", density= 2.7*g/cm3, nelements=1);
  CATH->AddElement(Al, 1);


//
// ------------ Generate & Add Material Properties Table ------------
//


const G4int nEntries = 255;

G4double PhotonEnergy[nEntries]; /* =
                {1.9, 1.92,1.94,1.96,1.98,2.0,2.02,2.04,2.06,
                2.08, 2.10, 2.12,2.14, 2.16,2.18,2.2,2.22,2.24,
                2.26,2.28,2.3,2.32,2.34,2.36,2.38,  
                2.4,2.42,2.44,2.46,2.48,2.5,2.52,2.54,2.56,2.58,
                2.6,2.62,2.64,2.66,2.68,2.7,2.72,2.74,2.76,2.78,
                2.8,2.82,2.84,2.86,2.88,2.9,2.92,2.94,2.96,2.98,
                3,3.02,3.04,3.06,3.08,3.1,3.12,3.14,3.16,3.18,
                3.2,3.22,3.24,3.26,3.28,3.3,3.32,3.34,3.36,3.38,
                3.4,3.42,3.44,3.46,3.48,3.5,3.52,3.54,3.56,3.58,
                3.6,3.62,3.64,3.66,3.68,3.7,3.72,3.74,3.76,3.78,
                3.8,3.82,3.84,3.86,3.88,3.9,3.92,3.94,3.96,3.98,
                4,4.02,4.04,4.06,4.08,4.1,4.12 ,4.14,4.16,4.18,  //Glass cuts off above 4.135eV, 87 entries
                4.2,4.22,4.24,4.26,4.28,4.3,4.32,4.34,4.36,4.38,
                4.4,4.42,4.44,4.46,4.48,4.5,4.52,4.54,4.56,4.58,
                4.6,4.62,4.64,4.66,4.68,4.7,4.72,4.74,4.76,4.78,
                4.8,4.82,4.84,4.86,4.88,4.9,4.92,4.94,4.96,4.98, //  Cut off -> 4.96eV ~ 250nm
                5,5.02,5.04,5.06,5.08,5.1,5.12,5.14,5.16,5.18,   // 5.04eV = 246 nm is the 30% cutoff, 133 entries
                5.2,5.22,5.24,5.26,5.28,5.3,5.32,5.34,5.36,5.38,
                5.4,5.42,5.44,5.46,5.48,5.5,5.52,5.54,5.56,5.58,        
                5.6,5.62,5.64,5.66,5.68,5.7,5.72,5.74,5.76,5.78,
                5.8,5.82,5.84,5.86,5.88,5.9,5.92,5.94,5.96,5.98,
                6,6.02,6.04,6.06,6.08,6.1,6.12,6.14,6.16,6.18, //200 nm
                6.22, 6.26, 6.3, 6.34, 6.38, 6.42, 6.46, 6.5,
                6.54, 6.58, 6.62, 6.66, 6.7, 6.74, 6.78, 6.82, 
                6.86, 6.9, 6.94, 6.98, 7.02, 7.06, 7.1, 7.14,
                7.18, 7.22, 7.26, 7.3, 7.34, 7.38, 7.42, 7.46, 
                7.5, 7.54, 7.58, 7.62, 7.66, 7.7, 7.74, 7.78};
*/
//std::cout<<"trying to open file";
myfile2.open("qe.txt");

//G4double PhotonEnergy[nEntries];
G4double Efficiency4[nEntries];
//G4double reflectivity[nEntries];

//std::cout<<"before";
    if(myfile2.is_open())
{
//std::cout<<"start\n";
char delim2 = ' ';
std::string inen;
int j = 0;
while(!myfile2.peek() != EOF && j<nEntries)
{
std::getline(myfile2, inen, delim2);
PhotonEnergy[j]=(300000000.0/(pow(10, -9)*atof(inen.c_str())))*(4.135667*pow(10, -15));
//std::cout<< PhotonEnergy[j] << std::endl;
std::getline(myfile2, inen);
Efficiency4[j] = atof(inen.c_str());
j++;
}
//std::cout<<j<<std::endl;
}
//std::cout<<"attempting to close\n";
myfile2.close();
//std::cout<<"closed";
/*	G4double PhotonEnergy[nEntries] =
		{1.9, 1.92,1.94,1.96,1.98,2.0,2.02,2.04,2.06,
		2.08, 2.10, 2.12,2.14, 2.16,2.18,2.2,2.22,2.24,
		2.26,2.28,2.3,2.32,2.34,2.36,2.38,  
		2.4,2.42,2.44,2.46,2.48,2.5,2.52,2.54,2.56,2.58,
		2.6,2.62,2.64,2.66,2.68,2.7,2.72,2.74,2.76,2.78,
		2.8,2.82,2.84,2.86,2.88,2.9,2.92,2.94,2.96,2.98,
		3,3.02,3.04,3.06,3.08,3.1,3.12,3.14,3.16,3.18,
		3.2,3.22,3.24,3.26,3.28,3.3,3.32,3.34,3.36,3.38,
		3.4,3.42,3.44,3.46,3.48,3.5,3.52,3.54,3.56,3.58,
		3.6,3.62,3.64,3.66,3.68,3.7,3.72,3.74,3.76,3.78,
		3.8,3.82,3.84,3.86,3.88,3.9,3.92,3.94,3.96,3.98,
		4,4.02,4.04,4.06,4.08,4.1,4.12 ,4.14,4.16,4.18,  //Glass cuts off above 4.135eV, 87 entries
		4.2,4.22,4.24,4.26,4.28,4.3,4.32,4.34,4.36,4.38,
		4.4,4.42,4.44,4.46,4.48,4.5,4.52,4.54,4.56,4.58,
		4.6,4.62,4.64,4.66,4.68,4.7,4.72,4.74,4.76,4.78,
		4.8,4.82,4.84,4.86,4.88,4.9,4.92,4.94,4.96,4.98, //  Cut off -> 4.96eV ~ 250nm
		5,5.02,5.04,5.06,5.08,5.1,5.12,5.14,5.16,5.18,   // 5.04eV = 246 nm is the 30% cutoff, 133 entries
		5.2,5.22,5.24,5.26,5.28,5.3,5.32,5.34,5.36,5.38,
		5.4,5.42,5.44,5.46,5.48,5.5,5.52,5.54,5.56,5.58,	
		5.6,5.62,5.64,5.66,5.68,5.7,5.72,5.74,5.76,5.78,
		5.8,5.82,5.84,5.86,5.88,5.9,5.92,5.94,5.96,5.98,
		6,6.02,6.04,6.06,6.08,6.1,6.12,6.14,6.16,6.18, //200 nm
		6.22, 6.26, 6.3, 6.34, 6.38, 6.42, 6.46, 6.5,
		6.54, 6.58, 6.62, 6.66, 6.7, 6.74, 6.78, 6.82, 
		6.86, 6.9, 6.94, 6.98, 7.02, 7.06, 7.1, 7.14,
		7.18, 7.22, 7.26, 7.3, 7.34, 7.38, 7.42, 7.46, 
		7.5, 7.54, 7.58, 7.62, 7.66, 7.7, 7.74, 7.78};  
*/	
	G4double RefractiveIndex1[nEntries]; //quartz ref index
	G4double Absorption1[nEntries];  //quartz absorbtion
	G4double RefractiveIndex2[nEntries];  //air ref index
	G4double RefractiveIndex3[nEntries];  //mirror ref index
	G4double Reflectivity4[nEntries];  //cathode reflectivity
	G4double Reflectivity3[nEntries];  //mirror reflectivity
	G4double ReflectivityPMT[nEntries];  //pmt window reflectivity
	G4double RefractiveIndexAR[nEntries];  //argon ref index
	G4double RefractiveIndexN2[nEntries];  //nitrogen ref index
	G4double RefractiveIndexCO2[nEntries]; //carbon dioxide ref index

    //reflectivity for light guide
    G4double Reflect_LG[nEntries];
    
    //reflectivity for blackening
    G4double Reflect_Blk[nEntries];
    
    //reflectivity paper
    G4double Reflect_Paper[nEntries];
/*
	G4double Efficiency4[nEntries] = //cathode efficiency
	{
		8.7279, 9.047, 9.3778, 9.7207, 10.076, 10.4444,  10.9285, 10.8284, 11.3268, 11.74, 
		12.1684, 11.2264, 11.6368, 12.0623, 12.5032, 12.6118, 12.5057, 13.0714, 
		13.5483, 14.0427, 12.9653, 13.4394, 13.9306, 14.042, 13.9334, 14.5537, 
		15.0847, 15.6351, 14.4455, 14.9737, 15.5211, 16.2048, 16.0916, 16.2041, 
		16.0947, 16.7945, 16.6863, 16.7936, 16.6895, 16.7929, 16.6926, 16.7921, 
		16.6959, 17.404, 17.3096, 17.4032, 17.3129, 17.4022, 17.3162, 18.0365, 
		17.9527, 18.0356, 17.9561, 18.6928, 18.6162, 18.6919, 18.6197, 18.6909, 
		18.6233, 19.3721, 19.3078, 19.3712, 19.3115, 19.3702, 19.3152, 19.3693, 19.3189, 
		19.3684, 19.3226, 20.0742, 20.0328, 20.0732,  20.0722, 20.0405, 20.0713, 20.0443, 
		20.8027, 20.7811, 20.8016, 20.785, 20.8007, 20.789, 20.7997, 20.793, 21.5575, 
		21.5573, 21.5566,  21.5554, 21.5655, 21.5545, 21.5695, 21.5535,  22.3389, 
		22.3667, 22.3378, 22.371, 22.3367, 22.3753, 22.3357,  22.3346, 22.3838, 
		22.3881, 22.3325,  23.1463, 23.2155, 23.1452, 23.2199, 23.1441, 23.2243, 
		23.143,  23.1419, 23.2332,  23.2376, 23.1396, 23.1385, 23.2465, 23.1374, 
		24.101, 23.9805,  23.9794,  23.9783, 24.1148,  24.1194, 23.976,  23.9748, 
		24.1286, 23.9737, 24.1332,  24.1378, 23.9714, 24.1424, 23.9703, 24.147, 
		24.1516, 23.968,  23.9668, 24.1608, 23.9657, 24.1654, 24.17, 23.9634,  
		23.9623, 24.1793, 23.9611, 24.1839,  23.3353, 22.3017, 21.5156, 21.7265,  
		20.9642, 20.0255,  19.3195,  19.5189, 18.6385, 18.834,  18.8376, 17.9807,  
		17.3469, 17.5389, 16.9235, 16.1456,  15.5764, 15.0274, 15.2039,   14.153,  
		13.0192,  13.0186, 13.1773, 12.5596,  12.117, 12.2688, 11.6899, 11.8383, 
		11.2778, 11.423, 10.8803, 11.0222, 10.1272,  9.7702, 9.9003, 9.4259,  
		8.7734, 9.2177, 8.8925, 8.4642,  8.1659, 8.2795,  7.6007,  7.0746,  7.1745, 
		6.8252, 6.9228,  6.3529, 6.4443, 6.1289, 6.2182, 5.9129, 6, 5.7045, 5.5036, 
		5.7894, 5.5853, 5.3096, 5.1227, 4.9424, 5.3893, 5.1992, 5.0159, 4.7682, 
		4.6002, 4.4384, 4.8399, 4.6692, 4.5045, 4.2819, 4.1312, 4.3464, 4.1931,  
		3.8453, 3.7099, 4.046, 3.9033, 3.7656, 3.5791, 3.6335, 3.453, 3.3314, 
		3.2141, 3.101, 3.3822, 3.263, 3.148, 2.9917, 2.8864, 3.0375, 2.9304, 2.7846, 
		2.6865, 2.592, 2.8276, 2.7278, 2.6316, 2.5006, 2.4126, 2.3277,  2.4497, 
		2.3633, 2.2456, 2.0903, 2.2804, 2.1224,  1.9456, 1.8771, 1.8111, 1.7473	
	}; */
/*	G4cout<<"set data";
	G4double temp;
	for(int i = 0; i < nEntries/2; i++){
		temp = PhotonEnergy[nEntries-i-1];
		PhotonEnergy[nEntries-1-i] = PhotonEnergy[i];
		PhotonEnergy[i] = temp;
	}
*/	G4double wav = 0;
        G4double wavelength = 0; 
	G4double *RefIn1; 
	if(gasType == 1) RefIn1 = RefractiveIndex2;
        else if (gasType == 2) RefIn1 = RefractiveIndexN2;
        else if (gasType == 3) RefIn1 = RefractiveIndexCO2;
        else RefIn1 = RefractiveIndexAR;
	std::string str;
	G4double inwav = 0;
	G4double inref = 0;
	char delim = ' ';
//	outfile<<"W-len\t\tRIAir\t\tRIN2\t\tRICO2\t\tRIAr\t\tEnergy\t\tQE\n";
	for (int i = 0; i < nEntries; i++) {
		RefractiveIndex1[i]= 1.455 -(.005836*PhotonEnergy[i])+(.003374*PhotonEnergy[i]*PhotonEnergy[i]);
		PhotonEnergy[i] = PhotonEnergy[i]*eV;
	

//Aluminum
//		Reflectivity3[i] = 0; //.6;
	
//Aluminum Real
/*   if (PhotonEnergy[i] < 4.135*eV) Reflectivity3[i] = .75;  // regularly .75, .7 below  .56/.53/.46 tunes to 50 PEs
		else if (PhotonEnergy[i] >= 4.135*eV && PhotonEnergy[i] < 6.203*eV) Reflectivity3[i] = .7;
   else Reflectivity3[i] = .6;	*/	// .6
		//Reflectivity3[i] = .6;
//ALZAK		
//		if (PhotonEnergy[i] < 3.26*eV) {
//			Reflectivity3[i]=.93; }
//		else { Reflectivity3[i] = 0;}

// No Mirror
//		Reflectivity3[i] = 0;
		
//		Absorption1[i] = 50.*cm;  //Uniform
   
        Absorption1[i] = (exp(4.325)*exp(1.191*PhotonEnergy[i]/eV)*exp(-.213*PhotonEnergy[i]*PhotonEnergy[i]/(eV*eV))*exp(-.04086*PhotonEnergy[i]*PhotonEnergy[i]*PhotonEnergy[i]/(eV*eV*eV)))*m;
       if (Absorption1[i] > 25*m) {Absorption1[i] = 25*m;}

		wavelength = 1.2398*eV/PhotonEnergy[i]; 
		wav = pow( (wavelength), -2); 
		RefractiveIndex2[i]=1+(.05792105/(238.0185-wav))+(.00167917/(57.362-wav));
		RefractiveIndex3[i]=0;
		Reflectivity4[i]=0;
		RefractiveIndexAR[i]=1+(2.50141*pow(10.,-3)/(91.012-wav))+(5.00283*pow(10.,-4)/(87.892-wav))+(5.22343*pow(10.,-2)/(214.02-wav));
       		RefractiveIndexN2[i]=1+(6.8552*pow(10.,-5))+(3.243157*pow(10.,-2)/(144-wav));
        	RefractiveIndexCO2[i]=1+(6.991*pow(10.,-2)/(166.175-wav))+(1.4472*pow(10.,-3)/(79.609-wav))+(6.42941*pow(10.,-5)/(56.3064-wav))+(5.21306*pow(10.,-5)/(46.0196-wav))+(1.46847*pow(10.,-6)/(.0584738-wav));
/*                if( PhotonEnergy[i] > 2.7*eV ){
                    Efficiency4[i]=.25;
                } else if ( 2.7*eV >= PhotonEnergy[i]  ) {
                    Efficiency4[i]=.2;
                } 
*/		Efficiency4[i] = Efficiency4[i]/100.0;
		ReflectivityPMT[i] = pow((*(RefIn1 + i)-RefractiveIndex1[i])/(*(RefIn1 + i)+RefractiveIndex1[i]),2);

        //LG
        Reflect_LG[i] = .1;

        //Blackening
        Reflect_Blk[i]=0;

        //Paper
        Reflect_Paper[i] = 0;
		 //outfile<<wavelength<<"\t"<<RefractiveIndex2[i]<<"\t\t"<<RefractiveIndexN2[i]<<"\t\t"<<RefractiveIndexCO2[i]<<"\t\t"<<RefractiveIndexAR[i]<<"\t\t"<<PhotonEnergy[i]<<"\t\t"<<Efficiency4[i]<<"\n";
		//Reads from file, must change file path in beginning of program to where your reflectivity table is
		if(myfile.is_open()&&!myfile.eof()){
			do{
				std::getline(myfile, str, delim);
				inwav = atof(str.c_str());
				delim = '0';
				std::getline(myfile, str, delim);
				delim = ' ';
				std::getline(myfile, str, delim);
				inref = atof(str.c_str());
				std::getline(myfile, str);
			}while(inwav < (1000*wavelength));
			//gStyle->SetOptStat(0);
    			//gStyle->SetOptFit(0);			
			Reflectivity3[i] = inref;
		}	
	//PhotonEnergy[i] = PhotonEnergy[i]*eV;

	}
	myfile.close();
	//outfile.close();
//G4cout<<"Closed file";
	
//QUARTZ
	
  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();
  myMPT1->AddProperty("RINDEX",       PhotonEnergy, RefractiveIndex1,nEntries);
  myMPT1->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption1,     nEntries);
  
  Quartz->SetMaterialPropertiesTable(myMPT1);

//
// Air
//

/*  G4double RefractiveIndex2[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };  */

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);
  myMPT2->AddConstProperty("SCINTILLATIONYIELD", 25./MeV);


  const G4int NUMENTRIES = 2;
  G4double Scnt_PP[NUMENTRIES] = { 2.*eV, 6*eV };

  G4double Scnt_FAST[NUMENTRIES] = { 0.5, 0.5 };
  G4double Scnt_SLOW[NUMENTRIES] = { 0.5, 0.5 };

  myMPT2->AddProperty("FASTCOMPONENT", Scnt_PP, Scnt_FAST, NUMENTRIES);
  myMPT2->AddProperty("SLOWCOMPONENT", Scnt_PP, Scnt_SLOW, NUMENTRIES);

  myMPT2->AddConstProperty("RESOLUTIONSCALE", 2.0);
  myMPT2->AddConstProperty("FASTTIMECONSTANT",  1.*ns);
  myMPT2->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
  myMPT2->AddConstProperty("YIELDRATIO", 1.0);
  
  Air->SetMaterialPropertiesTable(myMPT2);


//
// Ar
//

  G4MaterialPropertiesTable* myMPTAR = new G4MaterialPropertiesTable();
  myMPTAR->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexAR, nEntries);
  myMPTAR->AddConstProperty("SCINTILLATIONYIELD", 510./MeV);



  myMPTAR->AddProperty("FASTCOMPONENT", Scnt_PP, Scnt_FAST, NUMENTRIES);
  myMPTAR->AddProperty("SLOWCOMPONENT", Scnt_PP, Scnt_SLOW, NUMENTRIES);

  myMPTAR->AddConstProperty("RESOLUTIONSCALE", 2.0);
  myMPTAR->AddConstProperty("FASTTIMECONSTANT",  1.*ns);
  myMPTAR->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
  myMPTAR->AddConstProperty("YIELDRATIO", 1.0);

  Ar->SetMaterialPropertiesTable(myMPTAR);


//
//CO2
//
  
  const G4int nEC = 41;
  G4double PhotonEC[nEC] = {
	2.04358*eV,2.0664*eV,2.09046*eV,2.14023*eV,
	2.16601*eV,2.20587*eV,2.23327*eV,2.26137*eV,
	2.31972*eV,2.35005*eV,2.38116*eV,2.41313*eV,
	2.44598*eV,2.47968*eV,2.53081*eV,2.58354*eV,
	2.6194*eV,2.69589*eV,2.73515*eV,2.79685*eV,
	2.86139*eV,2.95271*eV,3.04884*eV,3.12665*eV,
	3.2393*eV,3.39218*eV,3.52508*eV,3.66893*eV,
	3.82396*eV,3.99949*eV,4.13281*eV,4.27679*eV,
	4.48244*eV,4.65057*eV,4.89476*eV,5.02774*eV,
	5.16816*eV,5.31437*eV,5.63821*eV,5.90401*eV,
	6.19921*eV};
  G4double CO2_1atm_AbsLen[nEC] = {
	70316.5*m, 66796.2*m, 63314.0*m, 56785.7*m,
	53726.5*m, 49381.2*m, 46640.7*m, 44020.0*m,
	39127.2*m, 36845.7*m, 34671.4*m, 32597.4*m,
	30621.3*m, 28743.4*m, 26154.3*m, 23775.1*m,
	22306.7*m, 19526.3*m, 18263.4*m, 16473.0*m,
	14823.5*m, 12818.8*m, 11053.4*m, 9837.32*m,
	8351.83*m, 6747.67*m, 5648.87*m, 4694.87*m,
	3876.99*m, 3150.27*m, 2706.97*m, 2310.46*m,
	1859.36*m, 1568.2*m, 1237.69*m, 1093.38*m,
	962.586*m, 846.065*m, 643.562*m, 520.072*m,
	133.014*m
	};

  G4MaterialPropertiesTable* myMPTC = new G4MaterialPropertiesTable();
  myMPTC->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexCO2, nEntries);
  myMPTC->AddConstProperty("SCINTILLATIONYIELD", 5./MeV);
  myMPTC->AddProperty("ABSLENGTH",    PhotonEC, CO2_1atm_AbsLen,     nEC);


  myMPTC->AddProperty("FASTCOMPONENT", Scnt_PP, Scnt_FAST, NUMENTRIES);
  myMPTC->AddProperty("SLOWCOMPONENT", Scnt_PP, Scnt_SLOW, NUMENTRIES);


  myMPTC->AddConstProperty("RESOLUTIONSCALE", 2.0);
  myMPTC->AddConstProperty("FASTTIMECONSTANT",  1.*ns);
  myMPTC->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
  myMPTC->AddConstProperty("YIELDRATIO", 1.0);

  CO2->SetMaterialPropertiesTable(myMPTC);


//
//N2
//

  G4MaterialPropertiesTable* myMPTN = new G4MaterialPropertiesTable();
  myMPTN->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexN2, nEntries);
  myMPTN->AddConstProperty("SCINTILLATIONYIELD", 140./MeV);

  

  myMPTN->AddProperty("FASTCOMPONENT", Scnt_PP, Scnt_FAST, NUMENTRIES);
  myMPTN->AddProperty("SLOWCOMPONENT", Scnt_PP, Scnt_SLOW, NUMENTRIES);

  myMPTN->AddConstProperty("RESOLUTIONSCALE", 2.0);
  myMPTN->AddConstProperty("FASTTIMECONSTANT",  1.*ns);
  myMPTN->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
  myMPTN->AddConstProperty("YIELDRATIO", 1.0);

  N2->SetMaterialPropertiesTable(myMPTN);

    //Paper material property table
    G4MaterialPropertiesTable* table_material_paper = new G4MaterialPropertiesTable();
    table_material_paper->AddProperty("REFLECTIVITY(P)",PhotonEnergy,Reflect_Paper, nEntries);
    Paper->SetMaterialPropertiesTable(table_material_paper);

//
// Mirror (refractive index = 0) 
//


/*  G4double RefractiveIndex3[nEntries] =
            { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00 };   */


 

//  G4MaterialPropertiesTable* myMPT3 = new G4MaterialPropertiesTable();
//  myMPT3->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex3, nEntries);
  // myMPT3->AddProperty("ABSLENGTH",    PhotonEnergy, Absorption3, nEntries);  
  //  myMPT3->AddProperty("REFLECTIVITY", PhotonEnergy, Reflectivity3, nEntries);
  //  myMPT3->AddProperty("EFFICIENCY",    PhotonEnergy, Efficiency3, nEntries);  

//  Mirror->SetMaterialPropertiesTable(myMPT3);

//
// CATH
//

/*
 G4double Reflectivity4[nEntries] =
            { 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00,
              0.00, 0.00, 0.00, 0.00 };

  G4double Efficiency4[nEntries] =
           {0.25,  0.25,  0.25,  0.25, 0.25, 0.25,
           0.25,  0.25,  0.25,  0.25, 0.25, 0.25,
           0.25,  0.25,  0.25,  0.25, 0.25, 0.25,
           0.25,  0.25,  0.25,  0.25, 0.25, 0.25,
           0.25,  0.25,  0.25,  0.25, 0.25, 0.25,
           0.25, 0.25 };  */



 
  //G4MaterialPropertiesTable* myMPT4 = new G4MaterialPropertiesTable();
  //myMPT4->AddProperty("REFLECTIVITY",       PhotonEnergy, Reflectivity4,nEntries);
  //myMPT4->AddProperty("EFFICIENCY",    PhotonEnergy, Efficiency4, nEntries);  

  //CATH->SetMaterialPropertiesTable(myMPT4);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();

    ////////////////////////////////////////////
    // Defining geometry
    ////////////////////////////////////////////

    // Volumes

    // World and detector box

    G4double world_x, world_y, world_z;
    G4double det_x, det_y, det_z;

    world_x = world_y = world_z = 275*cm;

    det_x = 150*cm;
    det_y = 50*cm;
    det_z = 50*cm;

    //---world
    G4Box* world_box = new G4Box("World",world_x,world_y,world_z);

    G4LogicalVolume* world_log = new G4LogicalVolume(world_box,man->FindOrBuildMaterial("G4_Galactic"),"World_log",0,0,0);
   // world_log->SetVisAttributes(G4VisAttributes::Invisible);

    G4VPhysicalVolume* world_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),world_log,"World",0,false,0);

    //---det box
    G4Box* det_box = new G4Box("Detector",det_x,det_y,det_z);

    G4LogicalVolume* det_log = new G4LogicalVolume(det_box,Air,"Detector_log",0,0,0);
   // det_log->SetVisAttributes(G4VisAttributes::Invisible);

    ///-------------------single cut quartz
/*    ///---Moller style quartz


    //---quartz
   */ G4double q_lz=8.4*cm; //moller quartz
    G4double q_ly=2.0*cm;  // 1.5cm thick nominal
    G4double q_lx_long=16.0*cm;/*
    G4double q_lx_short=q_lx_long-q_ly; // we are doing 45 degree single cut
    G4Trap* quartz_box = new G4Trap("Quartz", q_lz, q_ly, q_lx_long, q_lx_short);
    G4LogicalVolume* quartz_log = new G4LogicalVolume(quartz_box,Quartz,"Quartz_log",0,0,0);

    //make quartz sensitive detector
    qsimScintDetector* quartzSD = new qsimScintDetector("quartzSD", 10);   // detector ID is 10
    SDman->AddNewDetector(quartzSD);
    quartz_log->SetSensitiveDetector(quartzSD);

    //put quartz in det box
    G4RotationMatrix* rotate_quartz = new G4RotationMatrix;
    rotate_quartz->rotateX(-90.0*M_PI/180.0*rad);  // cut part is facing the beam dump
    G4VPhysicalVolume* quartz_phys = new G4PVPlacement(rotate_quartz,G4ThreeVector(0,0,0),quartz_log,"Quartz", det_log,false,0); //quartz center is on (0,0,0)


*/

    G4double in = 2.54*cm;

    G4double thick_ness = 0.47625*cm;

    G4double x_1 = 7*cm;
    G4double y_1 = -1*cm; //-0.5*cm;

    G4double x_2 = -1*cm;
    G4double y_2 = -1*cm; //-0.5*cm;

    G4double x_3 = -1*cm;
    G4double y_3 = 1*cm;//0.5*cm;

    G4double x_4 = 7*cm;
    G4double y_4 = 1*cm;//0.5*cm;

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
    G4VPhysicalVolume* con_phy22 = new G4PVPlacement(0,G4ThreeVector(0,0,y_7-thick_ness/2),con_log1, "cone part22",det_log,false,0);

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

//---make light guide top sensitive

    qsimDetector* lgtSD = new qsimDetector("lgtSD",3); //---ID is 3

    SDman->AddNewDetector(lgtSD);
    lgt_log1->SetSensitiveDetector(lgtSD);

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

    G4VPhysicalVolume* lgb_phy2 = new G4PVPlacement(0,G4ThreeVector(-(x_6-x_5),0,-q_lz+ 3*thick_ness),lgb_log2,"LGBP",det_log,false,0);


    lgb_log2->SetVisAttributes(lgb);



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

//Blackening Tape
    G4double blkX,blkY,blkZ;
    blkX = 1.0*cm;
    blkY = 1.0*cm;
    blkZ = 0.0001*cm;


    G4Box* black = new G4Box("black",blkX,blkY,blkZ);

    G4LogicalVolume* blk_log = new G4LogicalVolume(black,Mylar,"blkMylar",0,0,0);


    G4VisAttributes *blk_color = new G4VisAttributes();
    blk_color->SetColour(1.0,0.0,0.0);
    //place in light guide
    blk_log->SetVisAttributes(blk_color);
    G4RotationMatrix* rotate_black = new G4RotationMatrix;
    rotate_black->rotateY(-11.5*M_PI/180*rad);
    //G4VPhysicalVolume* black_phys = new G4PVPlacement(rotate_black, G4ThreeVector(-(2+thick_ness)+(xxun-xxdeux)-5*cm,0.0*cm,y_7+8*thick_ness+5.0*cm*sin(11.5*M_PI/180*rad)),blk_log,"BLACK",det_log,false,0);
    


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


    G4int step = 1;
    G4double worldAngle = /*11.5*deg + */beam_angle;
    //if(worldAngle < 0)
	//step = -1;
    rotate_det->rotateY(worldAngle);   //no rotation corresponding to world system
    G4VPhysicalVolume* det_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),det_log,"detector_phys",world_log,false,0);
    //G4VPhysicalVolume* det_phys = new G4PVPlacement(rotate_det,G4ThreeVector((-(x_6-x_5+thick_ness)+step*(10.305)*sin(((worldAngle/deg)/*-11.5*/)*M_PI/180*rad))*cm,0,0),det_log,"detector_phys",world_log,false,0);

// world is buit already

    // Surfaces
  /*  // quartz optical surface
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
*/
    //reflector surface
    G4OpticalSurface* OpReflectorSurface = new G4OpticalSurface("ReflectorSurface");
    OpReflectorSurface->SetType(dielectric_metal);
    OpReflectorSurface->SetFinish(polishedlumirrorair);
    OpReflectorSurface->SetModel(glisur);


    G4MaterialPropertiesTable* table_surface_mirror = new G4MaterialPropertiesTable();
    table_surface_mirror->AddProperty("REFLECTIVITY",PhotonEnergy,Reflectivity3,nEntries);

    OpReflectorSurface->SetMaterialPropertiesTable(table_surface_mirror);

    G4LogicalSkinSurface* MirrorSurface = new G4LogicalSkinSurface("ReflectorSurface",ref_log,OpReflectorSurface);
    //G4LogicalSkinSurface* MirrorSurface = new G4LogicalSkinSurface("ReflectorSurface",doum_log,OpReflectorSurface);

    G4OpticalSurface* OpMylarSurface = new G4OpticalSurface("MylarSurface");
    OpMylarSurface->SetType(dielectric_metal);
    OpMylarSurface->SetFinish(polishedlumirrorair);
    OpMylarSurface->SetModel(glisur);

    G4MaterialPropertiesTable* table_surface_mylar = new G4MaterialPropertiesTable();
    table_surface_mylar->AddProperty("REFLECTIVITY",PhotonEnergy,Reflect_LG,nEntries);

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
    table_surface_cathode->AddProperty("REFLECTIVITY", PhotonEnergy, Reflectivity4, nEntries);
    table_surface_cathode->AddProperty("EFFICIENCY", PhotonEnergy, Efficiency4, nEntries );

    OpCathodeSurface->SetMaterialPropertiesTable(table_surface_cathode);

    G4LogicalSkinSurface *cathodeSurface=new G4LogicalSkinSurface("CathodeOpsurface",cath_log,OpCathodeSurface);
    //G4LogicalBorderSurface is a class for surfaces defined by the boundary of two physical volumes
//G4LogicalSkinSurface is a class for the surface surronding a single logical volume

//Blackening Surface
//
    G4OpticalSurface* blkSurface = new G4OpticalSurface("blackening");
    blkSurface -> SetType(dielectric_metal);
    blkSurface -> SetFinish(polishedlumirrorair);
    blkSurface -> SetModel(glisur);
    
    G4MaterialPropertiesTable* tab_blkSurface = new G4MaterialPropertiesTable();
    tab_blkSurface -> AddProperty("REFLECTIVITY",PhotonEnergy,Reflect_Blk,nEntries);

    blkSurface -> SetMaterialPropertiesTable(tab_blkSurface);


    //always return the physical World
    return world_phys;



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
