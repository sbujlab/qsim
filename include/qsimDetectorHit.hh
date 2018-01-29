#ifndef __QSIMDETECTORHIT_HH
#define __QSIMDETECTORHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class qsimDetectorHit : public G4VHit {
    public:
	qsimDetectorHit(G4int, G4int);
	~qsimDetectorHit();

	qsimDetectorHit(const qsimDetectorHit &right);
	const qsimDetectorHit& operator=(const qsimDetectorHit &right);
	G4int operator==(const qsimDetectorHit &right) const;

	inline void *operator new(size_t);
	inline void operator delete(void *aHit);
	void *operator new(size_t,void*p){return p;}

    private:

    public:
	G4int fDetID;
	G4int fCopyID;

	// Position and momentum in lab coordinates
	G4ThreeVector f3X;
	G4ThreeVector f3P;
	// Total momentum, energy, mass
	G4double fP, fE, fM;
        // 
        G4double fLambda;
        G4double fEff;
        //
	// Origin
	G4ThreeVector f3V;
	G4ThreeVector f3D;
	// Geant4 track ID, particle type, and mother ID
	G4int    fTrID, fPID, fmTrID;
	// Process generator type
	G4int    fGen;
};


typedef G4THitsCollection<qsimDetectorHit> qsimDetectorHitsCollection;

extern G4Allocator<qsimDetectorHit> qsimDetectorHitAllocator;

inline void* qsimDetectorHit::operator new(size_t){
    void *aHit;
    aHit = (void *) qsimDetectorHitAllocator.MallocSingle();
    return aHit;
}

inline void qsimDetectorHit::operator delete(void *aHit){
    qsimDetectorHitAllocator.FreeSingle( (qsimDetectorHit*) aHit);
}

#endif//__QSIMDETECTORHIT_HH
