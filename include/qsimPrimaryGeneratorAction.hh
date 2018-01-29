
#ifndef qsimPrimaryGeneratorAction_h
#define qsimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4String.hh"

class G4ParticleGun;
class G4Event;
class qsimIO;
class qsimEvent;

class qsimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    qsimPrimaryGeneratorAction();
    ~qsimPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);
    G4ParticleGun* GetParticleGun();
    void SetIO( qsimIO *io ){ fIO = io; }

  private:
    G4ParticleGun* fParticleGun;

    qsimEvent *fDefaultEvent;
    qsimIO *fIO;

  public:
    G4double fXmin, fXmax, fYmin, fYmax, fZmin, fZmax;
    G4double fThetaMin, fThetaMax, fPhiMin, fPhiMax;
    G4double fEmin, fEmax;
};

#endif


