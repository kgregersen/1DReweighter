#ifndef __UTILS__
#define __UTILS__

// ROOT includes
#include "TString.h"

// STL includes
#include <vector>
#include <regex>



namespace Utils {

  
  bool FindVariable(const TString & name, const std::vector<TString> & variables);

}

#endif
