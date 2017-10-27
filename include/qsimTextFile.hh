#ifndef __QSIMTEXTFILE_HH
#define __QSIMTEXTFILE_HH

#define __STRLEN 1024

#include "TObject.h"

class qsimTextFile : public TObject {
    using TObject::Print;
    public:
    qsimTextFile();
    qsimTextFile(const qsimTextFile &);
    const qsimTextFile& operator=(const qsimTextFile &);
    qsimTextFile(const char *);
    ~qsimTextFile();
                                                 
    void copyFileIn(const char *);
                                                   
    void Print();

    const char *GetFilename(){ return fFilename; }
    unsigned long long int GetBufferSize(){ return fBufferSize; }
    
    void Recreate(const char *fn = NULL, bool clobber = false);
    void RecreateInDir(const char *path, bool clobber = false);
    
    private:
        
    int fFilenameSize;
    char *fFilename;
    
    unsigned long long int fBufferSize;
    char *fBuffer;
    
    const char *GetBaseFile(const char *fp = NULL);
        
    ClassDef(qsimTextFile, 1);
        
};
#endif//__QSIMTEXTFILE_HH

