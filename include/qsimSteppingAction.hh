
#ifndef __QSIMSTEPPINGACTION_HH
#define __QSIMSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class qsimSteppingAction : public G4UserSteppingAction
{
  public:
    qsimSteppingAction();
    virtual ~qsimSteppingAction(){};

    virtual void UserSteppingAction(const G4Step*);

  private:
    G4bool drawFlag;

  public:
    inline void SetDrawFlag(G4bool val)
    { drawFlag = val; };
};

#endif//__QSIMSTEPPINGACTION_HH
