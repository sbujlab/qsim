#include "qsimIO.hh"

#include "G4SystemOfUnits.hh"

#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>

#include "G4ParticleDefinition.hh"

#include "qsimDetectorHit.hh"
#include "qsimMonitorDetectorHit.hh"
#include "qsimScintDetectorHit.hh"
#include "qsimEvent.hh"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>


qsimIO::qsimIO(){
    fTree = NULL;
    InitializeTree();
    // Default filename
    strcpy(fFilename, "qsimout.root");

    fFile = NULL;
}

qsimIO::~qsimIO(){
    if( fTree ){ delete fTree; }
    fTree = NULL;
    if( fFile ){ delete fFile; }
    fFile = NULL;
}

void qsimIO::SetFilename(G4String fn){
    G4cout << "Setting output file to " << fn << G4endl;
    strcpy(fFilename, fn.data());
}

void qsimIO::InitializeTree(){
    if( fFile ){
	fFile->Close();
	delete fFile;
    }

    fFile = new TFile(fFilename, "RECREATE");

    if( fTree ){ delete fTree; }

    fTree = new TTree("T", "Geant4 Quartz Detector Simulation");

    // Event information
    fTree->Branch("ev.pid",   &fEvPart_PID, "ev.pid/I");
    fTree->Branch("ev.vx",    &fEvPart_X,   "ev.vx/D");
    fTree->Branch("ev.vy",    &fEvPart_Y,   "ev.vy/D");
    fTree->Branch("ev.vz",    &fEvPart_Z,   "ev.vz/D");
    fTree->Branch("ev.p",     &fEvPart_P,   "ev.p/D");
    fTree->Branch("ev.px",    &fEvPart_Px,  "ev.px/D");
    fTree->Branch("ev.py",    &fEvPart_Py,  "ev.py/D");
    fTree->Branch("ev.pz",    &fEvPart_Pz,  "ev.pz/D");


    // Monitor hit
    fTree->Branch("mon.n",	&fNMonDetHit,	    "mon.n/I");
    fTree->Branch("mon.pid",	fMonDetHit_pid,     "mon.pid[mon.n]/I");
    fTree->Branch("mon.trid",	fMonDetHit_TrId,    "mon.trid[mon.n]/I");

    fTree->Branch("mon.x",	fMonDetHit_X,	    "mon.x[mon.n]/D");
    fTree->Branch("mon.y",    	fMonDetHit_Y,       "mon.y[mon.n]/D");
    fTree->Branch("mon.z",    	fMonDetHit_Z,       "mon.z[mon.n]/D");

    fTree->Branch("mon.px",   	fMonDetHit_Px,	    "mon.px[mon.n]/D");
    fTree->Branch("mon.py",   	fMonDetHit_Py,	    "mon.py[mon.n]/D");
    fTree->Branch("mon.pz",   	fMonDetHit_Pz,	    "mon.pz[mon.n]/D");

    fTree->Branch("mon.vx",	fMonDetHit_Vx,	    "mon.vx[mon.n]/D");
    fTree->Branch("mon.vy",   	fMonDetHit_Vy,      "mon.vy[mon.n]/D");
    fTree->Branch("mon.vz",   	fMonDetHit_Vz,      "mon.vz[mon.n]/D");

//    fTree->Branch("mon.vdx",	fMonDetHit_Vdx,     "mon.vdx[mon.n]/D");
//    fTree->Branch("mon.vdy",   	fMonDetHit_Vdy,     "mon.vdy[mon.n]/D");
//    fTree->Branch("mon.vdz",   	fMonDetHit_Vdz,     "mon.vdz[mon.n]/D");

    fTree->Branch("mon.p",	fMonDetHit_P,	    "mon.p[mon.n]/D");
    fTree->Branch("mon.e",	fMonDetHit_E,	    "mon.e[mon.n]/D");
    fTree->Branch("mon.m",	fMonDetHit_M,	    "mon.m[mon.n]/D");

    // Layer hit
    fTree->Branch("layer.n",	&fNLayerDetHit,	    "layer.n/I");
    fTree->Branch("layer.pid",	fLayerDetHit_pid,   "layer.pid[layer.n]/I");
    fTree->Branch("layer.trid",	fLayerDetHit_TrId,  "layer.trid[layer.n]/I");

    fTree->Branch("layer.x",	fLayerDetHit_X,	    "layer.x[layer.n]/D");
    fTree->Branch("layer.y",    fLayerDetHit_Y,     "layer.y[layer.n]/D");
    fTree->Branch("layer.z",    fLayerDetHit_Z,     "layer.z[layer.n]/D");

    fTree->Branch("layer.px",   fLayerDetHit_Px,    "layer.px[layer.n]/D");
    fTree->Branch("layer.py",   fLayerDetHit_Py,    "layer.py[layer.n]/D");
    fTree->Branch("layer.pz",   fLayerDetHit_Pz,    "layer.pz[layer.n]/D");

    fTree->Branch("layer.vx",	fLayerDetHit_Vx,    "layer.vx[layer.n]/D");
    fTree->Branch("layer.vy",   fLayerDetHit_Vy,    "layer.vy[layer.n]/D");
    fTree->Branch("layer.vz",   fLayerDetHit_Vz,    "layer.vz[layer.n]/D");

    fTree->Branch("layer.p",	fLayerDetHit_P,	    "layer.p[layer.n]/D");
    fTree->Branch("layer.e",	fLayerDetHit_E,	    "layer.e[layer.n]/D");
    fTree->Branch("layer.m",	fLayerDetHit_M,	    "layer.m[layer.n]/D");
    return;
}

void qsimIO::FillTree(){
    if( !fTree ){ 
	fprintf(stderr, "Error %s: %s line %d - Trying to fill non-existant tree\n", __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	return; 
    }

    fTree->Fill();
}

void qsimIO::Flush(){
    //  Set arrays to 0
    fNMonDetHit = 0;
    fNLayerDetHit = 0;

    // reset vertex
    fEvPart_X  = 9999.e9;
    fEvPart_Y  = 9999.e9;
    fEvPart_Z  = 9999.e9;
    fEvPart_P  = 9999.e9;
    fEvPart_Px = 9999.e9;
    fEvPart_Py = 9999.e9;
    fEvPart_Pz = 9999.e9;
    fEvPart_PID= 999999999;
}

void qsimIO::WriteTree(){
    assert( fFile );
    assert( fTree );

    if( !fFile->IsOpen() ){
	G4cerr << "ERROR: " << __FILE__ << " line " << __LINE__ << ": TFile not open" << G4endl;
	exit(1);
    }

    G4cout << "Writing output to " << fFile->GetName() << "... ";

    fFile->cd();

    fTree->Write("T", TObject::kOverwrite);

    fTree->ResetBranchAddresses();
    delete fTree;
    fTree = NULL;

    fFile->Close();

    delete fFile;
    fFile = NULL;

    G4cout << "written" << G4endl;

    return;
}

///////////////////////////////////////////////////////////////////////////////
// Interfaces to output section ///////////////////////////////////////////////

// Event Data

void qsimIO::SetEventData(qsimEvent *ev){
    fEvPart_PID = ev->fPartType[0]->GetPDGEncoding();

    fEvPart_X = ev->fPartPos[0].x()/__L_UNIT;
    fEvPart_Y = ev->fPartPos[0].y()/__L_UNIT;
    fEvPart_Z = ev->fPartPos[0].z()/__L_UNIT;

    fEvPart_Px = ev->fPartMom[0].x()/__E_UNIT;
    fEvPart_Py = ev->fPartMom[0].y()/__E_UNIT;
    fEvPart_Pz = ev->fPartMom[0].z()/__E_UNIT;

    fEvPart_P = ev->fPartMom[0].mag()/__E_UNIT;

    return;
}

// Monitor Detector Hit
void qsimIO::AddMonitorDetectorHit(qsimMonitorDetectorHit *hit){
    int n = fNMonDetHit;
    if ( n >= __IO_MAXHIT){
	return;
    }

    fMonDetHit_pid[n]	= hit->fPID;
    fMonDetHit_TrId[n]	= hit->fTrID;

    fMonDetHit_X[n]	= hit->f3X.x()/__L_UNIT;
    fMonDetHit_Y[n]  	= hit->f3X.y()/__L_UNIT;
    fMonDetHit_Z[n]  	= hit->f3X.z()/__L_UNIT;

    fMonDetHit_Vx[n]	= hit->f3V.x()/__L_UNIT;
    fMonDetHit_Vy[n]  	= hit->f3V.y()/__L_UNIT;
    fMonDetHit_Vz[n]  	= hit->f3V.z()/__L_UNIT;

//    fMonDetHit_Vdx[n]	= hit->f3D.x();
//    fMonDetHit_Vdy[n]  	= hit->f3D.y();
//    fMonDetHit_Vdz[n]  	= hit->f3D.z();

    fMonDetHit_Px[n]	= hit->f3P.x()/__E_UNIT;
    fMonDetHit_Py[n]	= hit->f3P.y()/__E_UNIT;
    fMonDetHit_Pz[n]	= hit->f3P.z()/__E_UNIT;

    fMonDetHit_P[n]	= hit->fP/__E_UNIT;
    fMonDetHit_E[n]	= hit->fE/__E_UNIT;
    fMonDetHit_M[n]	= hit->fM/__E_UNIT;

    fNMonDetHit++;

    return;
}

// Layer Detector Hit
void qsimIO::AddLayerDetectorHit(qsimMonitorDetectorHit *hit){
    int n = fNLayerDetHit;
    if ( n >= __IO_MAXHIT){
	return;
    }

    fLayerDetHit_pid[n]	= hit->fPID;
    fLayerDetHit_TrId[n]= hit->fTrID;

    fLayerDetHit_X[n]	= hit->f3X.x()/__L_UNIT;
    fLayerDetHit_Y[n]  	= hit->f3X.y()/__L_UNIT;
    fLayerDetHit_Z[n]  	= hit->f3X.z()/__L_UNIT;

    fLayerDetHit_Vx[n]	= hit->f3V.x()/__L_UNIT;
    fLayerDetHit_Vy[n]  = hit->f3V.y()/__L_UNIT;
    fLayerDetHit_Vz[n]  = hit->f3V.z()/__L_UNIT;

    fLayerDetHit_Px[n]	= hit->f3P.x()/__E_UNIT;
    fLayerDetHit_Py[n]	= hit->f3P.y()/__E_UNIT;
    fLayerDetHit_Pz[n]	= hit->f3P.z()/__E_UNIT;

    fLayerDetHit_P[n]	= hit->fP/__E_UNIT;
    fLayerDetHit_E[n]	= hit->fE/__E_UNIT;
    fLayerDetHit_M[n]	= hit->fM/__E_UNIT;

    fNLayerDetHit++;

    return;
}
