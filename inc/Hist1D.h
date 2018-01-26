#ifndef __HIST1D__
#define __HIST1D__

// local includes
#include "HistBase.h"

// ROOT includes
#include "TH1F.h"


class Hist1D : public HistBase {

public:

  Hist1D();
  Hist1D(const TString & name, TH1F * hist);
  Hist1D(const TString & name, const TString & xaxis, const TString & yaxis, int xbins, float xlow, float xhigh);
  ~Hist1D();

  virtual void Fill(const Event & event) {}
  void Divide(const HistBase * other);
  float Integral() const;
  Double_t IntegralAndError(float low, float high, Double_t & err) const;
  void Scale(float scale);
  void Add(const HistBase * other, float scale = 1);
  const TH1F * Get() const { return m_hist; }
  HistBase * Clone() const;
  
  
protected:

  TH1F * m_hist;
   
};

#endif
