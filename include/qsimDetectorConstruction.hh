//From brads_qsim branch
#ifndef qsimDetectorConstruction_h
#define qsimDetectorConstruction_h 1

#include "globals.hh"
#include "G4SubtractionSolid.hh"
#include "G4VUserDetectorConstruction.hh"
#include <iostream>
#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class qsimDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    qsimDetectorConstruction();
   ~qsimDetectorConstruction();
	void DetModeSet(G4int );
	void StandModeSet(G4int );
	void LGReflectivitySet(G4double );
  public:
    G4VPhysicalVolume* Construct();

  private:
	G4int fDetMode;
	G4int fStandMode;
    G4double det_x;
    G4double det_y;
    G4double det_z;

    G4double quartz_x;
    G4double quartz_y;
    G4double quartz_z;

	G4double quartz_zPos;

    G4double cone_rmin1;
    G4double cone_rmax1;
    G4double cone_rmin2;
    G4double cone_rmax2;
    G4double cone_z;
    G4double cone_sphi;
    G4double cone_fphi;
	
	G4double rin;
	G4double rout;
	G4double lngth;

	std::ifstream myfile;
	std::ofstream outfile;
	std::ifstream myfile2;

    public:
	G4int gasType;
	G4double beam_angle;
    G4double fDetAngle, fQuartzPolish;
    G4double fLGReflectivity;
    // POSSCAN
    G4double fDetPosX, fDetPosY;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*qsimDetectorConstruction_h*/
