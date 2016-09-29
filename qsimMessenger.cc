#include "qsimMessenger.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"

#include "qsimDetectorConstruction.hh"
#include "qsimIO.hh"
#include "qsimEventAction.hh"
#include "qsimPrimaryGeneratorAction.hh"
#include "qsimSteppingAction.hh"

#include "G4UImanager.hh"
#include "G4UIdirectory.hh"
#include "G4RunManager.hh"

#include "G4VPhysicalVolume.hh"

#include <iostream>

qsimMessenger::qsimMessenger(){
    /*  Initialize all the things it talks to to NULL */

    fIO           = NULL;
    fdetcon       = NULL;
    fevact        = NULL;
    fprigen       = NULL;
    fStepAct      = NULL;

    fRemollDir = new G4UIdirectory("/qsim/");
    fRemollDir->SetGuidance("UI commands of this code");

    fileCmd = new G4UIcmdWithAString("/qsim/filename",this);
    fileCmd->SetGuidance("Output filename");
    fileCmd->SetParameterName("filename", false);

    seedCmd = new G4UIcmdWithAnInteger("/qsim/seed",this);
    seedCmd->SetGuidance("Set random engine seed");
    seedCmd->SetParameterName("seed", false);

		// new

		//fStandModeCmd = new G4UIcmdWithAnInteger("/qsim/fStandMode",this);
		//fStandModeCmd->SetGuidance("Set fStandMode to an option");
		//fStandModeCmd->SetParameterName("standmode", false);
		
		fDetModeCmd = new G4UIcmdWithAnInteger("/qsim/fDetMode",this);
		fDetModeCmd->SetGuidance("Set fDetMode to an option");
		fDetModeCmd->SetParameterName("detmode", false);

		fQModeCmd = new G4UIcmdWithAnInteger("/qsim/fQMode",this);
		fQModeCmd->SetGuidance("Set fQMode to an option");
		fQModeCmd->SetParameterName("qmode", false);
		
		fStandModeCmd = new G4UIcmdWithAnInteger("/qsim/fStandMode",this);
		fStandModeCmd->SetGuidance("Set fStandMode to an option");
		fStandModeCmd->SetParameterName("standmode", false);

		fSourceModeCmd = new G4UIcmdWithAnInteger("/qsim/fSourceMode",this);
		fSourceModeCmd->SetGuidance("Set fSourceMode to an option");
		fSourceModeCmd->SetParameterName("sourcemode", false);

		fQuartzPolishCmd = new G4UIcmdWithADouble("/qsim/fQuartzPolish",this);
		fQuartzPolishCmd->SetGuidance("Set fQuartzPolish to a value");
		fQuartzPolishCmd->SetParameterName("quartzpolish",false);
		
		fDetAngleCmd = new G4UIcmdWithADoubleAndUnit("/qsim/fDetAngle",this);
		fDetAngleCmd->SetGuidance("Set fDetAngle to a value");
		fDetAngleCmd->SetParameterName("detangle",false);

		// POSSCAN
		fDetPosXCmd = new G4UIcmdWithADoubleAndUnit("/qsim/fDetPosX",this);
		fDetPosXCmd->SetGuidance("Set fDetPosX to a value");
		fDetPosXCmd->SetParameterName("detxpos",false);
		
		fDetPosYCmd = new G4UIcmdWithADoubleAndUnit("/qsim/fDetPosY",this);
		fDetPosYCmd->SetGuidance("Set fDetPosY to a value");
		fDetPosYCmd->SetParameterName("detypos",false);
		// old

    fXminCmd = new G4UIcmdWithADoubleAndUnit("/qsim/xmin", this);
    fXminCmd->SetGuidance("Set x range minimum");
    fXminCmd->SetParameterName("xmin", false);

    fXmaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/xmax", this);
    fXmaxCmd->SetGuidance("Set x range maximum");
    fXmaxCmd->SetParameterName("xmax", false);

    fYminCmd = new G4UIcmdWithADoubleAndUnit("/qsim/ymin", this);
    fYminCmd->SetGuidance("Set y range minimum");
    fYminCmd->SetParameterName("ymin", false);

    fYmaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/ymax", this);
    fYmaxCmd->SetGuidance("Set y range maximum");
    fYmaxCmd->SetParameterName("ymax", false); 

    fEminCmd = new G4UIcmdWithADoubleAndUnit("/qsim/emin", this);
    fEminCmd->SetGuidance("Set energy range minimum");
    fEminCmd->SetParameterName("emin", false);

    fEmaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/emax", this);
    fEmaxCmd->SetGuidance("Set Energy range maximum");
    fEmaxCmd->SetParameterName("emax", false);
//
		fthetaMinCmd = new G4UIcmdWithADoubleAndUnit("/qsim/thetamin", this);
    fthetaMinCmd->SetGuidance("Set theta range minimum");
    fthetaMinCmd->SetParameterName("thetamin", false);

    fthetaMaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/thetamax", this);
    fthetaMaxCmd->SetGuidance("Set theta range maximum");
    fthetaMaxCmd->SetParameterName("thetamax", false);


//    fThetaCmd = new G4UIcmdWithADoubleAndUnit("/qsim/theta", this);
//    fThetaCmd->SetGuidance("Set particle theta");
//    fThetaCmd->SetParameterName("theta", false);

//    fPhiCmd = new G4UIcmdWithADoubleAndUnit("/qsim/phi", this);
//    fPhiCmd->SetGuidance("Set particle phi");
//    fPhiCmd->SetParameterName("phi", false);

    fZCmd = new G4UIcmdWithADoubleAndUnit("/qsim/z", this);
    fZCmd->SetGuidance("Set particle z");
    fZCmd->SetParameterName("z", false);

}

qsimMessenger::~qsimMessenger(){
}


void qsimMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue){
    if( cmd == fileCmd ){
	fIO->SetFilename(newValue);
    }

    if( cmd == seedCmd ){
	G4int seed = seedCmd->GetNewIntValue(newValue);
	CLHEP::HepRandom::setTheSeed(seed);
    }

/*

		if (cmd == fStandModeCmd ) {
	G4int x = fStandModeCmd->GetNewIntValue(newValue);
	fdetcon->StandModeSet(x);
		}
*/		
		if (cmd == fDetModeCmd ) {
	G4int x = fDetModeCmd->GetNewIntValue(newValue);
	fdetcon->DetModeSet(x);
		}

		if (cmd == fQModeCmd ) {
	G4int x = fQModeCmd->GetNewIntValue(newValue);
	fdetcon->QModeSet(x);
		}
		
	if (cmd == fStandModeCmd ) {
	G4int x = fStandModeCmd->GetNewIntValue(newValue);
	fdetcon->StandModeSet(x);
		}
		
	if (cmd == fSourceModeCmd ) {
	G4int x = fSourceModeCmd->GetNewIntValue(newValue);
	fprigen->SourceModeSet(x);
		}

	if (cmd == fQuartzPolishCmd ) {
		G4double x = fQuartzPolishCmd->GetNewDoubleValue(newValue);
		fdetcon->fQuartzPolish = x;
	}
	
	if (cmd == fDetAngleCmd ) {
		G4double x = fDetAngleCmd->GetNewDoubleValue(newValue);
		fdetcon->fDetAngle = x;
	}

	// POSSCAN
	
	if (cmd == fDetPosXCmd ) {
		G4double x = fDetPosXCmd->GetNewDoubleValue(newValue);
		fdetcon->fDetPosX = x;
	}

	if (cmd == fDetPosYCmd ) {
		G4double x = fDetPosYCmd->GetNewDoubleValue(newValue);
		fdetcon->fDetPosY = x;
	}

    if( cmd == fXminCmd ){
	G4double x = fXminCmd->GetNewDoubleValue(newValue);
	fprigen->fXmin = x;
    }
    if( cmd == fXmaxCmd ){
	G4double x = fXmaxCmd->GetNewDoubleValue(newValue);
	fprigen->fXmax = x;
    }
    if( cmd == fYminCmd ){
	G4double x = fYminCmd->GetNewDoubleValue(newValue);
	fprigen->fYmin = x;
    }
    if( cmd == fYmaxCmd ){
	G4double x = fYmaxCmd->GetNewDoubleValue(newValue);
	fprigen->fYmax = x;
    }
    if( cmd == fEminCmd ){
	G4double x = fEminCmd->GetNewDoubleValue(newValue);
	fprigen->fEmin = x;
    }
    if( cmd == fEmaxCmd ){
	G4double x = fEmaxCmd->GetNewDoubleValue(newValue);
	fprigen->fEmax = x;
    }
    if( cmd == fthetaMinCmd ){
	G4double x = fthetaMinCmd->GetNewDoubleValue(newValue);
	fprigen->fthetaMin = x;
    }
    if( cmd == fthetaMaxCmd ){
	G4double x = fthetaMaxCmd->GetNewDoubleValue(newValue);
	fprigen->fthetaMax = x;
    }
// INSERT new commands for theta

//    if( cmd == fThetaCmd ){
//	G4double x = fThetaCmd->GetNewDoubleValue(newValue);
//	fprigen->fTheta = x;
//    }

//    if( cmd == fPhiCmd ){
//	G4double x = fPhiCmd->GetNewDoubleValue(newValue);
//	fprigen->fPhi = x;
//    }
    if( cmd == fZCmd ){
	G4double x = fZCmd->GetNewDoubleValue(newValue);
	fprigen->fZ = x;
    }

}
