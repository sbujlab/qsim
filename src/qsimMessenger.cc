#include "qsimMessenger.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"

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

    fZminCmd = new G4UIcmdWithADoubleAndUnit("/qsim/zmin", this);
    fZminCmd->SetGuidance("Set z range minimum");
    fZminCmd->SetParameterName("zmin", false);

    fZmaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/zmax", this);
    fZmaxCmd->SetGuidance("Set z range maximum");
    fZmaxCmd->SetParameterName("zmax", false);

    fEminCmd = new G4UIcmdWithADoubleAndUnit("/qsim/emin", this);
    fEminCmd->SetGuidance("Set energy range min");
    fEminCmd->SetParameterName("emin", false);

    fEmaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/emax", this);
    fEmaxCmd->SetGuidance("Set energy range max");
    fEmaxCmd->SetParameterName("emax", false);

    fThetaMinCmd = new G4UIcmdWithADoubleAndUnit("/qsim/thetaMin", this);
    fThetaMinCmd->SetGuidance("Set particle theta min");
    fThetaMinCmd->SetParameterName("thetaMin", false);

    fThetaMaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/thetaMax", this);
    fThetaMaxCmd->SetGuidance("Set particle theta max");
    fThetaMaxCmd->SetParameterName("thetaMax", false);

    fPhiMinCmd = new G4UIcmdWithADoubleAndUnit("/qsim/phiMin", this);
    fPhiMinCmd->SetGuidance("Set particle phi min");
    fPhiMinCmd->SetParameterName("phiMin", false);

    fPhiMaxCmd = new G4UIcmdWithADoubleAndUnit("/qsim/phiMax", this);
    fPhiMaxCmd->SetGuidance("Set particle phi max");
    fPhiMaxCmd->SetParameterName("phiMax", false);

    fbeam_angleCmd = new G4UIcmdWithADoubleAndUnit("/qsim/beamAngle", this);
    fbeam_angleCmd->SetGuidance("Set beam angle");
    fbeam_angleCmd->SetParameterName("beam_angle", false);

    fGasTypeCmd = new G4UIcmdWithAnInteger("/qsim/gasType", this);
    fGasTypeCmd->SetGuidance("Set gas type");
    fGasTypeCmd->SetParameterName("gasType", false);
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
    if( cmd == fZminCmd ){
	G4double x = fZminCmd->GetNewDoubleValue(newValue);
	fprigen->fZmin = x;
    }
    if( cmd == fZmaxCmd ){
	G4double x = fZmaxCmd->GetNewDoubleValue(newValue);
	fprigen->fZmax = x;
    }
    if( cmd == fEminCmd ){
	G4double x = fEminCmd->GetNewDoubleValue(newValue);
	fprigen->fEmin = x;
    }
    if( cmd == fEmaxCmd ){
	G4double x = fEmaxCmd->GetNewDoubleValue(newValue);
	fprigen->fEmax = x;
    }

    if( cmd == fThetaMinCmd ){
	G4double x = fThetaMinCmd->GetNewDoubleValue(newValue);
	fprigen->fThetaMin = x;
    }

    if( cmd == fThetaMaxCmd ){
	G4double x = fThetaMaxCmd->GetNewDoubleValue(newValue);
	fprigen->fThetaMax = x;
    }

    if( cmd == fPhiMinCmd ){
	G4double x = fPhiMinCmd->GetNewDoubleValue(newValue);
	fprigen->fPhiMin = x;
    }

    if( cmd == fPhiMaxCmd ){
	G4double x = fPhiMaxCmd->GetNewDoubleValue(newValue);
	fprigen->fPhiMax = x;
    }

    if( cmd == fGasTypeCmd ){
	G4int x = fGasTypeCmd->GetNewIntValue(newValue);
	fdetcon->gasType = x;	
    }
    if( cmd == fbeam_angleCmd ){
	G4double x = fbeam_angleCmd->GetNewDoubleValue(newValue);
        fdetcon->beam_angle = x;
    }
	
}
