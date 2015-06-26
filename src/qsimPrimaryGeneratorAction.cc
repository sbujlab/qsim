#include "qsimPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "qsimIO.hh"
#include "qsimEvent.hh"
#include "qsimtypes.hh"
#include "globals.hh"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"

#include "G4SystemOfUnits.hh"

qsimPrimaryGeneratorAction::qsimPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);


  fDefaultEvent = new qsimEvent();

  fXmin =  -15.0*cm;
  fXmax =  -2.75*cm;

  fYmin =  -1.5*cm;
  fYmax =   1.5*cm;

  fEmin = 1000.0*MeV;
  fEmax = 1000.0*MeV;

  fZ = -10.6*cm;

  fTheta = 45.0*deg;
  fPhi   = 0.0*deg;
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

    double xPos = CLHEP::RandFlat::shoot( fXmin, fXmax );
    double yPos = CLHEP::RandFlat::shoot( fYmin, fYmax );
    double zPos = fZ;
    double E = CLHEP::RandFlat::shoot( fEmin, fEmax );

    double mass = fParticleGun->GetParticleDefinition()->GetPDGMass();
    
    assert( E > 0.0 );
    assert( E > mass );

    double p = sqrt( E*E - mass*mass );

    double pX = sin(fTheta)*cos(fPhi)*p;
    double pY = sin(fTheta)*sin(fPhi)*p;
    double pZ = cos(fTheta)*p;

    fDefaultEvent->ProduceNewParticle(
	    G4ThreeVector(xPos, yPos, zPos),
	    G4ThreeVector(pX, pY, pZ ),
	    fParticleGun->GetParticleDefinition()->GetParticleName() );

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

