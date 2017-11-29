#include "qsimGenericDetectorSum.hh"

G4ThreadLocal G4Allocator<qsimGenericDetectorSum>* qsimGenericDetectorSumAllocator = 0;

qsimGenericDetectorSum::qsimGenericDetectorSum(int detid, int copyid)
: fDetID(detid),fCopyID(copyid) {
  fEdep   = 0.0;
}

qsimGenericDetectorSum::~qsimGenericDetectorSum() { }

qsimGenericDetectorSum::qsimGenericDetectorSum(const qsimGenericDetectorSum &right)
: G4VHit(right) {
  // copy constructor
  fDetID  = right.fDetID;
  fCopyID = right.fCopyID;
  fEdep   = right.fEdep;
}

const qsimGenericDetectorSum& qsimGenericDetectorSum::operator=(const qsimGenericDetectorSum &right){
  (*this) = right;
  return *this;
}

G4int qsimGenericDetectorSum::operator==(const qsimGenericDetectorSum &right) const {
  return (this==&right) ? 1 : 0;
}
