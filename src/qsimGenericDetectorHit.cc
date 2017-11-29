#include "qsimGenericDetectorHit.hh"

G4ThreadLocal G4Allocator<qsimGenericDetectorHit>* qsimGenericDetectorHitAllocator = 0;

qsimGenericDetectorHit::qsimGenericDetectorHit(G4int det, G4int copy)
: fDetID(det),fCopyID(copy)
{
  f3X = G4ThreeVector(-1e9, -1e9, -1e9);
  f3P = G4ThreeVector(-1e9, -1e9, -1e9);
  f3V = G4ThreeVector(-1e9, -1e9, -1e9);

  fP  = -1.0;
  fE  = -1.0;
  fM  = -1.0;

  fTrID  = -1;
  fPID   = (G4int) 1e9;
  fmTrID = -1;

  fGen   = 1;
}

qsimGenericDetectorHit::~qsimGenericDetectorHit() { }

qsimGenericDetectorHit::qsimGenericDetectorHit(const qsimGenericDetectorHit &right)
: G4VHit(right) {
  // copy constructor
  fDetID  = right.fDetID;
  fCopyID = right.fCopyID;
  f3X     = right.f3X;
  f3P     = right.f3P;
  f3V     = right.f3V;

  fP      = right.fP;
  fE      = right.fE;
  fM      = right.fM;

  fTrID   = right.fTrID;
  fPID    = right.fPID;
  fmTrID  = right.fmTrID;
  fGen    = right.fGen;
}

const qsimGenericDetectorHit& qsimGenericDetectorHit::operator=(const qsimGenericDetectorHit &right) {
  (*this) = right;
  return *this;
}

G4int qsimGenericDetectorHit::operator==(const qsimGenericDetectorHit &right) const {
  return (this==&right) ? 1 : 0;
}
