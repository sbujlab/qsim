#ifndef __QSIMGENERICDETECTOR_HH
#define __QSIMGENERICDETECTOR_HH

#include "G4VSensitiveDetector.hh"

// Included to avoid forward declaration of collection typedef
#include "qsimGenericDetectorHit.hh"
#include "qsimGenericDetectorSum.hh"

#include <map>

/*! 
      Default detector class.  This will record information on:

      - Primary generated hit information
      - Secondary hit information on particles not produced within the volume
      - Calorimetric data (total energy deposited)

      This requires two hit collections, one for "hits" and one for "sum"
      Summing occurs uniquely over copyIDs
*/

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class qsimGenericDetectorSum;

class qsimGenericDetector : public G4VSensitiveDetector {
    public:
	qsimGenericDetector( G4String name, G4int detnum );
	virtual ~qsimGenericDetector();

	virtual void Initialize(G4HCofThisEvent*);
	virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
	virtual void EndOfEvent(G4HCofThisEvent*);

    private:
	qsimGenericDetectorHitCollection *fHitColl;
	qsimGenericDetectorSumCollection *fSumColl;

	G4int fHCID, fSCID;

	std::map<int, qsimGenericDetectorSum *> fSumMap;

	G4bool fTrackSecondaries;
	G4int fDetNo;

};

#endif//__QSIMGENERICDETECTOR_HH
