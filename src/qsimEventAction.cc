#include "qsimEventAction.hh"
#include "qsimDetectorHit.hh"
#include "qsimScintDetectorHit.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#include "qsimIO.hh"


qsimEventAction::qsimEventAction() {
}

qsimEventAction::~qsimEventAction(){
}


void qsimEventAction::BeginOfEventAction(const G4Event*ev) {
    // Pretty ongoing status with flush
    if( (ev->GetEventID() % 10) == 0 ){
	printf("Event %8d\r", ev->GetEventID() );
	fflush(stdout);
    }

    return;
}

void qsimEventAction::EndOfEventAction(const G4Event* evt ) {
  //G4SDManager   *SDman = G4SDManager::GetSDMpointer();
  G4HCofThisEvent *HCE = evt->GetHCofThisEvent();

  G4VHitsCollection *thiscol;

  // Traverse all hit collections, sort by output type
  for( int hcidx = 0; hcidx < HCE->GetCapacity(); hcidx++ ){
      thiscol = HCE->GetHC(hcidx);
      if(thiscol){ // This is NULL if nothing is stored
	  // Dyanmic cast to test types, process however see fit and feed to IO
	  
	  ////  Detector Hits ///////////////////////////////////
	  if( qsimDetectorHitsCollection *thiscast = 
		  dynamic_cast<qsimDetectorHitsCollection *>(thiscol)){
	      for( unsigned int hidx = 0; hidx < thiscast->GetSize(); hidx++ ){
		  fIO->AddDetectorHit(
			  (qsimDetectorHit *) thiscast->GetHit(hidx) );
	      }
	  }
	  
	  ////  Scint Generic Detector Hits ///////////////////////////////////
	  if( qsimScintDetectorHitsCollection *thiscast =
		  dynamic_cast<qsimScintDetectorHitsCollection *>(thiscol)){
	      for( unsigned int hidx = 0; hidx < thiscast->GetSize(); hidx++ ){
		  fIO->AddScintDetectorHit(
			  (qsimScintDetectorHit *) thiscast->GetHit(hidx) );
	      }
	  }


      }
  }

  // Fill tree and reset buffers
  fIO->FillTree();
  fIO->Flush();

  return;
}



