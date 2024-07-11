// K.Okawa modified <okawa@cns.s.u-tokyo.ac.jp>


#ifndef _TMODULEDECODERTIMESTAMP_h_
#define _TMODULEDECODERTIMESTAMP_h_

#include "TModuleDecoder.h"

namespace art {
  class TModuleDecoderTimestamp;
}

class art::TModuleDecoderTimestamp : public TModuleDecoder {
 public:
  static const int kID = 8;
  TModuleDecoderTimestamp();
  virtual ~TModuleDecoderTimestamp();
  virtual Int_t Decode(char* buffer, const Int_t &size, TObjArray *seg);

 protected:
  TObjArray *fHitData; // array to tomprally store the data for the aggregation

  ClassDef(TModuleDecoderTimestamp,0) // timestamp decoder
};
#endif // _TMODULEDECODERTIMESTAMP_h_
