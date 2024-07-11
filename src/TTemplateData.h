// template .h Data file for artemis

#ifndef _TTEMPLATEDATA_H_
#define _TTEMPLATEDATA_H_

#include "TDataObject.h"
#include "constant.h"

namespace art
{
    class TTemplateData;
}

class art::TTemplateData : public TDataObject
{
public:
    typedef enum
    {
        kID,
        kTiming
    } ESortType;
    typedef enum
    {
        kASC,
        kDESC
    } ESortOrder;

    TTemplateData();
    virtual ~TTemplateData();
    TTemplateData(const TTemplateData &rhs);
    TTemplateData &operator=(const TTemplateData &rhs);

    // getter and setter of data member
    Double_t GetVal() const { return fVal; }
    void SetVal(Double_t arg) { fVal = arg; }

    virtual void Copy(TObject &dest) const;
    virtual void Clear(Option_t *opt = "");

protected:
    Double_t fVal;

    ClassDef(TTemplateData, 1)
};

#endif // end of #ifndef _TTEMPLATEDATA_H_
