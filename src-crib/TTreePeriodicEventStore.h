// add okawa

#ifndef _TTREEPERIODICEVENTSTORE_H_
#define _TTREEPERIODICEVENTSTORE_H_

#include "TProcessor.h"
#include "IEventStore.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif


namespace art {
   class TTreePeriodicEventStore;
   class TConditionBit;
   class TEventHeader;
}

class TString;
class TFile;
class TTree;
class TList;

class art::TTreePeriodicEventStore  : public TProcessor, public IEventStore {

public:
   TTreePeriodicEventStore();
   ~TTreePeriodicEventStore();


   virtual void Init(TEventCollection *col);
   virtual void Process();

   Int_t GetRunNumber() const; 
   const char* GetRunName() const;
   
   
private:
   
   TString  fFileName;
   TString  fTreeName;
   TFile   *fFile; //!
   TTree   *fTree; //!
   TList   *fObjects; //!
   Long_t   fEventNum;
   Long_t   fMaxEventNum;
   Long_t   fTreeEventNum;
   Long_t   fCurrentNum;

   TEventHeader **fEventHeader; //!

#ifdef USE_MPI   
   Int_t fRankID; //!
   Int_t fNPE; //! number of pe
#endif   
   
   
   ClassDef(TTreePeriodicEventStore,1);
};
#endif // end of #ifdef _TTREEPERIODICEVENTSTORE_H_
