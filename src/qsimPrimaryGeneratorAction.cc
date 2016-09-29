#include "qsimPrimaryGeneratorAction.hh"


#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <TFile.h>
#include <TH2.h>
#include <TTree.h>
#include <TLeaf.h>
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

	if ( fSourceMode == 1 || ((cos(Th)*cos(Th)) > test) )
		return true;
	else
		return false;
}


//void qsimPrimaryGeneratorAction::SourceModeSet() {
//	SourceModeSet(0); // point to the one below with default settings = 0. // should I just use default parameters?
//}

// allow user modifications of private member and functional modifiable definition of primary generator variables
void qsimPrimaryGeneratorAction::SourceModeSet(G4int mode = 0) {
	fSourceMode = mode;
	// 0 is cosmic mode
	// 1 is beam mode
	// 2 is PREX mode
	if (fSourceMode==0){
		fXmin =  -5.0*cm;
		fXmax =  5.*cm;

		fYmin =  -5.*cm;
		fYmax =  5.*cm;

		fEmin = 10.0*MeV;
		fEmax = 50.0*GeV;
	
		fthetaMin = 0.0*deg;
		fthetaMax = 90.0*deg;
	}
	else if (fSourceMode==1) {
		fXmin =  0*mm;//-245.2*mm/2; // pinpoint at Mainz
		fXmax =  0*mm;//245.2*mm/2; // questionable at JLab

		fYmin =  0.0*mm;//-123.0*mm;
		fYmax =  0.0*mm;//123.0*mm;

		fEmin = 855.0*MeV; // = 250 MeV at Mainz
		fEmax = 855.0*MeV; // = 1.063 Gev for JLab
	
		fthetaMin = 0.0*deg;
		fthetaMax = 0.0*deg;
	}
	else if (fSourceMode==2){
		
		fEmin = 1.063*GeV; 
		fEmax = 1.063*GeV; 

	}
	


}

qsimPrimaryGeneratorAction::qsimPrimaryGeneratorAction() {
  G4int n_particle = 1;

	SourceModeSet(); // Accelerator beam mode, default set to 0, setting the mode to cosmic stand mode.
	
  fParticleGun = new G4ParticleGun(n_particle);
  fDefaultEvent = new qsimEvent();

	fZ = -0.52*m;
}


qsimPrimaryGeneratorAction::~qsimPrimaryGeneratorAction() {
  delete fParticleGun;
  delete fDefaultEvent;
}


bool qsimPrimaryGeneratorAction::pspectrum(double p) {
	double test = CLHEP::RandFlat::shoot(0.0,1.0) ;


	// Muon energy spctrum obtained from and fit to PDG data for 0 degree incident angle
	// good to 25% out to 36 GeV.
	// if the accelerator mode is on then just return true anyway.
	if ( fSourceMode==1 || fSourceMode == 2 || (((pow(p/GeV,-2.7)*(exp(-0.7324*(pow(log(p/GeV),2))+4.7099*log(p/GeV)-1.5)))/0.885967) > test) ) 
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

	
	double xPos, yPos, zPos;	

	if( fSourceMode == 0 || fSourceMode == 1) {
	    xPos = CLHEP::RandFlat::shoot( fXmin, fXmax );
	    yPos = CLHEP::RandFlat::shoot( fYmin, fYmax );
	}
	
	zPos = fZ;


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
	
	
	double p = sqrt( E*E - mass*mass );
	double pX, pY, pZ;
	double randTheta, randPhi;
	double tanth, tanph;
	
	if (fSourceMode == 0 || fSourceMode == 1) {
		bool goodTheta = false;
		while ( goodTheta == false ) {
			randTheta = CLHEP::RandFlat::shoot( fthetaMin, fthetaMax );
			goodTheta = Thetaspectrum(randTheta);
		}
		
		randPhi = CLHEP::RandFlat::shoot( 0.0,360.0)*deg ;
    
    		pX = sin(randTheta)*cos(randPhi)*p;
   		pY = sin(randTheta)*sin(randPhi)*p;
    		pZ = cos(randTheta)*p;
	}

	if (fSourceMode == 2) {
		int chosenEvent;
		TFile *primaryFile = new TFile("primaryDistribution.root");
		TTree *T = (TTree*)primaryFile->Get("T");
		chosenEvent = rand() % T->GetEntries();
		T->GetEntry(chosenEvent);		
		xPos = T->GetLeaf("x")->GetValue()*m;
		yPos = T->GetLeaf("y")->GetValue()*m;
		tanth = T->GetLeaf("theta")->GetValue();		
		tanph = T->GetLeaf("phi")->GetValue();		
		primaryFile->Close();
		pZ = sqrt(p/(1. + tanth*tanth + tanph*tanph));
		pX = pZ*tanth;
		pY = pZ*tanph;	
			
	}

    
    assert( E > 0.0 );
    assert( E > mass );

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

