// local includes
#include "Hist1D.h"

// STL includes
#include <iostream>


Hist1D::Hist1D() :
  HistBase(),
  m_hist(0)
{

}


Hist1D::Hist1D(const TString & name, TH1F * hist) :
  HistBase(name),
  m_hist(static_cast<TH1F *>(hist->Clone()))
{

}


Hist1D::Hist1D(const TString & name, const TString & xaxis, const TString & yaxis, int xbins, float xlow, float xhigh) :
  HistBase(name),
  m_hist(0)
{

  m_hist = new TH1F(name, name, xbins, xlow, xhigh);
  m_hist->SetDirectory(0);
  m_hist->GetXaxis()->SetTitle(xaxis);
  m_hist->GetYaxis()->SetTitle(yaxis);

}


Hist1D::~Hist1D()
{

  delete m_hist;
  
}


void Hist1D::Divide(const HistBase * other)
{

  const Hist1D * hOther = dynamic_cast<const Hist1D *>( other );
  if ( ! hOther ) {
    std::cout << "Hist1D::Divide() : Error - denominator is not Hist1D" << std::endl;
    throw(0);
  }

  const float significance = 3.;
  
  const TH1F * hist = hOther->m_hist;
  for (int i = 0; i <= hist->GetNbinsX() + 1; ++i) {
    float num   = m_hist->GetBinContent( i );
    float num_e = m_hist->GetBinError( i );
    float den   = hist->GetBinContent( i );
    float den_e = hist->GetBinError( i );
    if ( num == 0. || den == 0. || num/num_e < significance || den/den_e < significance ) {
      m_hist->SetBinContent(i, 0.);
      m_hist->SetBinError(i, 0.);
    }
    else {
      m_hist->SetBinContent(i, num/den);
      m_hist->SetBinError(i, (num/den)*sqrt(pow(num_e/num, 2.) + pow(den_e/den, 2.)));

    }
  }

}


void Hist1D::Add(const HistBase * other, float scale)
{

  const Hist1D * hOther = dynamic_cast<const Hist1D *>( other );
  if ( ! hOther ) {
    std::cout << "Hist1D::Add() : Error - the histogram to be added is not Hist1D" << std::endl;
    throw(0);
  }
  
  const TH1F * hist = hOther->m_hist;

  m_hist->Add( hist , scale );
  
}


float Hist1D::Integral() const
{

  if ( m_hist ) return m_hist->Integral(0, m_hist->GetNbinsX() + 1);
  
  return 0.;

}


Double_t Hist1D::IntegralAndError(float low, float high, Double_t & err) const
{

  if ( ! m_hist ) {
    std::cout << "Hist1D::InteralAndError() : Error - the histogram is a null pointer" << std::endl;
    throw(0);
  }

  int binLow  = m_hist->FindBin( low );
  int binHigh = (high == -1 ? m_hist->GetNbinsX() + 1 : m_hist->FindBin( high ));
  
  return m_hist->IntegralAndError(binLow, binHigh, err);
 
}


void Hist1D::Scale(float scale)
{

  if ( m_hist ) m_hist->Scale( scale );

}


HistBase * Hist1D::Clone() const
{

  return new Hist1D(m_name, m_hist); 

}

