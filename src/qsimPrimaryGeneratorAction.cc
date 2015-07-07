#include "qsimPrimaryGeneratorAction.hh"


#include <stdlib.h>
#include <stdio.h>
#include <time.h>


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


bool qsimPrimaryGeneratorAction::Thetaspectrum(double Th) {
	double test = CLHEP::RandFlat::shoot(0.0,1.0);

	if ( (cos(Th)*cos(Th)) > test )
		return true;
	else
		return false;
}

qsimPrimaryGeneratorAction::qsimPrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fDefaultEvent = new qsimEvent();

  fXmin =  -15.0*cm;
  fXmax =  15.*cm;

  fYmin =  -15.*cm;
  fYmax =  15.*cm;

  fEmin = 10.0*MeV;
  fEmax = 50.0*GeV;

	fthetaMin = 0.0*deg;
	fthetaMax = 90.0*deg;

//  fEmin = 1000.0*MeV;
//  fEmax = 1000.0*MeV;

// This needs to be well defined  
	fZ = -0.6*m;
}


qsimPrimaryGeneratorAction::~qsimPrimaryGeneratorAction() {
  delete fParticleGun;
  delete fDefaultEvent;
}


bool qsimPrimaryGeneratorAction::pspectrum(double p) {
	double test = CLHEP::RandFlat::shoot(0.0,1.0) ;


	// Muon energy spctrum obtained from and fit to PDG data for 0 degree incident angle
	// good to 25% out to 36 GeV
	if ( pow(p/GeV,-2.7)*(exp(-0.7324*(log(pow(p/GeV,2))+4.7099*log(p/GeV)-1.5))/0.885967) > test ) 
		return true;
	else 
		return false;
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

// begin changed stuff to generate probability distribution of energies as expected
		bool good_p = false;
		double p3sq, E;
		double mass = fParticleGun->GetParticleDefinition()->GetPDGMass();

		while ( good_p == false ) {
			E = CLHEP::RandFlat::shoot( fEmin, fEmax );
                        p3sq = E*E - mass*mass;
                        if( p3sq < 0 ) continue;

			good_p = pspectrum(sqrt(p3sq));
		}


	// fTheta needs to be a random distribution determined by the cos^2(theta) distribution	
	bool goodTheta = false;
	double randTheta;

	while ( goodTheta == false ) {
		randTheta = CLHEP::RandFlat::shoot( fthetaMin, fthetaMax )*deg;
		goodTheta = Thetaspectrum(randTheta);
	}
	
// end need
  srand(rand());
  double randPhi = CLHEP::RandFlat::shoot( 0.0,360.0)*deg ;
	
		
// end    
		

    
    assert( E > 0.0 );
    assert( E > mass );


    double p = sqrt( E*E - mass*mass );

    double pX = sin(randTheta)*cos(randPhi)*p;
    double pY = sin(randTheta)*sin(randPhi)*p;
    double pZ = cos(randTheta)*p;


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

