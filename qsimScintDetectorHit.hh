#ifndef __QSIMSCINTDETECTORHIT_HH
#define __QSIMSCINTDETECTORHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class qsimScintDetectorHit : public G4VHit {
    public:
	qsimScintDetectorHit(G4int, G4int);
	~qsimScintDetectorHit();

	qsimScintDetectorHit(const qsimScintDetectorHit &right);
	const qsimScintDetectorHit& operator=(const qsimScintDetectorHit &right);
	G4int operator==(const qsimScintDetectorHit &right) const;

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
	// Origin
	G4ThreeVector f3V;
	G4ThreeVector f3D;
	// Geant4 track ID, particle type, and mother ID
	G4int    fTrID, fPID, fmTrID;
	// Process generator type
	G4int    fGen;

	G4double fEdep;
};


typedef G4THitsCollection<qsimScintDetectorHit> qsimScintDetectorHitsCollection;

extern G4Allocator<qsimScintDetectorHit> qsimScintDetectorHitAllocator;

inline void* qsimScintDetectorHit::operator new(size_t){
    void *aHit;
    aHit = (void *) qsimScintDetectorHitAllocator.MallocSingle();
    return aHit;
}

inline void qsimScintDetectorHit::operator delete(void *aHit){
    qsimScintDetectorHitAllocator.FreeSingle( (qsimScintDetectorHit*) aHit);
}

#endif//__QSIMSCINTDETECTORHIT_HH
