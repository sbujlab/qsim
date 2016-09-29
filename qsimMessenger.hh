#ifndef qsimMessenger_HH
#define qsimMessenger_HH

#include "globals.hh"
#include "qsimtypes.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4VModularPhysicsList.hh"

/*!
 *   Global messenger class
 */

class qsimIO;
class qsimDetectorConstruction;
class qsimEventAction;
class qsimPrimaryGeneratorAction;
class qsimSteppingAction;

class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIdirectory;

class qsimMessenger : public G4UImessenger {
    public:
       	qsimMessenger();
       	~qsimMessenger();

	void SetIO( qsimIO *io ){ fIO = io; }
	void SetPriGen( qsimPrimaryGeneratorAction *pg ){ fprigen = pg; }
	void SetDetCon( qsimDetectorConstruction *dc ){ fdetcon= dc; }
	void SetEvAct( qsimEventAction *ev ){ fevact = ev; }
	void SetStepAct( qsimSteppingAction *st ){ fStepAct = st; }

	void SetNewValue(G4UIcommand* cmd, G4String newValue);

    private:
	qsimIO *fIO;
	qsimDetectorConstruction *fdetcon;
	qsimEventAction *fevact;
	qsimPrimaryGeneratorAction *fprigen;
	qsimSteppingAction *fStepAct;

	G4UIdirectory *fRemollDir;
	
	G4UIcmdWithAnInteger *seedCmd;
	G4UIcmdWithAString   *fileCmd;
	//
	//G4UIcmdWithAnInteger *fStandModeCmd;
	G4UIcmdWithAnInteger *fDetModeCmd;
	G4UIcmdWithAnInteger *fQModeCmd;
	G4UIcmdWithAnInteger *fStandModeCmd;
	G4UIcmdWithAnInteger *fSourceModeCmd;
	G4UIcmdWithADouble *fQuartzPolishCmd;
	G4UIcmdWithADoubleAndUnit *fDetAngleCmd;
	// POSSCAN
	G4UIcmdWithADoubleAndUnit *fDetPosXCmd;	
	G4UIcmdWithADoubleAndUnit *fDetPosYCmd;	
	//
	G4UIcmdWithADoubleAndUnit *fXminCmd;
	G4UIcmdWithADoubleAndUnit *fXmaxCmd;
	G4UIcmdWithADoubleAndUnit *fYminCmd;
	G4UIcmdWithADoubleAndUnit *fYmaxCmd;
	G4UIcmdWithADoubleAndUnit *fEminCmd;
	G4UIcmdWithADoubleAndUnit *fEmaxCmd;

	G4UIcmdWithADoubleAndUnit *fthetaMinCmd;
	G4UIcmdWithADoubleAndUnit *fthetaMaxCmd;

//	G4UIcmdWithADoubleAndUnit *fThetaCmd;
//	G4UIcmdWithADoubleAndUnit *fPhiCmd;
	G4UIcmdWithADoubleAndUnit *fZCmd;

};

#endif//qsimMessenger_HH


