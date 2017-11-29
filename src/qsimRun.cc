#include "qsimRun.hh"

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "qsimRunData.hh"
qsimRunData* qsimRun::fRunData = 0;
qsimRunData* qsimRun::GetRunData()
{
  if (!fRunData) {
    fRunData = new qsimRunData();
    fRunData->Init();
  }
  return fRunData;
}

qsimRun::qsimRun(){ }

qsimRun::~qsimRun(){ }
