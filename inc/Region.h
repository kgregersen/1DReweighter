#ifndef __REGION__
#define __REGION__

// ROOT includes
#include "TString.h"

// forward declarations
class Event;


class Region {
  
public:

  enum TYPE {
    NOREGION,
    INC,
    SR,
    CR,
    VR1,
    VR2,
    CRexclVR1,
    CRexclVR2,
    SRexclVR1,
    SRexclVR2
  };

  static TString String(TYPE r)
  {
    switch( r ) {
    case NOREGION  : return "NOREGION";
    case INC       : return "INC";
    case SR        : return "SR";
    case CR        : return "CR";
    case VR1       : return "VR1";
    case VR2       : return "VR2";
    case CRexclVR1 : return "CRexclVR1";
    case CRexclVR2 : return "CRexclVR2";
    case SRexclVR1 : return "SRexclVR1";
    case SRexclVR2 : return "SRexclVR2";
    }
    return "[REGION]";
  }

  static TString LatexString(TYPE r)
  {
    switch( r ) {
    case NOREGION  : return "NO REGION";
    case INC       : return "Inclusive Regions";
    case SR        : return "Signal Region";
    case CR        : return "Control Region";
    case VR1       : return "Validation Region 1";
    case VR2       : return "Validation Region 2";
    case CRexclVR1 : return "Control Region excl. Validation Region 1";
    case CRexclVR2 : return "Control Region excl. Validation Region 2";
    case SRexclVR1 : return "Signal Region excl. Validation Region 1";
    case SRexclVR2 : return "Signal Region excl. Validation Region 2";
    }
    return "[REGION]";
  }
  
  static bool SelectSR       (const Event & event);
  static bool SelectVR1      (const Event & event);
  static bool SelectVR2      (const Event & event);
  static bool SelectCR       (const Event & event);
  static bool SelectCRexclVR1(const Event & event);
  static bool SelectCRexclVR2(const Event & event);
  static bool SelectSRexclVR1(const Event & event);
  static bool SelectSRexclVR2(const Event & event);
  
};


#endif
