// template .h Processor file for artemis
// this is example file that create a new branch from the created branch
// (not mapping processor)

#ifndef _TTEMPLATEPROCESSOR_H_
#define _TTEMPLATEPROCESSOR_H_

#include <TProcessor.h>

namespace art
{
    class TTemplateProcessor;
}

class TClonesArray;

class art::TTemplateProcessor : public TProcessor
{
public:
    TTemplateProcessor();
    virtual ~TTemplateProcessor();

    virtual void Init(TEventCollection *col);
    virtual void Process();

protected:
    TString fInputColName;
    TString fOutputColName;
    TClonesArray **fInData; //!
    TClonesArray *fOutData; //!

    Double_t fParameter;

private:
    TTemplateProcessor(const TTemplateProcessor &rhs);
    TTemplateProcessor &operator=(const TTemplateProcessor &rhs);

    ClassDef(TTemplateProcessor, 1)
};

#endif // end of #ifndef _TTEMPLATEPROCESSOR_H_
