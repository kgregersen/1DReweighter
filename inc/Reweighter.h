#ifndef __REWEIGHTER__
#define __REWEIGHTER__

// ROOT includes
#include "TString.h"

// forward declarations
class TGraphErrors;
class TF1;
class TSpline5;
class TH1F;
class TH2F;
class HistCollection;
class TString;
class HistCollection;
class Hist1D;
class Hist2D;


class Reweighter {

public:

  enum METHOD {
    NOMETHOD,
    HIST1D,
    HIST2D,
    LINFIT,
    SPLINE,
    SMOOTH,
    CUSTOM,
    HYBRIDCL
  };
  
  Reweighter(const TString & hname, const TString & xvar, const TString & yvar = "");
  Reweighter(const Reweighter & other);
  ~Reweighter();

  void Initialise(const HistCollection & histograms);
  void Initialise1D(const Hist1D * rHist1D);
  void Initialise2D(const Hist2D * rHist2D);

  float Eval(float x, float y = 0.) const;

  void SetHist1D(const TH1F * h);
  void SetHist2D(const TH2F * h);
  void SetSpline(const TH1F * h);
  void SetFit   (const TH1F * h);
  void SetSmooth(const TH1F * h);
  void SetCustom(const TH1F * h, float y_exp = 2.0, float x_exp = 3.0, float x_fac = 1.0);
  void SetHybrid(const TH1F * h, float fracLow1 = 0.0, float fracLow2 = 0.25, float fracHigh1 = 0.75, float fracHigh2 = 1.0, bool useExistingLinFit = false);
  
  METHOD Method() const { return m_method; }
  const TString & Xvar() const { return m_xvar; }
  const TString & Yvar() const { return m_yvar; }
  
  const TH1F         * GetHist1D () const { return m_hist1D  ; }
  const TH2F         * GetHist2D () const { return m_hist2D  ; }
  const TF1          * GetFit    () const { return m_linfit  ; }
  const TSpline5     * GetSpline () const { return m_spline  ; }
  const TGraphErrors * GetSmooth () const { return m_gSmooth ; }
  const TGraphErrors * GetCustom () const { return m_gCustom ; }
  const TGraphErrors * GetHybrid () const { return m_gHybrid ; }

  bool Init() const { return m_init; }

  
private:

  TGraphErrors * GetGraph(const TH1F * h);
  
  TH1F         * m_hist1D;
  TH2F         * m_hist2D;
  TF1          * m_linfit;
  TSpline5     * m_spline;
  TGraphErrors * m_gSmooth;
  TGraphErrors * m_gCustom;
  TGraphErrors * m_gHybrid;
  float          m_redFac;
  METHOD         m_method;
  TString        m_hname;
  TString        m_xvar;
  TString        m_yvar;
  bool           m_init;
  
};


#endif
