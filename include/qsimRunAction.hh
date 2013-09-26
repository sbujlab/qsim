
#ifndef qsimRunAction_h
#define qsimRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"

class G4Timer;
class G4Run;
class qsimIO;

class qsimRunAction : public G4UserRunAction
{
  public:
    qsimRunAction();
    ~qsimRunAction();

  public:
    void BeginOfRunAction(const G4Run* aRun);
    void EndOfRunAction(const G4Run* aRun);

    void SetIO( qsimIO *io ){ fIO = io; }

  private:
    G4Timer* timer;

    qsimIO *fIO;
};

#endif

