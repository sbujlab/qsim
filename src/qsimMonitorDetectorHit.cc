#include "qsimMonitorDetectorHit.hh"

G4Allocator<qsimMonitorDetectorHit> qsimMonitorDetectorHitAllocator;

qsimMonitorDetectorHit::qsimMonitorDetectorHit(G4int det, G4int copy){
    fDetID  = det;
    fCopyID = copy;

    f3X = G4ThreeVector(-1e9, -1e9, -1e9);
    f3P = G4ThreeVector(-1e9, -1e9, -1e9);
    f3V = G4ThreeVector(-1e9, -1e9, -1e9);
    f3D = G4ThreeVector(-1e9, -1e9, -1e9);

    fP  = -1.0;
    fE  = -1.0;
    fM  = -1.0;

    fTrID  = -1;
    fPID   = (G4int) 1e9;
    fmTrID = -1;

    fGen   = 1;
}

qsimMonitorDetectorHit::~qsimMonitorDetectorHit(){
}

qsimMonitorDetectorHit::qsimMonitorDetectorHit(const qsimMonitorDetectorHit &right) : G4VHit(){
    // copy constructor

    fDetID  = right.fDetID;
    fCopyID = right.fCopyID;
    f3X     = right.f3X;
    f3P     = right.f3P;
    f3D     = right.f3D;
    f3V     = right.f3V;

    fP      = right.fP;
    fE      = right.fE;
    fM      = right.fM;

    fTrID   = right.fTrID;
    fPID    = right.fPID;
    fmTrID  = right.fmTrID;
    fGen    = right.fGen;
}

const qsimMonitorDetectorHit& qsimMonitorDetectorHit::operator =(const qsimMonitorDetectorHit &right){
    (*this) = right;
    return *this;
}

G4int qsimMonitorDetectorHit::operator==(const qsimMonitorDetectorHit &right ) const {
    return (this==&right) ? 1 : 0;
}
