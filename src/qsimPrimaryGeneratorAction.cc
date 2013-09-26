#include "qsimPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "qsimIO.hh"
#include "qsimEvent.hh"
#include "qsimtypes.hh"
#include "globals.hh"


qsimPrimaryGeneratorAction::qsimPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);


  fDefaultEvent = new qsimEvent();
}

qsimPrimaryGeneratorAction::~qsimPrimaryGeneratorAction() {
  delete fParticleGun;
  delete fDefaultEvent;
}


void qsimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    /*  Generate event, set IO data */

    // Use default, static single generator
    // Update this just in case things changed
    // from the command user interface
    fDefaultEvent->Reset();

    // Set data //////////////////////////////////
    // Magic happens here

    fDefaultEvent->ProduceNewParticle(
	    G4ThreeVector(0.*cm,0.*cm,-100.*cm),
	    G4ThreeVector(0.0,0.0, gDefaultBeamE),
	    "e-" );

    /////////////////////////////////////////////////////////////
    // Register and create event
    //
    
    double kinE = sqrt(fDefaultEvent->fPartMom[0].mag()*fDefaultEvent->fPartMom[0].mag()
	    + fDefaultEvent->fPartType[0]->GetPDGMass()*fDefaultEvent->fPartType[0]->GetPDGMass() )
	-  fDefaultEvent->fPartType[0]->GetPDGMass();

    fParticleGun->SetParticleDefinition(fDefaultEvent->fPartType[0]);
    fParticleGun->SetParticleMomentumDirection(fDefaultEvent->fPartMom[0].unit());
    fParticleGun->SetParticleEnergy( kinE  );
    fParticleGun->SetParticlePosition( fDefaultEvent->fPartPos[0] );


    fIO->SetEventData(fDefaultEvent);
    fParticleGun->GeneratePrimaryVertex(anEvent);

}

G4ParticleGun* qsimPrimaryGeneratorAction::GetParticleGun() {
  return fParticleGun;
} 

