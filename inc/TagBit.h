#ifndef __TAGBIT__
#define __TAGBIT__

// local includes
#include "Event.h"


class TagBit {

  //
  // b-tag bit definition
  //
  //        (2^3)  (2^2)  (2^1)  (2^0)
  // int  | H2J2 | H2J1 | H1J2 | H1J1 |     comment
  //-----------------------------------------------------
  //   1  |   0  |   0  |   0  |   1  | -- 1-tag --
  //   2  |   0  |   0  |   1  |   0  | -- 1-tag --
  //   3  |   0  |   0  |   1  |   1  | H1 2-tag 
  //   4  |   0  |   1  |   0  |   0  | -- 1-tag --
  //   5  |   0  |   1  |   0  |   1  | 2-tag split
  //   6  |   0  |   1  |   1  |   0  | 2-tag split
  //   7  |   0  |   1  |   1  |   1  | 3-tag
  //   8  |   1  |   0  |   0  |   0  | -- 1-tag --
  //   9  |   1  |   0  |   0  |   1  | 2-tag split
  //  10  |   1  |   0  |   1  |   0  | 2-tag split
  //  11  |   1  |   0  |   1  |   1  | 3-tag
  //  12  |   1  |   1  |   0  |   0  | H2 2-tag
  //  13  |   1  |   1  |   0  |   1  | 3-tag
  //  14  |   1  |   1  |   1  |   0  | 3-tag
  //  15  |   1  |   1  |   1  |   1  | 4-tag
  
public:

  enum WP {
    NOWP,
    WP30,
    WP50,
    WP60,
    WP70,
    WP77,
    WP85
  };
  
  enum TAGDEF {
    NOTAGDEF,
    TAG4,
    TAG3,
    TAG2,
    TAG2SPLIT,
    TAG2NONSPLIT    
  };

  static int GetBit(const Event & event, WP wp)
  {
    if ( wp == WP30 ) return event.GetVar<int>("eff30TagBits");
    if ( wp == WP50 ) return event.GetVar<int>("eff50TagBits");
    if ( wp == WP70 ) return event.GetVar<int>("eff70TagBits");
    return 0;
  }
  
  static bool Select4Tag(const Event & event, WP wp)
  {
    int bit = GetBit(event, wp);
    return ( bit == 15 );
  }

  static bool Select3Tag(const Event & event, WP wp)
  {
    int bit = GetBit(event, wp);
    return ( bit == 7 || bit == 11 || bit == 13 || bit == 14 );
  }

  static bool Select2Tag(const Event & event, WP wp)
  {
    int bit = GetBit(event, wp);
    return ( bit == 3 || bit == 5 || bit == 6 || bit == 9 || bit == 10 || bit == 12);
  }

  static bool Select2TagSplit(const Event & event, WP wp)
  {
    int bit = GetBit(event, wp);
    return ( bit == 5 || bit == 6 || bit == 9 || bit == 10 );
  }

  static bool Select2TagLeadHiggs(const Event & event, WP wp)
  {
    int bit = GetBit(event, wp);
    return ( bit == 3);
  }

  static bool Select2TagSubLHiggs(const Event & event, WP wp)
  {
    int bit = GetBit(event, wp);
    return ( bit == 12 );
  }

  static bool Select2TagNonSplit(const Event & event, WP wp)
  {
    int bit = GetBit(event, wp);
    return ( bit == 3 || bit == 12 );
  }
  
};

#endif
