#ifndef __QSIMMONITORDETECTOR_HH
#define __QSIMMONITORDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "qsimMonitorDetectorHit.hh"

#include <map>

/*! 
      Default detector class.  This will record information on:

      - Primary generated hit information

*/

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class qsimMonitorDetector : public G4VSensitiveDetector {
    public:
	qsimMonitorDetector( G4String name, G4int detnum );
	virtual ~qsimMonitorDetector();

	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);

    private:
	qsimMonitorDetectorHitsCollection *fHitColl;
	G4int fHCID;

	G4bool fTrackSecondaries;
	G4int fDetNo;

	G4double fDontRecordThresh;
};

#endif//__QSIMMONITORDETECTOR_HH
