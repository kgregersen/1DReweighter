#ifndef __TRIGBIT__
#define __TRIGBIT__

// local includes
#include "Event.h"


class TrigBit {

  //
  // trigger bit definition
  //
  //  data 2016:
  //  b1 = HLT_2j35_bmv2c1060_split_2j35_L14J15_0ETA25
  //  b2 = HLT_j100_2j55_bmv2c1060_split
  //  b3 = HLT_j100_2j55_bmv2c1060_split
  //
  //        (2^2)  (2^1)  (2^0)
  // int  |  b3  |  b2  |  b1  |
  //----------------------------
  //   0  |   0  |   0  |   0  |
  //   1  |   0  |   0  |   1  |
  //   2  |   0  |   1  |   0  |
  //   3  |   0  |   1  |   1  |
  //   4  |   1  |   0  |   0  |
  //   5  |   1  |   0  |   1  |
  //   6  |   1  |   1  |   0  |
  //   7  |   1  |   1  |   1  |
  //  -1  : Logical OR between b1, b2, b3
  
public:

  enum TYPE {
    ALLPASS,
    OR2016,
    T1,
    T2,
    T3,
    B0,
    B1,
    B2,
    B3,
    B4,
    B5,
    B6,
    B7
  };

  static bool Pass2016(const Event & event, TYPE bit)
  {
    if ( bit == ALLPASS ) return true;
    int t1 = event.GetVar<int>( Name(T1) );
    int t2 = event.GetVar<int>( Name(T2) );
    int t3 = event.GetVar<int>( Name(T3) );
    if ( bit == OR2016 &&  (t3 == 1  ||  t2 == 1  ||  t1 == 1) ) return true;
    if ( bit == T1     &&   t1 == 1                            ) return true;
    if ( bit == T2     &&   t2 == 1                            ) return true;
    if ( bit == T3     &&   t3 == 1                            ) return true;
    if ( bit == B0     &&   t3 == 0  &&  t2 == 0  &&  t1 == 0  ) return true;
    if ( bit == B1     &&   t3 == 0  &&  t2 == 0  &&  t1 == 1  ) return true;
    if ( bit == B2     &&   t3 == 0  &&  t2 == 1  &&  t1 == 0  ) return true;
    if ( bit == B3     &&   t3 == 0  &&  t2 == 1  &&  t1 == 1  ) return true;
    if ( bit == B4     &&   t3 == 1  &&  t2 == 0  &&  t1 == 0  ) return true;
    if ( bit == B5     &&   t3 == 1  &&  t2 == 0  &&  t1 == 1  ) return true;
    if ( bit == B6     &&   t3 == 1  &&  t2 == 1  &&  t1 == 0  ) return true;
    if ( bit == B7     &&   t3 == 1  &&  t2 == 1  &&  t1 == 1  ) return true;
    return false;
  }

  static TString Name(TYPE bit)
  {
    if ( bit == ALLPASS ) return "ALLPASS";
    if ( bit == OR2016  ) return "OR2016";
    if ( bit == T1      ) return "HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25";
    if ( bit == T2      ) return "HLT_j100_2j55_bmv2c2060_split";
    if ( bit == T3      ) return "HLT_j225_bmv2c2060_split";
    if ( bit == B0      ) return "B0";
    if ( bit == B1      ) return "B1";
    if ( bit == B2      ) return "B2";
    if ( bit == B3      ) return "B3";
    if ( bit == B4      ) return "B4";
    if ( bit == B5      ) return "B5";
    if ( bit == B6      ) return "B6";
    if ( bit == B7      ) return "B7";
    return "NOT_FOUND";
  }
    
};

#endif
