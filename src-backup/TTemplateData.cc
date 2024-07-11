// template .cc Data file for artemis

#include "TTemplateData.h"
#include <TConverterBase.h>

using art::TTemplateData;

ClassImp(art::TTemplateData);

TTemplateData::TTemplateData()
    : fVal(kInvalidD)
{
    TDataObject::SetID(kInvalidI);
}

TTemplateData::~TTemplateData() {}

TTemplateData::TTemplateData(const TTemplateData &rhs)
    : TDataObject(rhs),
      fVal(rhs.fVal)
{
}

TTemplateData &TTemplateData::operator=(const TTemplateData &rhs)
{
    if (this != &rhs)
    {
        ((TTemplateData &)rhs).Copy(*this);
    }
    return *this;
}

void TTemplateData::Copy(TObject &dest) const
{
    TDataObject::Copy(dest);
    TTemplateData &cobj = *(TTemplateData *)&dest;
    cobj.fVal = this->GetVal();
}

void TTemplateData::Clear(Option_t *opt)
{
    TDataObject::Clear(opt);
    TDataObject::SetID(kInvalidI);
    fVal = kInvalidD;
}
