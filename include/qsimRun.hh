#ifndef __QSIMRUN_HH
#define __QSIMRUN_HH

/*
 * All information on the run
 * Data will be sent to output stream
 * Adapted from remollRun.hh
 */

#include "qsimRunData.hh"

class qsimRun {

    private:
        static qsimRun *gSingleton;
        qsimRun();

        qsimRunData *fRunData;

    public:
        static qsimRun *GetRun();
        ~qsimRun();

        qsimRunData *GetData() {
            return fRunData;
        }
};

#endif//__QSIMRUN_HH
