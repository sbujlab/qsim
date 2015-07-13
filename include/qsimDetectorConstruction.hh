
#ifndef qsimDetectorConstruction_h
#define qsimDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class qsimDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    qsimDetectorConstruction();
   ~qsimDetectorConstruction();
		//void StandModeSet();
		void StandModeSet(G4int );

  public:
    G4VPhysicalVolume* Construct();

  private:
    G4double quartz_x;
    G4double quartz_y;
    G4double quartz_z;
		G4int fStandMode;


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


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*qsimDetectorConstruction_h*/
