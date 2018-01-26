// local includes
#include "Utils.h"

namespace Utils {

  bool FindVariable(const TString & name, const std::vector<TString> & variables)
  {
    // exact search
    if (variables.size() == 0 || std::find(variables.begin(), variables.end(), name) != variables.end()) return true;
    // regular expression
    for (unsigned int i = 0; i < variables.size(); ++i) {
      const TString & item = variables.at(i);
      if ( std::regex_match(name.Data(), std::regex(item.Data())) ) {
	return true;
      } 
    }
    // not found
    return false;
  }

}
