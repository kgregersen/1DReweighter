#ifndef __HISTINFO__
#define __HISTINFO__

// STL includes
#include <vector>

// ROOT includes
#include <TString.h>


class HistInfo {

public:

  HistInfo() : m_name(""), m_treeVariables(), m_fillVariables() {}
  HistInfo(const TString & name, const std::vector<TString> & treeVariables, const std::vector<TString> & fillVariables) : m_name(name), m_treeVariables(treeVariables), m_fillVariables(fillVariables) {}
  
  const TString & Name() const { return m_name; }
  const std::vector<TString> & TreeVariables() const { return m_treeVariables; }
  const std::vector<TString> & FillVariables() const { return m_fillVariables; }

  
private:
  
  TString m_name;
  std::vector<TString> m_treeVariables;
  std::vector<TString> m_fillVariables;

};

// provide specialisation of std::less used for inserting HistInfo as key in std::map
namespace std
{
  template<> struct less<HistInfo>
  {  
    bool operator()(const HistInfo & lhs, const HistInfo & rhs) const { return lhs.Name() < rhs.Name(); } 
  };
}

#endif
