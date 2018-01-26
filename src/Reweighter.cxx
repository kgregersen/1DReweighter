// local includes
#include "Reweighter.h"
#include "HistCollection.h"
#include "Hist1D.h"
#include "Hist2D.h"

// ROOT includes
#include "TGraphErrors.h"
#include "TGraphSmooth.h"
#include "TF1.h"
#include "TSpline.h"
#include "TH1F.h"
#include "TString.h"

// STL includes
#include <iostream>
#include <vector>


Reweighter::Reweighter(const TString & hname, const TString & xvar, const TString & yvar) :
  m_hist1D(0),
  m_hist2D(0),
  m_linfit(0),
  m_spline(0),
  m_gSmooth(0),
  m_gCustom(0),
  m_gHybrid(0),
  m_redFac(1.),
  m_method(NOMETHOD),
  m_hname(hname),
  m_xvar(xvar),
  m_yvar(yvar),
  m_init(false)
{

}


Reweighter::Reweighter(const Reweighter & other) :
  m_hist1D ( other.m_hist1D  ? static_cast<TH1F         *>((other.m_hist1D )->Clone()) : 0 ),
  m_hist2D ( other.m_hist2D  ? static_cast<TH2F         *>((other.m_hist2D )->Clone()) : 0 ),
  m_linfit ( other.m_linfit  ? static_cast<TF1          *>((other.m_linfit )->Clone()) : 0 ),
  m_spline ( other.m_spline  ? static_cast<TSpline5     *>((other.m_spline )->Clone()) : 0 ),
  m_gSmooth( other.m_gSmooth ? static_cast<TGraphErrors *>((other.m_gSmooth)->Clone()) : 0 ),
  m_gCustom( other.m_gCustom ? static_cast<TGraphErrors *>((other.m_gCustom)->Clone()) : 0 ),
  m_gHybrid( other.m_gHybrid ? static_cast<TGraphErrors *>((other.m_gHybrid)->Clone()) : 0 ),
  m_redFac ( other.m_redFac ),
  m_method ( other.m_method ),
  m_hname  ( other.m_hname  ),
  m_xvar   ( other.m_xvar   ),
  m_yvar   ( other.m_yvar   ),
  m_init   ( other.m_init   )
{
  
  
}


Reweighter::~Reweighter()
{

}


void Reweighter::Initialise(const HistCollection & histograms)
{

  const HistBase * rHistBase = histograms.Histogram( m_hname );

  const Hist1D * rHist1D = dynamic_cast<const Hist1D *>( rHistBase );
  const Hist2D * rHist2D = dynamic_cast<const Hist2D *>( rHistBase );

  if      (   rHist1D && ! rHist2D ) Initialise1D( rHist1D );
  else if ( ! rHist1D &&   rHist2D ) Initialise2D( rHist2D );
  else {
    std::cout << "Reweighter::Initialise() : Error - couldn't determine histogram dimension!" << std::endl;
    throw(0);
  }
  
}


void Reweighter::Initialise2D(const Hist2D * rHist2D)
{

  const TH2F * rTH2F = rHist2D->Get();
  if ( ! rTH2F ) {
    std::cout << "Reweighter::Initialise2D() : Error - rTH2F is null!" << std::endl;
    return;
  }

  SetHist2D( rTH2F );
  m_method = HIST2D;
  m_init = true;
  
}

  
void Reweighter::Initialise1D(const Hist1D * rHist1D)
{

  const TH1F * rTH1F = rHist1D->Get();
  if ( ! rTH1F ) {
    std::cout << "Reweighter::Initialise2D() : Error - rTH1F is null!" << std::endl;
    return;
  }

  SetHist1D( rTH1F );
  SetFit   ( rTH1F );
  SetSpline( rTH1F );
  SetSmooth( rTH1F );

  if ( m_xvar == "jet_n" ) {
    m_method = HIST1D;
    SetCustom(rTH1F);
    SetHybrid(rTH1F, 0, 0.3, 0.7, 1.0, true);
  }
  else if ( m_xvar == "jet3_pt" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F, 2.0, 3.0, 10.0);
    SetHybrid(rTH1F, -1, -1, 0.3, 0.6, false);
  }
  else if ( m_xvar == "jet4_pt" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F, 2.0, 3.0, 10.0);
    SetHybrid(rTH1F, -1, -1, 0.3, 0.6, false);
  }
  else if ( m_xvar == "hcand1_m" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F, 2.0, 3.0, 10.0);
    SetHybrid(rTH1F, 0, 0.3, 0.7, 1.0, true);
  }
  else if ( m_xvar == "hcand2_m" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F, 2.0, 3.0, 10.0);
    SetHybrid(rTH1F, 0, 0.3, 0.7, 1.0, true);
  }
  else if ( m_xvar == "hcand1_pt" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F, 2.0, 3.0, 10.0);
    SetHybrid(rTH1F, -1, -1, 0.4, 0.75, false);
  }
  else if ( m_xvar == "hcand2_pt" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F, 2.0, 3.0, 10.0);
    SetHybrid(rTH1F, -1, -1, 0.4, 0.75, false);
  }
  else if ( m_xvar == "hcand1_hcand2_pt" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F, 2.0, 3.0, 10.0);
    SetHybrid(rTH1F, -1, -1, 0.4, 0.75, false);
  }
  else if ( m_xvar == "MEt" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F);
    SetHybrid(rTH1F, -1, -1, 0.4, 0.75, false);
  }
  else if ( m_xvar == "meffHC" ) {
    m_method = HYBRIDCL;
    SetCustom(rTH1F);
    SetHybrid(rTH1F, -1, -1, 0.4, 0.75, false);
  }
  else {
    m_method = CUSTOM;
    SetCustom(rTH1F);
    SetHybrid(rTH1F, 0, 0.3, 0.7, 1.0, true);
  }
        
  m_init = true;

}


TGraphErrors * Reweighter::GetGraph(const TH1F * h)
{
 
  TGraphErrors * ratio = new TGraphErrors;

  int p = 0;
  for (int i = 0; i <= h->GetNbinsX() + 1; ++i) {

    float x  = h->GetBinCenter(i);
    float y  = h->GetBinContent(i);
    float ey = h->GetBinError(i);

    if ( y == 0 ) continue; 
    
    ratio->SetPoint(p, x, y);
    ratio->SetPointError(p, 0., ey);
    ++p;
      
  }

  return ratio;

}


float Reweighter::Eval(float x, float y) const
{

  float w = 1.;

  if      (m_method == HIST1D   && m_hist1D  ) w = x < m_hist1D->GetXaxis()->GetXmin() ? m_hist1D->GetBinContent(1) : x > m_hist1D->GetXaxis()->GetXmax() ? m_hist1D->GetBinContent(m_hist1D->GetNbinsX()) : m_hist1D->GetBinContent(m_hist1D->FindBin(x));
  else if (m_method == LINFIT   && m_linfit  ) w = x < m_linfit->GetXmin() ? m_linfit->Eval(m_linfit->GetXmin()) : x > m_linfit->GetXmax() ? m_linfit->Eval(m_linfit->GetXmax()) : m_linfit->Eval(x);
  else if (m_method == SPLINE   && m_spline  ) w = m_spline->Eval(x);
  else if (m_method == SMOOTH   && m_gSmooth ) w = x < *(m_gSmooth->GetX()) ? *(m_gSmooth->GetY()) : x > *(m_gSmooth->GetX() + m_gSmooth->GetN() - 1) ? *(m_gSmooth->GetY() + m_gSmooth->GetN() - 1) : m_gSmooth->Eval(x);
  else if (m_method == CUSTOM   && m_gCustom ) w = x < *(m_gCustom->GetX()) ? *(m_gCustom->GetY()) : x > *(m_gCustom->GetX() + m_gCustom->GetN() - 1) ? *(m_gCustom->GetY() + m_gCustom->GetN() - 1) : m_gCustom->Eval(x);
  else if (m_method == HYBRIDCL && m_gHybrid ) w = x < *(m_gHybrid->GetX()) ? *(m_gHybrid->GetY()) : x > *(m_gHybrid->GetX() + m_gHybrid->GetN() - 1) ? *(m_gHybrid->GetY() + m_gHybrid->GetN() - 1) : m_gHybrid->Eval(x);
  else if (m_method == HIST2D   && m_hist2D  ) w = m_hist2D->GetBinContent( m_hist2D->GetXaxis()->FindBin(x) , m_hist2D->GetYaxis()->FindBin(y) );

  return w > 1. ? 1. + m_redFac*(w - 1.) : 1. - m_redFac*(1. - w);

}
  

void Reweighter::SetHist1D(const TH1F * h)
{

  m_hist1D = static_cast<TH1F*>(h->Clone());
  m_hist1D->SetDirectory(0);

}


void Reweighter::SetHist2D(const TH2F * h)
{

  m_hist2D = static_cast<TH2F*>(h->Clone());
  m_hist2D->SetDirectory(0);

}


void Reweighter::SetFit(const TH1F * h)
{

  TGraphErrors * ratio = GetGraph( h );
  
  m_linfit = new TF1("", "[0]*x+[1]", h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
  m_linfit->SetParameter(0, 0.);
  m_linfit->SetParameter(1, 1.);
  ratio->Fit(m_linfit, "QR0"); 

  delete ratio;
  
}


void Reweighter::SetSmooth(const TH1F * h)
{

  TGraph * ratio = static_cast<TGraph *>( GetGraph( h ) );

  TGraphSmooth gs;
  TGraph * graph = gs.SmoothLowess(ratio, "", 0.5, 10, 0);

  m_gSmooth = new TGraphErrors;
  
  for (int i = 0; i < graph->GetN(); ++i) {
    m_gSmooth->SetPoint(i, *(graph->GetX() + i), *(graph->GetY() + i));
    m_gSmooth->SetPointError(i, 0, 0);
  }    
    
  m_gSmooth->SetBit(TGraph::kIsSortedX);
  
  delete ratio;
  //delete graph; //No, it turns out we don't own this pointer...
  
}


void Reweighter::SetSpline(const TH1F * h)
{
    
  m_spline = new TSpline5( h );
  
}

void Reweighter::SetCustom(const TH1F * h, float y_exp, float x_exp, float x_fac)
{
    
  TGraphErrors * ratio = GetGraph( h );
  
  int np = ratio->GetN();
  Double_t * x_arr  = ratio->GetX();
  Double_t * y_arr  = ratio->GetY();
  Double_t * ye_arr = ratio->GetEY();
  float xUnit = 0.1*fabs(x_arr[np-1] - x_arr[0]);
  
  m_gCustom = new TGraphErrors;
  
  for (int k = 0; k < np; ++k) {

    float x = x_arr[k];
    float y = 0.;
    float sumW = 0.;

    for (int i = 0; i < np; ++i) {
      float dx = fabs(x - x_arr[i])/xUnit;
      float ye = ye_arr[i];
      sumW += ( 1./pow(ye, y_exp) ) * ( 1./exp(x_fac*pow(dx, x_exp)) );
    }

    for (int i = 0; i < np; ++i) {
      float dx  = fabs(x - x_arr[i])/xUnit;
      float ye  = ye_arr[i];
      y += y_arr[i] * ( 1./pow(ye, y_exp) ) * ( 1./exp(x_fac*pow(dx, x_exp)) ) / sumW;
    }
    
    m_gCustom->SetPoint(k, x, y);
    m_gCustom->SetPointError(k, 0, 0);
    
  }
  
  m_gCustom->SetBit(TGraph::kIsSortedX);

  delete ratio;
  
}


void Reweighter::SetHybrid(const TH1F * h, float fracLow1, float fracLow2, float fracHigh1, float fracHigh2, bool useExistingLinFit) 
{
  
  if ( ! m_gCustom ) SetCustom( h );
  if ( ! m_linfit  ) SetFit( h );

  TGraphErrors * ratio = GetGraph( h );
  
  m_gHybrid = new TGraphErrors;

  float xmin = h->GetXaxis()->GetXmin();
  float xmax = h->GetXaxis()->GetXmax();
  
  float low1  = fracLow1  < 0 ? xmin : *( m_gCustom->GetX() + (int)(fracLow1 *(m_gCustom->GetN() - 1)) );
  float low2  = fracLow2  < 0 ? xmin : *( m_gCustom->GetX() + (int)(fracLow2 *(m_gCustom->GetN() - 1)) );
  float high1 = fracHigh1 < 0 ? xmax : *( m_gCustom->GetX() + (int)(fracHigh1*(m_gCustom->GetN() - 1)) );
  float high2 = fracHigh2 < 0 ? xmax : *( m_gCustom->GetX() + (int)(fracHigh2*(m_gCustom->GetN() - 1)) );
  
  //std::cout << h->GetName() << " " << xmin << " " << xmax << "    " << low1 << " " << low2 << " " << high1 << " " << high2 << std::endl;

  std::vector<float> x_arr;
  
  TF1 * linfitLow = 0;
  if (fracLow1 >= 0 && fracLow2 >= 0 ) {
    if (useExistingLinFit) {
      linfitLow = m_linfit;
    }
    else {
      linfitLow = new TF1("linifitLow", "[0]*x+[1]", xmin, low2);
      linfitLow->SetParameter(0, 0.);
      linfitLow->SetParameter(1, 1.);
      ratio->Fit(linfitLow, "QR0");
    }
    x_arr.push_back(xmin);
  }
  
  
  for (int i = 0; i < ratio->GetN(); ++i) {
    x_arr.push_back( *(ratio->GetX() + i) );
  }
  
  TF1 * linfitHigh = 0;
  if (fracHigh1 >= 0 && fracHigh2 >= 0) {
    if (useExistingLinFit) {
      linfitHigh = m_linfit;
    }
    else {
      linfitHigh = new TF1("linifitHigh", "[0]*x+[1]", high1, xmax);
      linfitHigh->SetParameter(0, 0.);
      linfitHigh->SetParameter(1, 1.);
      ratio->Fit(linfitHigh, "QR0");
    }
    x_arr.push_back(xmax);
  }
  
  for (unsigned int k = 0; k < x_arr.size(); ++k) {
    
    float y = 1.;
    float x = x_arr.at(k);
    
    if (x < xmin) std::cout << "Reweighter::SetHybrid() : Error - x < xmin! x = " << x << "  xmin = " << xmin << std::endl;
    else if (x < low1) {
      if ( ! linfitLow ) {
	std::cout << "Reweighter::SetHybrid() : Error - linfitLow = null, but x = " << x << " xmin = " << xmin << " low1 = " << low1 << std::endl;
	continue;
      }
      y = linfitLow->Eval(x);
    }
    else if (x < low2) {
      if ( ! linfitLow ) {
	std::cout << "Reweighter::SetHybrid() : Error - linfitLow = null, but x = " << x << " low1 = " << low1 << " low2 = " << low2 << std::endl;
	continue;
      }
      float xfrac = (x - low1)/(low2 - low1);
      y = xfrac*m_gCustom->Eval(x) + (1. - xfrac)*linfitLow->Eval(x);
    }
    else if (x <= high1) {
      y = m_gCustom->Eval(x);
    }
    else if (x <= high2) {
      if ( ! linfitHigh ) {
	std::cout << "Reweighter::SetHybrid() : Error - linfitHigh = null, but x = " << x << " high1 = " << high1 << " high2 = " << high2 << std::endl;
	continue;
      }
      float xfrac  = (x - high1)/(high2 - high1);
      y = (1. - xfrac)*m_gCustom->Eval(x) + xfrac*linfitHigh->Eval(x);
    } 
    else if (x <= xmax) {
      if ( ! linfitHigh ) {
	std::cout << "Reweighter::SetHybrid() : Error - linfitHigh = null, but x = " << x << " high2 = " << high2 << " xmax = " << xmax << std::endl;
	continue;
      }
      y = linfitHigh->Eval(x);
    }
    else std::cout << "Reweighter::SetHybrid() : Error - x > xmax! x = " << x << "  xmax = " << xmax << std::endl;
    
    m_gHybrid->SetPoint(k, x, y);
    m_gHybrid->SetPointError(k, 0, 0);
    
  }   
  
  m_gHybrid->SetBit(TGraph::kIsSortedX);

  delete ratio;
  
  if ( ! useExistingLinFit ) {
    delete linfitLow;
    delete linfitHigh;
  }
  
}
