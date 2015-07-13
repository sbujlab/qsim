
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
		bool Thetaspectrum(double );
		bool pspectrum(double );
		//void SourceModeSet();
		void SourceModeSet(G4int );

	private:
    G4ParticleGun* fParticleGun;
		G4int fSourceMode;

    qsimEvent *fDefaultEvent;
    qsimIO *fIO;

  public:
		G4double fXmin, fXmax, fYmin, fYmax;
    G4double fZ;
    G4double fEmin, fEmax;
		G4double fthetaMin, fthetaMax;
};

#endif


