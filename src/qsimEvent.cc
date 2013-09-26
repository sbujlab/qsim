#include "qsimEvent.hh"
#include <math.h>

#include "G4ParticleTable.hh"

qsimEvent::qsimEvent(){
    Reset();
}

qsimEvent::~qsimEvent(){
}

void qsimEvent::ProduceNewParticle( G4ThreeVector pos, G4ThreeVector mom, G4String name ){
    fPartPos.push_back(pos);
    fPartMom.push_back(mom);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(name);

    fPartType.push_back(particle);

    return;
}


void qsimEvent::Reset(){
    fPartPos.clear();
    fPartMom.clear();
    fPartType.clear();

}

void qsimEvent::UndoLastParticle(){
    fPartPos.pop_back();
    fPartMom.pop_back();
    fPartType.pop_back();
}

G4bool qsimEvent::EventIsSane(){
    // Here we check all the variables and make sure there is nothing 
    // kinematically wrong and there aren't stuff like nans and infs

    unsigned int i;

    if( fPartPos.size() < 1 ){ 
	return false;
    }

    for( i = 0; i < fPartPos.size(); i++ ){
	if( !fPartType[i] ){ return false; }

	if( std::isnan(fPartPos[i].x()) || std::isinf(fPartPos[i].x()) ) return false;
	if( std::isnan(fPartPos[i].y()) || std::isinf(fPartPos[i].y()) ) return false;
	if( std::isnan(fPartPos[i].z()) || std::isinf(fPartPos[i].z()) ) return false;

	if( std::isnan(fPartMom[i].x()) || std::isinf(fPartMom[i].x()) ) return false;
	if( std::isnan(fPartMom[i].y()) || std::isinf(fPartMom[i].y()) ) return false;
	if( std::isnan(fPartMom[i].z()) || std::isinf(fPartMom[i].z()) ) return false;
    }

    return true;
}


void qsimEvent::Print(){
    G4cout << "Event " << this << " dump" << G4endl;

    G4cout << "\t" << fPartPos.size() << " particles generated" << G4endl;

    unsigned int i;

    for( i = 0; i < fPartPos.size(); i++ ){
	if( !fPartType[i] ){
	    G4cout << "\tParticle type for " << i << " not defined" << G4endl;
	} else {
	    G4cout << "\t" << fPartType[i]->GetParticleName() << ":" << G4endl;
	    G4cout << "\t\tat (" << fPartPos[i].x()/m << ", " << fPartPos[i].y()/m << ", " << fPartPos[i].z()/m  << ") m" << G4endl;
	    G4cout << "\t\tof (" << fPartMom[i].x()/GeV << ", " << fPartMom[i].y()/GeV << ", " << fPartMom[i].z()/GeV  << ") GeV" << G4endl;
	}
    }
}













