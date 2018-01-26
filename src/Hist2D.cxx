// local includes
#include "Hist2D.h"

// STL includes
#include <iostream>


Hist2D::Hist2D() :
  HistBase(),
  m_hist(0)
{

}


Hist2D::Hist2D(const TString & name, TH2F * hist) :
  HistBase(name),
  m_hist(static_cast<TH2F *>(hist->Clone()))
{

}


Hist2D::Hist2D(const TString & name, const TString & xaxis, const TString & yaxis, const TString & zaxis, int xbins, float xlow, float xhigh, int ybins, float ylow, float yhigh) :
  HistBase(name),
  m_hist(0)
{

  m_hist = new TH2F(name, name, xbins, xlow, xhigh, ybins, ylow, yhigh);
  m_hist->SetDirectory(0);
  m_hist->GetXaxis()->SetTitle(xaxis);
  m_hist->GetYaxis()->SetTitle(yaxis);
  m_hist->GetZaxis()->SetTitle(zaxis);
  
}


Hist2D::~Hist2D()
{

  delete m_hist;
  
}


void Hist2D::Divide(const HistBase * other)
{

  m_hist->Divide( ((Hist2D *)other)->m_hist );
  
}


float Hist2D::Integral() const
{

  if ( m_hist ) return m_hist->Integral(0, m_hist->GetNbinsX() + 1, 0, m_hist->GetNbinsY() + 1);
  
  return 0.;

}


void Hist2D::Scale(float scale)
{

  if ( m_hist ) m_hist->Scale( scale );

}


void Hist2D::Add(const HistBase * other, float scale)
{

  const Hist2D * hOther = dynamic_cast<const Hist2D *>( other );
  if ( ! hOther ) {
    std::cout << "Hist2D::Add() : Error - the histogram to be added is not Hist2D" << std::endl;
    throw(0);
  }
  
  const TH2F * hist = hOther->m_hist;

  m_hist->Add( hist , scale );
  
}


HistBase * Hist2D::Clone() const
{

  return new Hist2D(m_name, m_hist);

}


