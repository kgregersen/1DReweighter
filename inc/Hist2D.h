#ifndef __HIST2D__
#define __HIST2D__

// local includes
#include "HistBase.h"

// ROOT includes
#include "TH2F.h"


class Hist2D : public HistBase {

public:

  Hist2D();
  Hist2D(const TString & name, TH2F * hist);
  Hist2D(const TString & name, const TString & xaxis, const TString & yaxis, const TString & zaxis, int xbins, float xlow, float xhigh, int ybins, float ylow, float yhigh);
  ~Hist2D();

  virtual void Fill(const Event & event) {}
  void Divide(const HistBase * other);
  float Integral() const;
  void Scale(float scale);
  void Add(const HistBase * other, float scale = 1);
  const TH2F * Get() const { return m_hist; }
  HistBase * Clone() const;

  
protected:

  TH2F * m_hist;
   
};

#endif
