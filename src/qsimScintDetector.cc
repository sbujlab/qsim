#include "qsimScintDetector.hh"
#include "G4SDManager.hh"

qsimScintDetector::qsimScintDetector( G4String name, G4int detnum ) : G4VSensitiveDetector(name){
    char colname[255];

    fDetNo = detnum;
    assert( fDetNo > 0 );

    fHCID = -1;

//    fTrackSecondaries = false;
    fTrackSecondaries = true;

    sprintf(colname, "scihit_%s_%d", name.data(), detnum);
    collectionName.insert(G4String(colname));

}

qsimScintDetector::~qsimScintDetector(){
}

void qsimScintDetector::Initialize(G4HCofThisEvent *){
    fHitColl = new qsimScintDetectorHitsCollection( SensitiveDetectorName, collectionName[0] );

    fSumMap.clear();
}

///////////////////////////////////////////////////////////////////////

G4bool qsimScintDetector::ProcessHits( G4Step *step, G4TouchableHistory *){
    G4bool badedep = false;
    G4bool badhit = false;

    // Get touchable volume info
    G4TouchableHistory *hist = 
	(G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4int  copyID = hist->GetReplicaNumber();

//    G4StepPoint *prestep = step->GetPreStepPoint();
//    G4Track     *track   = step->GetTrack();

//    G4Material* material = track->GetMaterial();

    G4double edep = step->GetTotalEnergyDeposit();

//    printf("Hit in %s by %s with edep %f MeV\n", hist->GetVolume()->GetName().data(), track->GetParticleDefinition()->GetParticleName().data(), edep/MeV  );

    qsimScintDetectorHit *thissum = NULL;

    if( !fSumMap.count(copyID) ){
	if( edep > 0.0 ){
	    thissum = new qsimScintDetectorHit(fDetNo, copyID);
	    fSumMap[copyID] = thissum;
	    fHitColl->insert( thissum );

	} else {
	    badedep = true;
	}
    } else {
	thissum = fSumMap[copyID];
    }

    if( !badedep ){
	thissum->fEdep += edep;
    }

    return !badedep && !badhit;
}

///////////////////////////////////////////////////////////////////////

void qsimScintDetector::EndOfEvent(G4HCofThisEvent*HCE) {
    G4SDManager *sdman = G4SDManager::GetSDMpointer();

    if(fHCID<0){ fHCID = sdman->GetCollectionID(collectionName[0]); }

    HCE->AddHitsCollection( fHCID, fHitColl );
    return;
}


