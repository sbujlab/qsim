#ifndef qsimIO_HH
#define qsimIO_HH

#include "TROOT.h"
#include "TObject.h"
#include "qsimtypes.hh"
#include "G4String.hh"

class TFile;
class TTree;

class G4GenericMessenger;

class qsimGenericDetectorHit;
class qsimGenericDetectorSum;
class qsimEvent;
class qsimDetectorHit;
class qsimScintDetectorHit;

#include <xercesc/dom/DOMElement.hpp>

#define __IO_MAXHIT 10000
#define __FILENAMELEN 255

// Units for output
#define __E_UNIT GeV
#define __L_UNIT m
#define __T_UNIT ns
#define __ANG_UNIT rad
#define __ASYMM_SCALE 1e-9 // ppb

class qsimIO {
    private:
        // Singleton pointer
        static qsimIO* gInstance;
        // Private constructor
        qsimIO();
    
    public:
        // Public destructor
  	    virtual ~qsimIO();
        // Static instance getter
        static qsimIO* GetInstance();

	void SetFilename(const G4String& name) { 
    std::cout<<"test IO 1.0"<<std::endl;
    fFilename = name; 
    std::cout<<"test IO 1.1"<<std::endl;
  }
	G4String GetFilename() const { return fFilename; }

	void FillTree();
	void Flush();
	void WriteTree();

	void WriteRun();

	void InitializeTree();

    void GrabGDMLFiles( G4String fn );

    private:
	TFile *fFile;
	TTree *fTree;

	    G4GenericMessenger* fMessenger;
        
        G4String fFilename;
        
        std::vector<G4String> fGDMLFileNames;
    
        void SearchGDMLforFiles(G4String );
        void TraverseChildren( xercesc::DOMElement * );

	//  Interfaces and buffers to the tree
	//  This is potentially going to get very long...

	// Event data
    public:
    void SetEventSeed(const G4String& seed);
	void SetEventData(qsimEvent *);
    private:
	Double_t fEvPart_X;
	Double_t fEvPart_Y;
	Double_t fEvPart_Z;
	Double_t fEvPart_P;
	Double_t fEvPart_Px;
	Double_t fEvPart_Py;
	Double_t fEvPart_Pz;
	Int_t fEvPart_PID;
        Double_t fEvPart_kinE;

	//  DetectorHit
    public:
	void AddDetectorHit(qsimDetectorHit *);
    private:
	Int_t fNDetHit;
        //
        Int_t fNDetEp;
        //
	Int_t fDetHit_det[__IO_MAXHIT];
	Int_t fDetHit_id[__IO_MAXHIT];

	Int_t fDetHit_trid[__IO_MAXHIT];
	Int_t fDetHit_pid[__IO_MAXHIT];
	Int_t fDetHit_gen[__IO_MAXHIT];
	Int_t fDetHit_mtrid[__IO_MAXHIT];

	Double_t fDetHit_X[__IO_MAXHIT];
	Double_t fDetHit_Y[__IO_MAXHIT];
	Double_t fDetHit_Z[__IO_MAXHIT];

	Double_t fDetHit_Px[__IO_MAXHIT];
	Double_t fDetHit_Py[__IO_MAXHIT];
	Double_t fDetHit_Pz[__IO_MAXHIT];
	Double_t fDetHit_P[__IO_MAXHIT];
	Double_t fDetHit_E[__IO_MAXHIT];
	Double_t fDetHit_M[__IO_MAXHIT];
        //
        Double_t fDetHit_Lambda[__IO_MAXHIT];
        Double_t fDetHit_Eff[__IO_MAXHIT];
        //
        //
	Double_t fDetHit_Vx[__IO_MAXHIT];
	Double_t fDetHit_Vy[__IO_MAXHIT];
	Double_t fDetHit_Vz[__IO_MAXHIT];
	Double_t fDetHit_Vdx[__IO_MAXHIT];
	Double_t fDetHit_Vdy[__IO_MAXHIT];
	Double_t fDetHit_Vdz[__IO_MAXHIT];


    //  GenericDetectorHit
    public:
    void AddGenericDetectorHit(qsimGenericDetectorHit *);
    private:
	Int_t fNGenDetHit;
	Int_t fGenDetHit_det[__IO_MAXHIT];
	Int_t fGenDetHit_id[__IO_MAXHIT];

	Int_t fGenDetHit_trid[__IO_MAXHIT];
	Int_t fGenDetHit_pid[__IO_MAXHIT];
	Int_t fGenDetHit_gen[__IO_MAXHIT];
	Int_t fGenDetHit_mtrid[__IO_MAXHIT];

	Double_t fGenDetHit_X[__IO_MAXHIT];
	Double_t fGenDetHit_Y[__IO_MAXHIT];
	Double_t fGenDetHit_Z[__IO_MAXHIT];
	Double_t fGenDetHit_R[__IO_MAXHIT];
	Double_t fGenDetHit_Ph[__IO_MAXHIT];

	Double_t fGenDetHit_Px[__IO_MAXHIT];
	Double_t fGenDetHit_Py[__IO_MAXHIT];
	Double_t fGenDetHit_Pz[__IO_MAXHIT];
	Double_t fGenDetHit_P[__IO_MAXHIT];
	Double_t fGenDetHit_E[__IO_MAXHIT];
	Double_t fGenDetHit_M[__IO_MAXHIT];

	Double_t fGenDetHit_Vx[__IO_MAXHIT];
	Double_t fGenDetHit_Vy[__IO_MAXHIT];
	Double_t fGenDetHit_Vz[__IO_MAXHIT];

	Int_t fCollCut;

	//  GenericDetectorSum
    public:
	void AddGenericDetectorSum(qsimGenericDetectorSum *);
    private:
	Int_t fNGenDetSum;
	Int_t fGenDetSum_det[__IO_MAXHIT];
	Int_t fGenDetSum_id[__IO_MAXHIT];
	Double_t fGenDetSum_edep[__IO_MAXHIT];

	
    
    
    //  ScintDetectorHit
    public:
	void AddScintDetectorHit(qsimScintDetectorHit *);
    private:
	Int_t fNScintDetHit;
	Int_t fScintDetHit_det[__IO_MAXHIT];
	Int_t fScintDetHit_id[__IO_MAXHIT];

	Double_t fScintDetHit_edep[__IO_MAXHIT];

};

#endif//qsimIO_HH
