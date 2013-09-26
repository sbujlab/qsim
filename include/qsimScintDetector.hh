#ifndef __QSIMSCINTDETECTOR_HH
#define __QSIMSCINTDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "qsimScintDetectorHit.hh"

#include <map>

/*! 
      Default detector class.  This will record information on:

      - Primary generated hit information

*/

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class qsimScintDetector : public G4VSensitiveDetector {
    public:
	qsimScintDetector( G4String name, G4int detnum );
	virtual ~qsimScintDetector();

	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);

    private:
	qsimScintDetectorHitsCollection *fHitColl;
	G4int fHCID;

	G4bool fTrackSecondaries;
	G4int fDetNo;

	std::map<int, qsimScintDetectorHit *> fSumMap;

	G4double fDontRecordThresh;
};

#endif//__QSIMSCINTDETECTOR_HH
