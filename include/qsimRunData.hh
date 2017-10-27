#ifndef __QSIMRUNDATA_HH
#define __QSIMRUNDATA_HH

#include "TObject.h"
#include <vector>
#include <string>

#include "qsimtypes.hh"
#include "qsimTextFile.hh"

/*
 * All the information on the run
 * This will get put into the output
 * stream
 */

class TGeoManager;

class qsimRunData : public TObject {
    using TObject::Print;
    public:
    qsimRunData();
    ~qsimRunData();

    unsigned long long int GetNthrown(){ return fNthrown; }
    void SetNthrown(unsigned long long int n){ fNthrown = n; }

    void Init();
    
    void SetGenName(const char *n){ strcpy(fGenName, n); }
    const char *GetGenName(){ return fGenName; }

    void SetBeamE(double E){ fBeamE = E; }
    void SetSeed(unsigned int seed){ fSeed = seed; }

    void AddMagData(filedata_t d){fMagData.push_back(d);}
    void SetMacroFile(const char *fn){ fMacro = qsimTextFile(fn); }
    void AddGDMLFile(const char *fn);
    void ClearGDMLFiles(){ fGDMLFiles.clear(); }
                                                 
    void RecreateGDML(const char *adir = NULL, bool clobber = false);
                                                   
    qsimTextFile GetGDMLFile(int i){ return fGDMLFiles[i]; }
    
    void Print();

    TTimeStamp fRunTime;

    long int  fNthrown;
    unsigned int  fSeed;
    double fBeamE;
    char fGenName[__RUNSTR_LEN];
     
    char fHostName[__RUNSTR_LEN];
    char fRunPath[__RUNSTR_LEN];
    qsimTextFile              fMacro;
    std::vector<qsimTextFile> fGDMLFiles;

    std::vector<filedata_t> fMagData;

    ClassDef(qsimRunData, 1);
};

#endif// __QSIMRUNDATA_HH
