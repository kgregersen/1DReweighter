#ifndef __HISTBASE__
#define __HISTBASE__

// ROOT includes
#include "TString.h"

// forward declarations
class Event;
class TH1;


class HistBase {

public:

  HistBase() : m_name("NONAME") {}
  HistBase(const TString & name) : m_name(name) {}
  virtual ~HistBase() {}

  virtual void Fill(const Event & event) = 0;
  virtual void Divide(const HistBase * other) = 0;
  virtual float Integral() const = 0;
  virtual Double_t IntegralAndError(float low, float high, Double_t & err) const { return 0; }
  virtual void Scale(float scale) = 0;
  virtual void Add(const HistBase * other, float scale) = 0;
  virtual HistBase * Clone() const = 0;
  
protected:

  const TString m_name;
  
};

#endif
