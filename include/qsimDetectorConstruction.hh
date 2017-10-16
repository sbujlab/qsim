
#ifndef qsimDetectorConstruction_h
#define qsimDetectorConstruction_h 1

#include "qsimIO.hh"
#include "G4GDMLParser.hh"
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SubtractionSolid.hh"
#include <iostream>
#include <fstream>
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class qsimDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    qsimDetectorConstruction();
   ~qsimDetectorConstruction();
		//void StandModeSet();
		void DetModeSet(G4int );
		void StandModeSet(G4int );
        
   public:
    G4VPhysicalVolume* Construct();
    
    void SetDetectorGeomFile(const G4String&);
    G4GDMLParser *fGDMLParser;
    void SetIO( qsimIO *io ){ fIO = io; }


   private:
    G4String            fDetFileName;
    qsimIO *fIO;

    G4double quartz_x;
    G4double quartz_y;
    G4double quartz_z;
	//G4int fStandMode;
	G4int fDetMode;
	G4int fStandMode;

    G4double det_x, det_y, det_z;

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
	// POSSCAN
	G4double fDetPosX, fDetPosY;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*qsimDetectorConstruction_h*/
