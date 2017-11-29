#ifndef __QSIMGENERICDETECTORHIT_HH
#define __QSIMGENERICDETECTORHIT_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class qsimGenericDetectorHit : public G4VHit {
    public:
	qsimGenericDetectorHit(G4int, G4int);
	virtual ~qsimGenericDetectorHit();

	qsimGenericDetectorHit(const qsimGenericDetectorHit &right);
	const qsimGenericDetectorHit& operator=(const qsimGenericDetectorHit &right);
	G4int operator==(const qsimGenericDetectorHit &right) const;

	inline void *operator new(size_t);
	inline void operator delete(void *aHit);

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
	// Geant4 track ID, particle type, and mother ID
	G4int    fTrID, fPID, fmTrID;
	// Process generator type
	G4int    fGen;

};


typedef G4THitsCollection<qsimGenericDetectorHit> qsimGenericDetectorHitCollection;

extern G4ThreadLocal G4Allocator<qsimGenericDetectorHit>* qsimGenericDetectorHitAllocator;

inline void* qsimGenericDetectorHit::operator new(size_t){
  if (!qsimGenericDetectorHitAllocator)
    qsimGenericDetectorHitAllocator = new G4Allocator<qsimGenericDetectorHit>;
  return (void *) qsimGenericDetectorHitAllocator->MallocSingle();
}

inline void qsimGenericDetectorHit::operator delete(void *aHit){
  qsimGenericDetectorHitAllocator->FreeSingle( (qsimGenericDetectorHit*) aHit);
}

#endif//__QSIMGENERICDETECTORHIT_HH
