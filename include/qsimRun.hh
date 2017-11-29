#ifndef __QSIMRUN_HH
#define __QSIMRUN_HH

/*
 * All information on the run
 * Data will be sent to output stream
 * Adapted from qsimRun.hh
 */

#include "qsimRunData.hh"

#include "G4Run.hh"

class qsimRunData;

class qsimRun: public G4Run {

  public:
    qsimRun();
    virtual ~qsimRun();

  // Static run data access
  private:
    static qsimRunData* fRunData;
  public:
    static qsimRunData* GetRunData();
};

#endif//__QSIMRUN_HH
