#ifndef __HISTOGRAMS__
#define __HISTOGRAMS__

// local includes
#include "Hist1D.h"
#include "Hist2D.h"
#include "Event.h"


//--------------------------------------------------------------------------------------------------------------------
// Add new variables here:
//========================
// - Each variable is a class (which contains a histogram and knows how to fill it)
// - 1D histograms inherit from Hist1D, 2D histograms from Hist2D
// - Define the constructor and the Fill method as public methods
// - The constructor takes a number of arguments:
//   --> name (used by the Reweighter class to identify variable to reweight)
//   --> xaxis name (for plotting)
//   --> yaxis name (for plotting)
//   --> zaxis name (for plotting), only for 2D
//   --> number of x-bins
//   --> minimum x-value
//   --> maximum x-value
//   --> number of y-bins, only for 2D
//   --> minimum y-value, only for 2D
//   --> maximum y-value, only for 2D
//  - The Fill method accesses the Event object to get the variable
//    --> in case the variable doesn't exist in the Event class already
//        it should be added in Event::CalculateAdditionalVariables()
//        with Event::SetVar<type>(name,value)
//--------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------------------
// Leading Higgs mass
//--------------------------------------------------------------------------------------------------------------------
class hcand1_m : public Hist1D {
public:
  hcand1_m() : Hist1D("hcand1_m", "Leading Higgs mass [GeV]", "Number of events", 75, 50, 200) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand1_m") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Sub-leading Higgs mass
//--------------------------------------------------------------------------------------------------------------------
class hcand2_m : public Hist1D {
public:
  hcand2_m() : Hist1D("hcand2_m", "Sub-leading Higgs mass [GeV]", "Number of events", 75, 50, 200) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand2_m") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Leading Higgs Pt
//--------------------------------------------------------------------------------------------------------------------
class hcand1_pt : public Hist1D {
public:
  hcand1_pt() : Hist1D("hcand1_pt", "Leading Higgs p_{T} [GeV]", "Number of events", 100, 0, 600) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand1_pt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Sub-leading Higgs Pt
//--------------------------------------------------------------------------------------------------------------------
class hcand2_pt : public Hist1D {
public:
  hcand2_pt() : Hist1D("hcand2_pt", "Sub-leading Higgs p_{T} [GeV]", "Number of events", 100, 0, 600) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand2_pt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Leading jet Pt
//--------------------------------------------------------------------------------------------------------------------
class jet1_pt : public Hist1D {
public:
  jet1_pt() : Hist1D("jet1_pt", "Leading jet p_{T} [GeV]", "Number of events", 100, 0, 800) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("jet1_pt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Sub-leading jet Pt
//--------------------------------------------------------------------------------------------------------------------
class jet2_pt : public Hist1D {
public:
  jet2_pt() : Hist1D("jet2_pt", "Sub-leading jet p_{T} [GeV]", "Number of events", 100, 0, 600) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("jet2_pt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// 3th-leading jet Pt
//--------------------------------------------------------------------------------------------------------------------
class jet3_pt : public Hist1D {
public:
  jet3_pt() : Hist1D("jet3_pt", "3^{th} leading jet p_{T} [GeV]", "Number of events", 100, 30, 300) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("jet3_pt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// 4th-leading jet Pt
//--------------------------------------------------------------------------------------------------------------------
class jet4_pt : public Hist1D {
public:
  jet4_pt() : Hist1D("jet4_pt", "4th leading jet p_{T} [GeV]", "Number of events", 100, 30, 200) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("jet4_pt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Di-Higgs mass
//--------------------------------------------------------------------------------------------------------------------
class hcand1_hcand2_m : public Hist1D {
public:
  hcand1_hcand2_m() : Hist1D("hcand1_hcand2_m", "Di-Higgs mass [GeV]", "Number of events", 100, 100, 1600) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand1_hcand2_m") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Di-Higgs pt
//--------------------------------------------------------------------------------------------------------------------
class hcand1_hcand2_pt : public Hist1D {
public:
  hcand1_hcand2_pt() : Hist1D("hcand1_hcand2_pt", "Di-Higgs p_{T} [GeV]", "Number of events", 100, 0, 900) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand1_hcand2_pt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Delta R between Higgses
//--------------------------------------------------------------------------------------------------------------------
class hcand1_hcand2_dR : public Hist1D {
public:
  hcand1_hcand2_dR() : Hist1D("hcand1_hcand2_dR", "#Delta R(H_{1},H_{2})", "Number of events", 100, 0, 8) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand1_hcand2_dR") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Number of jets
//--------------------------------------------------------------------------------------------------------------------
class jet_n : public Hist1D {
public:
  jet_n() : Hist1D("jet_n", "Number of jets", "Number of events", 10, 4, 14) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<int>("jet_n") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Number of b-tagged jets
//--------------------------------------------------------------------------------------------------------------------
class bjet_n : public Hist1D {
public:
  bjet_n() : Hist1D("bjet_n", "Number of b-tagged jets", "Number of b-tagged events", 6, 2, 8) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<int>("bjet_n") , event.GetVar<float>("weight") ); }
};

//--------------------------------------------------------------------------------------------------------------------
// Missing Transverse momentum
//--------------------------------------------------------------------------------------------------------------------
class MEt : public Hist1D {
public:
  MEt() : Hist1D("MEt", "ME_{T} [GeV]", "Number of events", 200, 0, 1000) {}
  //MEt() : Hist1D("MEt", "ME_{T} [GeV]", "Number of events", 50, 0, 400) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("MEt") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Missing Transverse momentum Phi
//--------------------------------------------------------------------------------------------------------------------
class MEt_phi : public Hist1D {
public:
  MEt_phi() : Hist1D("MEt_phi", "ME_{T} #phi", "Number of events", 100, -3.14, 3.14) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("MEt_phi") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Effective mass (Higgs candidate jets)
//--------------------------------------------------------------------------------------------------------------------
class meffHC : public Hist1D {
public:
  meffHC() : Hist1D("meffHC", "m_{eff}^{b-jets} [GeV]", "Number of events", 200, 0, 2000) {}
  //meffHC() : Hist1D("meffHC", "m_{eff}^{b-jets} [GeV]", "Number of events", 50, 200, 1200) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("meffHC") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Effective mass
//--------------------------------------------------------------------------------------------------------------------
class meff : public Hist1D {
public:
  meff() : Hist1D("meff", "m_{eff} [GeV]", "Number of events", 200, 0, 2000) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("meff") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Xt1 (hadronic top hypothesis variable)
//--------------------------------------------------------------------------------------------------------------------
class Xt1 : public Hist1D {
public:
  Xt1() : Hist1D("Xt1", "X_{Wt}", "Number of events", 70, 0, 7) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("Xt1") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// xhh_SR (hadronic top hypothesis variable)
//--------------------------------------------------------------------------------------------------------------------
class xhh_SR : public Hist1D {
public:
  xhh_SR() : Hist1D("xhh_SR", "X_{hh}^{SR} [GeV]", "Number of events", 56, 0, 7) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("xhh_SR") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Average absolute value of jet eta (from Higgs candidate jets)
//--------------------------------------------------------------------------------------------------------------------
class avgAbsHCJetEta : public Hist1D {
public:
  avgAbsHCJetEta() : Hist1D("avgAbsHCJetEta", "Average |#eta_{jet}|", "Number of events", 50, 0, 4) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("avgAbsHCJetEta") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Maximum delta R between jets (from Higgs candidate jets)
//--------------------------------------------------------------------------------------------------------------------
class GCdR_max : public Hist1D {
public:
  GCdR_max() : Hist1D("GCdR_max", "Maximum #Delta R(j,j)", "Number of events", 100, 0, 6) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("GCdR_max") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// 'Other' (i.e. not max) delta R between jets (from Higgs candidate jets)
//--------------------------------------------------------------------------------------------------------------------
class GCdR_min : public Hist1D {
public:
  GCdR_min() : Hist1D("GCdR_min", "Minimum #Delta R(j,j)", "Number of events", 100, 0, 3) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("GCdR_min") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// trigger efficiency :   HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25
//--------------------------------------------------------------------------------------------------------------------
class HLT_2j35_bmv2c2060_split_2j35_L14J15_0ETA25 : public Hist1D {
public:
  HLT_2j35_bmv2c2060_split_2j35_L14J15_0ETA25() : Hist1D("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25", "HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25", "Number of events", 2, 0, 2) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<int>("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// trigger efficiency :   HLT_j100_2j55_bmv2c2060_split
//--------------------------------------------------------------------------------------------------------------------
class HLT_j100_2j55_bmv2c2060_split : public Hist1D {
public:
  HLT_j100_2j55_bmv2c2060_split() : Hist1D("HLT_j100_2j55_bmv2c2060_split", "HLT_j100_2j55_bmv2c2060_split", "Number of events", 2, 0, 2) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<int>("HLT_j100_2j55_bmv2c2060_split") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// trigger efficiency :   HLT_j225_bmv2c2060_split
//--------------------------------------------------------------------------------------------------------------------
class HLT_j225_bmv2c2060_split : public Hist1D {
public:
  HLT_j225_bmv2c2060_split() : Hist1D("HLT_j225_bmv2c2060_split", "HLT_j225_bmv2c2060_split", "Number of events", 2, 0, 2) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<int>("HLT_j225_bmv2c2060_split") , event.GetVar<float>("weight") ); }
};

//--------------------------------------------------------------------------------------------------------------------
// trigger efficiency :   OR2016
//--------------------------------------------------------------------------------------------------------------------
class OR2016 : public Hist1D {
public:
  OR2016() : Hist1D("OR2016", "OR2016", "Number of events", 2, 0, 2) {}
  void Fill(const Event & event)
  {
    if      ( event.GetVar<int>("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25") ) m_hist->Fill( 1.0 , event.GetVar<float>("weight") );
    else if ( event.GetVar<int>("HLT_j100_2j55_bmv2c2060_split"              ) ) m_hist->Fill( 1.0 , event.GetVar<float>("weight") );
    else if ( event.GetVar<int>("HLT_j225_bmv2c2060_split"                   ) ) m_hist->Fill( 1.0 , event.GetVar<float>("weight") );
    else m_hist->Fill( 0.0 , event.GetVar<float>("weight") );
  }
};




//--------------------------------------------------------------------------------------------------------------------
// Leading Higgs mass vs Sub-leading Higgs mass
//--------------------------------------------------------------------------------------------------------------------
class hcand1_m_hcand2_m : public Hist2D {
public:
  hcand1_m_hcand2_m() : Hist2D("hcand1_m_hcand2_m", "Leading Higgs mass [GeV]", "Sub-leading Higgs mass", "Number of events", 50, 0, 200, 50, 0, 200) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("hcand1_m") , event.GetVar<float>("hcand2_m") , event.GetVar<float>("weight") ); }
};


//--------------------------------------------------------------------------------------------------------------------
// Missing Transverse momentum vs effective mass (Higgs candidate jets)
//--------------------------------------------------------------------------------------------------------------------
class MEt_meffHC : public Hist2D {
public:
  MEt_meffHC() : Hist2D("MEt_meffHC", "ME_{T} [GeV]", "m_{eff} [GeV]", "Number of events", 200, 0, 1000, 200, 0, 2000) {}
  void Fill(const Event & event) { m_hist->Fill( event.GetVar<float>("MEt") , event.GetVar<float>("meffHC") , event.GetVar<float>("weight") ); }
};


#endif
