#ifndef __EVENT__
#define __EVENT__

// local includes
#include "Region.h"


// ROOT includes
#include "TTree.h"
#include "TString.h"

// STL includes
#include <map>
#include <typeinfo>
#include <vector>


class Event {

public:
  
  class VarBase {
  public:
    VarBase() {}
    virtual ~VarBase() {}
    virtual TString Type() const = 0;
  };

  template<typename T>
  class Var : public VarBase {
  public:
    Var() : value() {}
    Var(T v) : value(v) {}
    TString Type() const { return typeid(value).name(); }
    T value;
  };

  Event() {}
  Event(Event & other);
  ~Event() {}
  
  void ConnectAllVariables(TTree * tree, const std::vector<TString> & variables, Region::TYPE weightsRegion = Region::NOREGION);
  void CalculateRegionVariables();
  void CalculateAdditionalVariables();
  void AddWeight(float w);
  TString GetType(const TString & name) const;

  template<typename T>
  void SetVar(const TString & name, const T & value);
  
  template <typename T>
  T & GetVar(const TString & name);

  template <typename T>
  const T & GetVar(const TString & name) const;

  
private:

  std::map<TString, VarBase *> varMap;
  
  template<typename T>
  void ConnectVariable(const TString & name, TTree * tree, const std::vector<TString> * variables = 0);
     
  struct Jet {
  public:
    Jet() : pt(0), eta(0), phi(0) {}
    Jet(float _pt, float _eta, float _phi) : pt(_pt), eta(_eta), phi(_phi) {}
    float pt;
    float eta;
    float phi;
  };
  
  static bool sort_pt(const Jet & j1, const Jet & j2) { return j1.pt > j2.pt; }
  
};

#include "Event.icc"

#endif
