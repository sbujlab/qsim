#include "qsimRun.hh"
#include "qsimRunData.hh"

qsimRun *qsimRun::gSingleton = NULL;

qsimRun::qsimRun(){
    gSingleton = this;
    fRunData = new qsimRunData();
    fRunData->Init();
}

qsimRun::~qsimRun(){}

qsimRun *qsimRun::GetRun(){
    if( gSingleton == NULL ){
        gSingleton = new qsimRun();
    }
    return gSingleton;
}
