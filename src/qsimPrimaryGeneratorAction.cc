
#include "qsimPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

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

//
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TH1.h>
#include <TRandom.h>
//

#include "G4ParticleTable.hh"

//
static std::ifstream myfile;
//

qsimPrimaryGeneratorAction::qsimPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle
      = particleTable->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);

  fDefaultEvent = new qsimEvent();

  fXmin =  -0.0*cm;
  fXmax =   0.0*cm;

  fYmin =  -0.0*cm;
  fYmax =   0.0*cm;

  fZmin =  -65.0*cm;
  fZmax =  -65.0*cm;

  fEmin = 220*MeV;
  fEmax = 100000*MeV;

  fThetaMin = 0.0*deg;
  fThetaMax = 0.0*deg;
  fPhiMin = 0*deg;
  fPhiMax = 0*deg;
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


    double xPos = CLHEP::RandFlat::shoot( fXmin, fXmax );
    double yPos = CLHEP::RandFlat::shoot( fYmin, fYmax );
    double zPos = CLHEP::RandFlat::shoot( fZmin, fZmax );
    double theta = CLHEP::RandFlat::shoot( fThetaMin, fThetaMax );
    //double xPos = -55.5*cm*sin(theta);
    //double zPos = -55.5*cm*cos(theta);
    double phi = CLHEP::RandFlat::shoot( fPhiMin, fPhiMax );
    double mass = fParticleGun->GetParticleDefinition()->GetPDGMass();
    //double E = CLHEP::RandFlat::shoot( fEmin, fEmax );
    double E = 855*MeV;
   
    
    assert( E > 0.0 );
    assert( E > mass );

    double p = sqrt( E*E - mass*mass );

    double pX = sin(theta)*cos(phi)*p;
    double pY = sin(theta)*sin(phi)*p;
    double pZ = cos(theta)*p;

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

