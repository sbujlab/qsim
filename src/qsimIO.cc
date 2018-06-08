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


    // DetectorHit
    fTree->Branch("hit.n",    &fNDetHit,     "hit.n/I");
    fTree->Branch("hit.det",  &fDetHit_det,  "hit.det[hit.n]/I");
    fTree->Branch("hit.vid",  &fDetHit_id,   "hit.vid[hit.n]/I");

    fTree->Branch("hit.pid",  &fDetHit_pid,   "hit.pid[hit.n]/I");
    fTree->Branch("hit.trid", &fDetHit_trid,  "hit.trid[hit.n]/I");
    fTree->Branch("hit.mtrid",&fDetHit_mtrid, "hit.mtrid[hit.n]/I");
    fTree->Branch("hit.gen",  &fDetHit_gen,   "hit.gen[hit.n]/I");

    fTree->Branch("hit.x",    &fDetHit_X,   "hit.x[hit.n]/D");
    fTree->Branch("hit.y",    &fDetHit_Y,   "hit.y[hit.n]/D");
    fTree->Branch("hit.z",    &fDetHit_Z,   "hit.z[hit.n]/D");

    fTree->Branch("hit.px",   &fDetHit_Px,   "hit.px[hit.n]/D");
    fTree->Branch("hit.py",   &fDetHit_Py,   "hit.py[hit.n]/D");
    fTree->Branch("hit.pz",   &fDetHit_Pz,   "hit.pz[hit.n]/D");

    fTree->Branch("hit.vx",   &fDetHit_Vx,   "hit.vx[hit.n]/D");
    fTree->Branch("hit.vy",   &fDetHit_Vy,   "hit.vy[hit.n]/D");
    fTree->Branch("hit.vz",   &fDetHit_Vz,   "hit.vz[hit.n]/D");

    fTree->Branch("hit.vdx",   &fDetHit_Vdx,   "hit.vdx[hit.n]/D");
    fTree->Branch("hit.vdy",   &fDetHit_Vdy,   "hit.vdy[hit.n]/D");
    fTree->Branch("hit.vdz",   &fDetHit_Vdz,   "hit.vdz[hit.n]/D");

    fTree->Branch("hit.p",    &fDetHit_P,   "hit.p[hit.n]/D");
    fTree->Branch("hit.e",    &fDetHit_E,   "hit.e[hit.n]/D");
    fTree->Branch("hit.m",    &fDetHit_M,   "hit.m[hit.n]/D");

    // Monitor hit
    fTree->Branch("mon.n",	&fNMonDetHit,	    "mon.n/I");
    fTree->Branch("mon.pid",	&fMonDetHit_pid,    "mon.pid/I");

    fTree->Branch("mon.x",	&fMonDetHit_X,	    "mon.x/D");
    fTree->Branch("mon.y",    	&fMonDetHit_Y,      "mon.y/D");
    fTree->Branch("mon.z",    	&fMonDetHit_Z,      "mon.z/D");

    fTree->Branch("mon.px",   	&fMonDetHit_Px,	    "mon.px/D");
    fTree->Branch("mon.py",   	&fMonDetHit_Py,	    "mon.py/D");
    fTree->Branch("mon.pz",   	&fMonDetHit_Pz,	    "mon.pz/D");

    fTree->Branch("mon.vx",	&fMonDetHit_Vx,	    "mon.vx/D");
    fTree->Branch("mon.vy",   	&fMonDetHit_Vy,     "mon.vy/D");
    fTree->Branch("mon.vz",   	&fMonDetHit_Vz,     "mon.vz/D");

    fTree->Branch("mon.vdx",	&fMonDetHit_Vdx,       "mon.vdx/D");
    fTree->Branch("mon.vdy",   	&fMonDetHit_Vdy,       "mon.vdy/D");
    fTree->Branch("mon.vdz",   	&fMonDetHit_Vdz,       "mon.vdz/D");

    fTree->Branch("mon.p",	&fMonDetHit_P,	    "mon.p/D");
    fTree->Branch("mon.e",	&fMonDetHit_E,	    "mon.e/D");
    fTree->Branch("mon.m",	&fMonDetHit_M,	    "mon.m/D");

    // ScintDetectorHit
    fTree->Branch("sci.n",    &fNScintDetHit,     "sci.n/I");
    fTree->Branch("sci.det",  &fScintDetHit_det,  "sci.det[sci.n]/I");
    fTree->Branch("sci.id",   &fScintDetHit_id,   "sci.id[sci.n]/I");
    fTree->Branch("sci.edep", &fScintDetHit_edep, "sci.id[sci.n]/D");

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
    fNDetHit = 0;
    fNMonDetHit = 0;
    fNScintDetHit = 0;

    // reset vertex
    fEvPart_X  = 9999.e9;
    fEvPart_Y  = 9999.e9;
    fEvPart_Z  = 9999.e9;
    fEvPart_P  = 9999.e9;
    fEvPart_Px = 9999.e9;
    fEvPart_Py = 9999.e9;
    fEvPart_Pz = 9999.e9;
    fEvPart_PID= 999999999;

    // reset detector parameters
    for (int i=0; i<10; ++i){
	fDetHit_det[i] = 999999999;
	fDetHit_id[i]   = 999999999;

	fDetHit_trid[i] = 999999999;
	fDetHit_mtrid[i]= 999999999;
	fDetHit_pid[i]  = 999999999;
	fDetHit_gen[i]  = 999999999;

	fDetHit_X[i]  = 9999.e9;
	fDetHit_Y[i]  = 9999.e9;
	fDetHit_Z[i]  = 9999.e9;

	fDetHit_Px[i]  = 9999.e9;
	fDetHit_Py[i]  = 9999.e9;
	fDetHit_Pz[i]  = 9999.e9;

	fDetHit_Vx[i]  = 9999.e9;
	fDetHit_Vy[i]  = 9999.e9;
	fDetHit_Vz[i]  = 9999.e9;

	fDetHit_Vdx[i]  = 9999.e9;
	fDetHit_Vdy[i]  = 9999.e9;
	fDetHit_Vdz[i]  = 9999.e9;

	fDetHit_P[i]  = 9999.e9;
	fDetHit_E[i]  = 9999.e9;
	fDetHit_M[i]  = 9999.e9;
    }

    // reset scintillator detector parameters
    for (int i=0; i<10; ++i){
	fScintDetHit_det[i] = 999999999;
	fScintDetHit_edep[i] = 9999.e9;
	fScintDetHit_id[i] = 999999999;
    }

    // reset monitor detector parameters
    for (int i=0; i<10; ++i){
	fMonDetHit_pid[i]  = 999999999;

	fMonDetHit_X[i]  = 9999.e9;
	fMonDetHit_Y[i]  = 9999.e9;
	fMonDetHit_Z[i]  = 9999.e9;

	fMonDetHit_Px[i]  = 9999.e9;
	fMonDetHit_Py[i]  = 9999.e9;
	fMonDetHit_Pz[i]  = 9999.e9;

	fMonDetHit_Vx[i]  = 9999.e9;
	fMonDetHit_Vy[i]  = 9999.e9;
	fMonDetHit_Vz[i]  = 9999.e9;

	fMonDetHit_Vdx[i]  = 9999.e9;
	fMonDetHit_Vdy[i]  = 9999.e9;
	fMonDetHit_Vdz[i]  = 9999.e9;

	fMonDetHit_P[i]  = 9999.e9;
	fMonDetHit_E[i]  = 9999.e9;
	fMonDetHit_M[i]  = 9999.e9;
    }
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

// DetectorHit

void qsimIO::AddDetectorHit(qsimDetectorHit *hit){
    int n = fNDetHit;
    //printf("%d hits in detector\n", fNDetHit );

    if( n >= __IO_MAXHIT ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    fDetHit_det[n]  = hit->fDetID;
    fDetHit_id[n]   = hit->fCopyID;

    fDetHit_trid[n] = hit->fTrID;
    fDetHit_mtrid[n]= hit->fmTrID;
    fDetHit_pid[n]  = hit->fPID;
    fDetHit_gen[n]  = hit->fGen;

    fDetHit_X[n]  = hit->f3X.x()/__L_UNIT;
    fDetHit_Y[n]  = hit->f3X.y()/__L_UNIT;
    fDetHit_Z[n]  = hit->f3X.z()/__L_UNIT;

    fDetHit_Px[n]  = hit->f3P.x()/__E_UNIT;
    fDetHit_Py[n]  = hit->f3P.y()/__E_UNIT;
    fDetHit_Pz[n]  = hit->f3P.z()/__E_UNIT;

    fDetHit_Vx[n]  = hit->f3V.x()/__L_UNIT;
    fDetHit_Vy[n]  = hit->f3V.y()/__L_UNIT;
    fDetHit_Vz[n]  = hit->f3V.z()/__L_UNIT;

    fDetHit_Vdx[n]  = hit->f3D.x();
    fDetHit_Vdy[n]  = hit->f3D.y();
    fDetHit_Vdz[n]  = hit->f3D.z();

    fDetHit_P[n]  = hit->fP/__E_UNIT;
    fDetHit_E[n]  = hit->fE/__E_UNIT;
    fDetHit_M[n]  = hit->fM/__E_UNIT;

    fNDetHit++;

    return;
}

// Monitor Detector Hit
void qsimIO::AddMonitorDetectorHit(qsimMonitorDetectorHit *hit){
    int n = fNMonDetHit;
    if ( n >= __IO_MAXHIT){
	return;
    }

    fMonDetHit_pid[n]	= hit->fPID;

    fMonDetHit_X[n]	= hit->f3X.x()/__L_UNIT;
    fMonDetHit_Y[n]  	= hit->f3X.y()/__L_UNIT;
    fMonDetHit_Z[n]  	= hit->f3X.z()/__L_UNIT;

    fMonDetHit_Vx[n]	= hit->f3V.x()/__L_UNIT;
    fMonDetHit_Vy[n]  	= hit->f3V.y()/__L_UNIT;
    fMonDetHit_Vz[n]  	= hit->f3V.z()/__L_UNIT;

    fMonDetHit_Vdx[n]	= hit->f3D.x();
    fMonDetHit_Vdy[n]  	= hit->f3D.y();
    fMonDetHit_Vdz[n]  	= hit->f3D.z();

    fMonDetHit_Px[n]	= hit->f3P.x()/__E_UNIT;
    fMonDetHit_Py[n]	= hit->f3P.y()/__E_UNIT;
    fMonDetHit_Pz[n]	= hit->f3P.z()/__E_UNIT;

    fMonDetHit_P[n]	= hit->fP/__E_UNIT;
    fMonDetHit_E[n]	= hit->fE/__E_UNIT;
    fMonDetHit_M[n]	= hit->fM/__E_UNIT;

    fNMonDetHit++;

    return;
}


// Sciint Detector Hit
void qsimIO::AddScintDetectorHit(qsimScintDetectorHit *hit){
    int n = fNScintDetHit;
    if( n >= __IO_MAXHIT ){
//	G4cerr << "WARNING: " << __PRETTY_FUNCTION__ << " line " << __LINE__ << ":  Buffer size exceeded!" << G4endl;
	return;
    }

    fScintDetHit_det[n]  = hit->fDetID;
    fScintDetHit_id[n]   = hit->fCopyID;

    fScintDetHit_edep[n] = hit->fEdep/__E_UNIT;

    fNScintDetHit++;

    return;
}
