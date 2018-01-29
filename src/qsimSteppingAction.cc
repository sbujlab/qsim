#include "qsimSteppingAction.hh"

#include "G4VVisManager.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SteppingManager.hh"

qsimSteppingAction::qsimSteppingAction()
:drawFlag(false)
{

}

void qsimSteppingAction::UserSteppingAction(const G4Step *aStep) {
}


