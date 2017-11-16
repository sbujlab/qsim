#ifndef __QSIMGENERICDETECTORSUM_HH
#define __QSIMGENERICDETECTORSUM_HH

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class qsimGenericDetectorSum : public G4VHit {
    public:
	qsimGenericDetectorSum(G4int, G4int);
	virtual ~qsimGenericDetectorSum();

	qsimGenericDetectorSum(const qsimGenericDetectorSum &right);
	const qsimGenericDetectorSum& operator=(const qsimGenericDetectorSum &right);
	G4int operator==(const qsimGenericDetectorSum &right) const;

	inline void *operator new(size_t);
	inline void operator delete(void *aHit);

    public:
	G4int    fDetID;
	G4int    fCopyID;
	G4double fEdep;
};


typedef G4THitsCollection<qsimGenericDetectorSum> qsimGenericDetectorSumCollection;

extern G4ThreadLocal G4Allocator<qsimGenericDetectorSum>* qsimGenericDetectorSumAllocator;

inline void* qsimGenericDetectorSum::operator new(size_t){
  if (!qsimGenericDetectorSumAllocator)
    qsimGenericDetectorSumAllocator = new G4Allocator<qsimGenericDetectorSum>;
  return (void *) qsimGenericDetectorSumAllocator->MallocSingle();
}

inline void qsimGenericDetectorSum::operator delete(void *aHit){
  qsimGenericDetectorSumAllocator->FreeSingle( (qsimGenericDetectorSum*) aHit);
}

#endif//__QSIMGENERICDETECTORSUM_HH
