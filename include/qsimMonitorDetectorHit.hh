#ifndef __QSIMMONITORDETECTORHIT_HH
#define __QSIMMONITORDETECTORHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class qsimMonitorDetectorHit : public G4VHit {
    public:
	qsimMonitorDetectorHit(G4int, G4int);
	~qsimMonitorDetectorHit();

	qsimMonitorDetectorHit(const qsimMonitorDetectorHit &right);
	const qsimMonitorDetectorHit& operator=(const qsimMonitorDetectorHit &right);
	G4int operator==(const qsimMonitorDetectorHit &right) const;

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
};


typedef G4THitsCollection<qsimMonitorDetectorHit> qsimMonitorDetectorHitsCollection;

extern G4Allocator<qsimMonitorDetectorHit> qsimMonitorDetectorHitAllocator;

inline void* qsimMonitorDetectorHit::operator new(size_t){
    void *aHit;
    aHit = (void *) qsimMonitorDetectorHitAllocator.MallocSingle();
    return aHit;
}

inline void qsimMonitorDetectorHit::operator delete(void *aHit){
    qsimMonitorDetectorHitAllocator.FreeSingle( (qsimMonitorDetectorHit*) aHit);
}

#endif//__QSIMMONITORDETECTORHIT_HH
