#include "qsimIO.hh"

#include "G4SystemOfUnits.hh"

#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>

#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"

#include "qsimDetectorHit.hh"
#include "qsimScintDetectorHit.hh"
#include "qsimGenericDetectorHit.hh"
#include "qsimGenericDetectorSum.hh"
#include "qsimEvent.hh"
#include "qsimRun.hh"
#include "qsimRunData.hh"


#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNode.hpp>

// Singleton
qsimIO* qsimIO::gInstance = 0;
qsimIO* qsimIO::GetInstance() {
  if (gInstance == 0) {
    gInstance = new qsimIO();
  }
  return gInstance;
}

qsimIO::qsimIO()
: fFile(0),fTree(0),fFilename("qsimout.root")
{
    //  Set arrays to 0
    fNGenDetHit = 0;
    fNGenDetSum = 0;

    InitializeTree();

    // Create generic messenger
    fMessenger = new G4GenericMessenger(this,"/qsim/","qsim properties");
    fMessenger->DeclareProperty("filename",fFilename,"Output filename");
}

qsimIO::~qsimIO(){
    if( fTree ){ delete fTree; 
    fTree = NULL;}
    if( fFile ){ delete fFile; 
    fFile = NULL;}
}
/*
void qsimIO::SetFilename(G4String fn){
    G4cout << "Setting output file to " << fn << G4endl;
    strcpy(fFilename, fn.data());
}
*/
void qsimIO::InitializeTree()
{
    if (fFile) {
        fFile->Close();
        delete fFile;
        fFile = NULL;
        fTree = NULL;
    }

    if (fTree) {
        delete fTree;
        fTree = NULL;
    }

    fFile = new TFile(fFilename, "RECREATE");

    fTree = new TTree("T", "Geant4 Quartz Detector Simulation");

    fTree->SetMaxTreeSize(1900000000); // 1.9GB

    // Event information
    fTree->Branch("ev.pid",   &fEvPart_PID, "ev.pid/I");
    fTree->Branch("ev.vx",    &fEvPart_X,   "ev.vx/D");
    fTree->Branch("ev.vy",    &fEvPart_Y,   "ev.vy/D");
    fTree->Branch("ev.vz",    &fEvPart_Z,   "ev.vz/D");
    fTree->Branch("ev.p",     &fEvPart_P,   "ev.p/D");
    fTree->Branch("ev.px",    &fEvPart_Px,  "ev.px/D");
    fTree->Branch("ev.py",    &fEvPart_Py,  "ev.py/D");
    fTree->Branch("ev.pz",    &fEvPart_Pz,  "ev.pz/D");
    fTree->Branch("ev.kine", &fEvPart_kinE, "ev.kine/D");

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
    
    fTree->Branch("hit.lambda", &fDetHit_Lambda, "hit.lambda[hit.n]/D");
    fTree->Branch("hit.eff", &fDetHit_Eff, "hit.m[hit.n]/D");

    // ScintDetectorHit
    fTree->Branch("sci.n",    &fNScintDetHit,     "sci.n/I");
    fTree->Branch("sci.det",  &fScintDetHit_det,  "sci.det[sci.n]/I");
    fTree->Branch("sci.id",  &fScintDetHit_id,  "sci.id[sci.n]/I");
    fTree->Branch("sci.edep",  &fScintDetHit_edep,  "sci.id[sci.n]/D");

    return;
}

void qsimIO::FillTree(){
    if( !fTree ){ 
	fprintf(stderr, "Error %s: %s line %d - Trying to fill non-existant tree\n", __PRETTY_FUNCTION__, __FILE__, __LINE__ );
	return; 
    }

    fTree->Fill();
    fTree->GetCurrentFile();
}

void qsimIO::Flush()
{
    //  Set arrays to 0
    fNDetHit = 0;
    fNGenDetHit = 0;
    fNGenDetSum = 0;
    fNScintDetHit = 0;
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
    qsimRun::GetRunData()->Write("run_data", TObject::kOverwrite);

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
    
    fEvPart_kinE = sqrt(ev->fPartMom[0].mag()*ev->fPartMom[0].mag() + ev->fPartType[0]->GetPDGMass()*ev->fPartType[0]->GetPDGMass() ) - ev->fPartType[0]->GetPDGMass()/__E_UNIT;

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

    fDetHit_Lambda[n] = hit->fLambda/__L_UNIT;
    fDetHit_Eff[n] = hit->fEff;

    if (CLHEP::RandFlat::shoot(0.0,1.0) <= hit->fEff)
        fNDetEp++;

    return;
}


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

// GDML implementation functions

void qsimIO::GrabGDMLFiles(G4String fn)
{
    // Reset list
    fGDMLFileNames.clear();

    qsimRunData *rundata = qsimRun::GetRunData();
    rundata->ClearGDMLFiles();

    SearchGDMLforFiles(fn);


    // Store filename

    // Copy into buffers
    for(unsigned int idx = 0; idx < fGDMLFileNames.size(); idx++ ){
        G4cout << "Found GDML file " << fGDMLFileNames[idx] << G4endl;
        rundata->AddGDMLFile(fGDMLFileNames[idx]);
    }
}

void qsimIO::SearchGDMLforFiles(G4String fn) 
{
    // Chase down files to be included by GDML
    // Mainly look for file tags and perform recursively
    
    struct stat thisfile;

    int ret = stat(fn.data(), &thisfile);
    if( ret != 0 ){
        G4cerr << "ERROR opening file " << fn <<  " in " << __PRETTY_FUNCTION__ << ": " << strerror(errno) << G4endl;
        exit(1);
    }

    fGDMLFileNames.push_back(fn);


    xercesc::XMLPlatformUtils::Initialize();

    xercesc::XercesDOMParser *xmlParser = new xercesc::XercesDOMParser();
    xmlParser->parse(fn.data());
    xercesc::DOMDocument* xmlDoc = xmlParser->getDocument();
    xercesc::DOMElement* elementRoot = xmlDoc->getDocumentElement();

    TraverseChildren( elementRoot );

    xercesc::XMLPlatformUtils::Terminate();

    delete xmlParser;
}

void qsimIO::TraverseChildren( xercesc::DOMElement *thisel )
{
    xercesc::DOMNodeList* children = thisel->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();

    for( XMLSize_t xx = 0; xx < nodeCount; ++xx ){
        xercesc::DOMNode* currentNode = children->item(xx);
        if( currentNode->getNodeType() ){   // true is not NULL

            if (currentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) { // is element
                xercesc::DOMElement* currentElement
                  = dynamic_cast< xercesc::DOMElement* >( currentNode );
                // transcode
                XMLCh* str_file = xercesc::XMLString::transcode("file");
                if( xercesc::XMLString::equals(currentElement->getTagName(), str_file)){
                    // transcode
                    XMLCh* str_name = xercesc::XMLString::transcode("name");
                    const XMLCh* attr = currentElement->getAttribute(str_name);
                    char* str_attr = xercesc::XMLString::transcode(attr);
                    // search files
                    SearchGDMLforFiles(G4String(str_attr));
                    // release
                    xercesc::XMLString::release(&str_name);
                    xercesc::XMLString::release(&str_attr);
                }
                // release
                xercesc::XMLString::release(&str_file);

                if( currentElement->getChildNodes()->getLength() > 0 ){
                    TraverseChildren( currentElement );
                }
            }
        }
    }
}
