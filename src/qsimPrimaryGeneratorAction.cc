#include "qsimPrimaryGeneratorAction.hh"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <sstream>

#include <TFile.h>
#include <TMath.h>
#include <TH1.h>
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
TH1F *elasticDist = new TH1F("el","elastic dist",200,0.6,1.2);
TH1F *inelDist = new TH1F("inel","inelastic dist",200,0.6,1.2);


//void qsimPrimaryGeneratorAction::SourceModeSet() {
//	SourceModeSet(0); // point to the one below with default settings = 0. // should I just use default parameters?
//}

// allow user modifications of private member and functional modifiable definition of primary generator variables
void qsimPrimaryGeneratorAction::SourceModeSet(G4int mode = 3) {
	fSourceMode = mode;
	// 0 is cosmic mode
	// 1 is beam mode
	// 2 is PREX mode
    // 3 is remoll distribution mode
	if (fSourceMode==0){
		fXmin =  /*-5.0*/-0.0*cm;
		fXmax =  /*5.*/0.0*cm;

		fYmin =  /*-5.*/-0.0*cm;
		fYmax =  /*5.*/0.0*cm;

		fEmin = 10.0*MeV;
		fEmax = 50.0*GeV;
	
		fthetaMin = 0.0*deg;
		fthetaMax = 90.0*deg;

		fPhiMin = 0.0*deg;
		fPhiMax = 360.0*deg;
	}
	else if (fSourceMode==1) {
		fXmin =  -450*mm; // pinpoint at Mainz
		fXmax =  -950*mm; // questionable at JLab

		fYmin =  -200*mm;
		fYmax =  200*mm;

		fEmin = 3*GeV;//855.0*MeV; // = 250 MeV at Mainz
		fEmax = 8*GeV;//855.0*MeV; // = 1.063 Gev for JLab
	
		fthetaMin = 0.0*deg;
		fthetaMax = 5*deg;//0.0*deg;

		fPhiMin = 0.0;
		fPhiMax = 360.0;//0.0*deg;
  }
	else if (fSourceMode==2){
		
		fEmin = 1.063*GeV; 
		fEmax = 1.063*GeV; 

		fPhiMin = 0.0*deg;
		fPhiMax = 0.0*deg;

	}
    else if (fSourceMode==3){
        fEmin = 2.0*GeV;
        fEmax = 11.0*GeV;

        fthetaMin = -3.0*deg;//FIXME Needs justification, what about the angle about the z axis that this points as well, is that phi, or is phi the start position about z?
        fthetaMax = 3.0*deg;

    		fPhiMin = 0.0*deg;
	    	fPhiMax = 360.0*deg;

        fRing = 5;
        fSector = 0;
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
	if ( fSourceMode != 0 || (((pow(p/GeV,-2.7)*(exp(-0.7324*(pow(log(p/GeV),2))+4.7099*log(p/GeV)-1.5)))/0.885967) > test) ) 
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
	
	if (fSourceMode == 0 || fSourceMode == 1 || fSourceMode == 3) {
		bool goodTheta = false;
		while ( goodTheta == false ) {
			randTheta = CLHEP::RandFlat::shoot( fthetaMin, fthetaMax );
			goodTheta = Thetaspectrum(randTheta);
		}
		randPhi = CLHEP::RandFlat::shoot( fPhiMin,fPhiMax)*deg;
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

    if (fSourceMode == 3){
        //get radial distribution from remoll, radius is along x-axis
        double rad = RadSpectrum();
        zPos = -10*cm;
        yPos = (rad - (zPos*sin(randTheta)))*sin(randPhi);
        xPos = (rad - (zPos*sin(randTheta)))*cos(randPhi);
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


bool qsimPrimaryGeneratorAction::Thetaspectrum(double Th) {
	double test = CLHEP::RandFlat::shoot(0.0,1.0);

	if ( fSourceMode != 0 || ((cos(Th)*cos(Th)) > test) )
		return true;
	else
		return false;
}
//Generating radius from theta
double qsimPrimaryGeneratorAction::RadSpectrum(){   //Only used for fSourceMode 3
    //determine whether to use moller, elastic, or inelastic distribution
    double radius;
    double test = CLHEP::RandFlat::shoot(0.0,1.0);
    if (test < 24163.0/1265326.0){//inelastic
	double gArea[] = {2.60384e6,36188.3,8.65955e5,1.64544e6};
        double eArea[] = {2.76828e6,264188,7.15987e5,1.85492e6};
        double tH[] = {30e6,5e6,12e6,20e6};
        int tN[] = {2.81725e6,7.2377e4,9.07299e5,1.84075e6};
        double mu[] = {7.73e-1,7.5e-1,7.7e-1,7.73e-1};
        double sig[] = {1.88253e-2,1.12121e-2,1.77574e-2,2.16431e-2};
        double n[] = {5e6,3e5,1e6,3.5e6};
        double exp[] = {6.5,7.0,9.0,6.0};
        double v[] = {0.75,0.3,0.75,0.75};

    
        double div = (fSector == 1)? 0.75 :(fSector == 2)? 0.79 : 0.8;
            double u =  CLHEP::RandFlat::shoot(0.0,1.0);
            bool filled = false;

            if (u < gArea[fSector]/(gArea[fSector] + eArea[fSector])){
                while (!filled){
                    double u1 = 2* CLHEP::RandFlat::shoot(0.0,1.0)-1;
                    double u2 = 2* CLHEP::RandFlat::shoot(0.0,1.0)-1;
            
                    double r = u1*u1 + u2*u2;

                    if (r <= 1 && r > 1e-6){

                        double z1 = u1*sqrt(-2*log(r)/r);
                        radius = mu[fSector]+sig[fSector]*z1; 
            
                        if (radius < div){
                            inelDist->Fill(radius);
                            filled = true;
                        }
                    }
                }
            }
            else{
                do{
                    radius = CLHEP::RandFlat::shoot(div,1.2);
                    double u2 = CLHEP::RandFlat::shoot(0.0,1.0);

                    if ( tH[fSector]*u2 < n[fSector]*TMath::Power(radius,(-1*exp[fSector]))+v[fSector]){
                        inelDist->Fill(radius);
                        filled = true;
                    }
                }while (!filled);
            }

    }
    else if (test < 61237.0/1265326.0){//elastic
	double gArea[] = {7.21689e7,782945,1.3112e7,5.9954e7};
        double qArea[] = {1.70795e8,9.09159e6,5.50713e7,1.25122e8};
        double n[] = {800e6,60e6,550e6,600e6};
        double tN[] = {8.08859e7,8.19378e5,1.75234e7,6.79431e7};
        double tMu[] = {0.748941,0.746941,0.759441,0.749441};
        double tSig[] = {1.70039e-2,1.35555e-2,1.5783e-2,1.7318e-2};
        double tQ[] = {9.77635e9,3.3381e8,5.65955e9,7.1164e9};
        double tY[] = {2.45583e8,7.98e6,5.62236e7,1.80618e8};
        double tV[] = {9.75e-1,0.83,0.92,0.975};
        
        bool filled = false;
        
        double u1,u2;
        double u =  CLHEP::RandFlat::shoot(0.0,1.0);

        if (u >= gArea[fSector]/(gArea[fSector]+qArea[fSector])){
            do{
            u1 =  CLHEP::RandFlat::shoot(0.77,1.2);
            u2 =  CLHEP::RandFlat::shoot(0.0,1.0);

            if (u1 > 0.77 && u2*n[fSector] < tQ[fSector]*(u1-tV[fSector])*(u1-tV[fSector]) + tY[fSector]){
                radius = u1;
                elasticDist->Fill(radius);
                filled = true;
            }
            }while (!filled);
        }
        else{
            do{
            u1 = 2.0* CLHEP::RandFlat::shoot(0.0,1.0) - 1;
            u2 = 2.0* CLHEP::RandFlat::shoot(0.0,1.0) - 1;
            double r2 = (u1)*(u1) + (u2)*(u2);
            if (r2 <= 1){   
            radius = (tMu[fSector] + tSig[fSector]*(u1*sqrt(-2*log(r2)/r2)));

            if (elasticDist->GetBinContent(elasticDist->GetXaxis()->FindBin(tMu[fSector])) < tN[fSector]){
            if (radius < 0.77){
                elasticDist->Fill(radius);
                filled = true;
            }
            }
            }
        }while (!filled);
        }
    }
    else{//moller
	bool filled = false;

        double n[4],mu[4],sig[4],r,u1,u2;
        n = {3.28542e8,4.98888e7,1.43470e8,1.70085e8};
        mu = {9.97921e-1,1.01235,1.00520,0.984920};
        sig = {0.04,3.07008e-2,3.187e-2,0.04};
        do{
            u1 = 2.0*CLHEP::RandFlat::shoot(0.0,1.0) - 1;
            u2 = 2.0*CLHEP::RandFlat::shoot(0.0,1.0) - 1;
            r = u1*u1 + u2*u2;
            if (r <= 1) {
                radius = mu[fSector] + sig[fSector]*(u1*sqrt(-2*log(r)/r));
                filled = true;
            }
       }while (!filled);
    }
    double LGstart[] = {0.730,0.730,0.855,0.930,1.04,1.2};
    double finRad = (radius /*- LGstart[fRing] - 0.075*/)*1000.0; //0.075*m is the offset for the start of the light guide, since the center of the quartz is at (0,0,0).
    //if (-1.0*finRad > 75) std::cout <<"finRad" << std::endl; else std::cout <<"recur" << std::endl;
    //std::cout << finRad << std::endl << -1.0*finRad << std::endl;
    return  finRad;
}


