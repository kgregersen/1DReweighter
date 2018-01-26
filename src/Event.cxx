// local includes
#include "Event.h"
#include "Region.h"
#include "Utils.h"

// STL includes
#include <algorithm>
#include <vector>


Event::Event(Event & other)
{

  std::map<TString, VarBase *>::iterator it    = other.varMap.begin();
  std::map<TString, VarBase *>::iterator itEnd = other.varMap.end();
  for ( ; it != itEnd; ++it) {
    const TString & name = it->first;
    VarBase * var = it->second;
    varMap.insert( std::make_pair(name, var) );
  }
  
}

void Event::ConnectAllVariables(TTree * tree, const std::vector<TString> & variables, Region::TYPE weightsRegion)
{

  if ( variables.size() > 0 ) {
    std::cout << "Event::ConnectAllVariables() : Disabling all variables" << std::endl;
    tree->SetBranchStatus("*",0);
  }
  
  // always connect these variables
  ConnectVariable<float>("weight"                                      , tree );
  ConnectVariable<float>("hcand1_m"                                    , tree );
  ConnectVariable<float>("hcand2_m"                                    , tree );
  ConnectVariable<int>  ("eff70TagBits"                                , tree );
  ConnectVariable<int>  ("eff50TagBits"                                , tree );
  ConnectVariable<int>  ("eff30TagBits"                                , tree );
  ConnectVariable<float>("Xt1"                                         , tree );
  ConnectVariable<int>  ("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25" , tree );
  ConnectVariable<int>  ("HLT_j100_2j55_bmv2c2060_split"               , tree );
  ConnectVariable<int>  ("HLT_j225_bmv2c2060_split"                    , tree );
  ConnectVariable<int>  ("jet_n"                                       , tree );
  //ConnectVariable<int>  ("bjet_n"                                      , tree );
  ConnectVariable<int>  ("electron_n"                                  , tree );
  ConnectVariable<float>("electron_veto_pt"                            , tree );
  ConnectVariable<int>  ("muon_n"                                      , tree );
  ConnectVariable<float>("muon_veto_pt"                                , tree );

  // only connect these variables if needed
  ConnectVariable<float>("hcand1_jet1_pt"        , tree, &variables );
  ConnectVariable<float>("hcand1_jet1_eta"       , tree, &variables );
  ConnectVariable<float>("hcand1_jet1_phi"       , tree, &variables );
  ConnectVariable<float>("hcand1_jet2_pt"        , tree, &variables );
  ConnectVariable<float>("hcand1_jet2_eta"       , tree, &variables );
  ConnectVariable<float>("hcand1_jet2_phi"       , tree, &variables );
  ConnectVariable<float>("hcand2_jet1_pt"        , tree, &variables );
  ConnectVariable<float>("hcand2_jet1_eta"       , tree, &variables );
  ConnectVariable<float>("hcand2_jet1_phi"       , tree, &variables );
  ConnectVariable<float>("hcand2_jet2_pt"        , tree, &variables );
  ConnectVariable<float>("hcand2_jet2_eta"       , tree, &variables );
  ConnectVariable<float>("hcand2_jet2_phi"       , tree, &variables );
  ConnectVariable<float>("MEt"                   , tree, &variables );
  ConnectVariable<float>("MEt_phi"               , tree, &variables );
  ConnectVariable<float>("Xhh"                   , tree, &variables );
  ConnectVariable<float>("Dhh"                   , tree, &variables );
  ConnectVariable<float>("Rhh"                   , tree, &variables );
  ConnectVariable<float>("GCdR_min"              , tree, &variables );
  ConnectVariable<float>("GCdR_max"              , tree, &variables );
  ConnectVariable<float>("GCdR_diff"             , tree, &variables );
  ConnectVariable<float>("GCdR_sum"              , tree, &variables );
  ConnectVariable<float>("avgAbsHCJetEta"        , tree, &variables );
  ConnectVariable<float>("hcand1_hcand2_m"       , tree, &variables );
  ConnectVariable<float>("hcand1_hcand2_scaledM" , tree, &variables );
  ConnectVariable<float>("hcand1_hcand2_pt"      , tree, &variables );
  ConnectVariable<float>("hcand1_hcand2_dR"      , tree, &variables );
  ConnectVariable<float>("hcand1_pt"             , tree, &variables );
  ConnectVariable<float>("hcand1_dRjj"           , tree, &variables );
  ConnectVariable<float>("hcand2_pt"             , tree, &variables );
  ConnectVariable<float>("hcand2_dRjj"           , tree, &variables );
  ConnectVariable<float>("meff"                  , tree, &variables );
  ConnectVariable<float>("meffHC"                , tree, &variables );
  ConnectVariable<float>("mtbmin"                , tree, &variables );
  ConnectVariable<float>("xhh"                   , tree, &variables );
  
  // connect reweighting variables
  if ( weightsRegion == Region::CR ) {
    ConnectVariable<float>("muQCD_R1D_CR"    , tree);
    ConnectVariable<float>("kinWeight_R1D_CR", tree);
  }
  else if ( weightsRegion == Region::CRexclVR1 ) {
    ConnectVariable<float>("muQCD_R1D_CRexclVR1"    , tree);
    ConnectVariable<float>("kinWeight_R1D_CRexclVR1", tree);
  }
  else if ( weightsRegion == Region::CRexclVR2 ) {
    ConnectVariable<float>("muQCD_R1D_CRexclVR2"    , tree);
    ConnectVariable<float>("kinWeight_R1D_CRexclVR2", tree);
  }

}


void Event::CalculateRegionVariables()
{

  float hcand1_m = GetVar<float>("hcand1_m");
  float hcand2_m = GetVar<float>("hcand2_m");

  float m1_SR    = 120.0;
  float m2_SR    = 110.0;
  float Xhh_SR   = sqrt( pow((hcand1_m - m1_SR)/(0.1*hcand1_m), 2.) + pow((hcand2_m - m2_SR)/(0.1*hcand2_m), 2.) );

  float m1_VR1   = 96.0;
  float m2_VR1   = 88.0;
  float Xhh_VR1  = sqrt( pow((hcand1_m - m1_VR1)/(0.1*hcand1_m), 2.) + pow((hcand2_m - m2_VR1)/(0.1*hcand2_m), 2.) );

  float m1_VR2   = 149.0;
  float m2_VR2   = 137.0;
  float Xhh_VR2  = sqrt( pow((hcand1_m - m1_VR2)/(0.1*hcand1_m), 2.) + pow((hcand2_m - m2_VR2)/(0.1*hcand2_m), 2.) );

  float m1_CR    = 126.0;
  float m2_CR    = 115.5;
  float Rhh_CR   = sqrt( pow(hcand1_m - m1_CR, 2.) + pow(hcand2_m - m2_CR, 2.) );
  
  SetVar<float>("xhh_SR" , Xhh_SR );
  SetVar<float>("xhh_VR1", Xhh_VR1);
  SetVar<float>("xhh_VR2", Xhh_VR2);
  SetVar<float>("rhh_CR" , Rhh_CR );
  
}


void Event::CalculateAdditionalVariables()
{
  
  std::vector<Jet> jets;
  jets.push_back( Jet( GetVar<float>("hcand1_jet1_pt"), GetVar<float>("hcand1_jet1_eta"), GetVar<float>("hcand1_jet1_phi") ) );
  jets.push_back( Jet( GetVar<float>("hcand1_jet2_pt"), GetVar<float>("hcand1_jet2_eta"), GetVar<float>("hcand1_jet2_phi") ) );
  jets.push_back( Jet( GetVar<float>("hcand2_jet1_pt"), GetVar<float>("hcand2_jet1_eta"), GetVar<float>("hcand1_jet1_phi") ) );
  jets.push_back( Jet( GetVar<float>("hcand2_jet2_pt"), GetVar<float>("hcand2_jet2_eta"), GetVar<float>("hcand1_jet2_phi") ) );
  std::sort(jets.begin(), jets.end(), sort_pt);
  
  SetVar<float>("jet1_pt" , jets.at(0).pt );
  SetVar<float>("jet2_pt" , jets.at(1).pt );
  SetVar<float>("jet3_pt" , jets.at(2).pt );
  SetVar<float>("jet4_pt" , jets.at(3).pt );
  SetVar<float>("jet1_eta", jets.at(0).eta);
  SetVar<float>("jet2_eta", jets.at(1).eta);
  SetVar<float>("jet3_eta", jets.at(2).eta);
  SetVar<float>("jet4_eta", jets.at(3).eta);
  SetVar<float>("jet1_phi", jets.at(0).phi);
  SetVar<float>("jet2_phi", jets.at(1).phi);
  SetVar<float>("jet3_phi", jets.at(2).phi);
  SetVar<float>("jet4_phi", jets.at(3).phi);
  
}


void Event::AddWeight(float w)
{
  
  float & weight = GetVar<float>("weight");
  weight *= w;
  
}


TString Event::GetType(const TString & name) const
{

  std::map<TString, VarBase *>::const_iterator it = varMap.find( name );
  if (it == varMap.end()) {
    std::cout << "Event::GetType() : Error - " << name.Data() << " is not in map!" << std::endl;
    throw(0);
  }

  const VarBase * var = it->second;
  
  return var->Type();  

}
