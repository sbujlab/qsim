#ifndef __QSIMOPTICALPHYSICS_HH
#define __QSIMOPTICALPHYSICS_HH 

#include "globals.hh"

#include "G4OpWLS.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"

#include "G4OpMieHG.hh"
#include "G4OpRayleigh.hh"
#include "G4OpAbsorption.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4VPhysicsConstructor.hh"

class qsimOpticalPhysics : public G4VPhysicsConstructor
{
  public:

    qsimOpticalPhysics();
    qsimOpticalPhysics(G4bool toggle=true);
    ~qsimOpticalPhysics();
    //virtual ~qsimOpticalPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

    G4OpWLS* GetWLSProcess() {return theWLSProcess;}
    G4Cerenkov* GetCerenkovProcess() {return theCerenkovProcess;}
    G4Scintillation* GetScintillationProcess() {return theScintProcess;}
    G4OpAbsorption* GetAbsorptionProcess() {return theAbsorptionProcess;}
    G4OpRayleigh* GetRayleighScatteringProcess() {return theRayleighScattering;}
    G4OpMieHG* GetMieHGScatteringProcess() {return theMieHGScatteringProcess;}
    G4OpBoundaryProcess* GetBoundaryProcess() { return theBoundaryProcess;}

	void AllowCerenkovSet(G4bool );
	void AllowScintillationSet(G4bool );

    void SetNbOfPhotonsCerenkov(G4int);

private:

    G4OpWLS*             theWLSProcess;
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScattering;
    G4OpMieHG*           theMieHGScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;

    bool fAllowCerenkov;
    bool fAllowScintillation;
 
    G4bool AbsorptionOn;

};
#endif//__QSIMOPTICALPHYSICS_HH 
