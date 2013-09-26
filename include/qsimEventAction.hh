
#ifndef qsimEventAction_h
#define qsimEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class qsimIO;

class qsimEventAction : public G4UserEventAction
{
  public:
    qsimEventAction();
    virtual ~qsimEventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void SetIO( qsimIO *io ){ fIO = io; }

  private:

    qsimIO *fIO;

  public:
};

#endif

    
