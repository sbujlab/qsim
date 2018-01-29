#ifndef __QSIMDETECTOR_HH
#define __QSIMDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "qsimDetectorHit.hh"

#include <map>

//
#include <fstream>
#include <iostream>
//

/*! 
      Default detector class.  This will record information on:

      - Primary generated hit information

*/

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class qsimDetector : public G4VSensitiveDetector {
    public:
	qsimDetector( G4String name, G4int detnum );
	virtual ~qsimDetector();

	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);

        //
        double lambda, QE, thisLambda ,thisQE;
        //

    private:
	qsimDetectorHitsCollection *fHitColl;
	G4int fHCID;

	G4bool fTrackSecondaries;
	G4int fDetNo;

	G4double fDontRecordThresh;
};

#endif//__QSIMDETECTOR_HH
