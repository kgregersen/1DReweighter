//local includes
#include "HistCollection.h"
#include "Histograms.h"
#include "Event.h"
#include "TagBit.h"
#include "TrigBit.h"
#include "Reweighter.h"
#include "Utils.h"

// STL includes
#include <iostream>
#include <typeinfo>
#include <ctime>
#include <iomanip>


HistCollection::HistCollection(const TString & name) :
  m_name(name),
  m_select()
{

}


HistCollection::HistCollection(const TString              & name,
			       TagBit::TAGDEF               tagdef,
			       TagBit::WP                   wp,
			       Region::TYPE                 region,
			       TrigBit::TYPE                trigbit2016,
			       bool                         lepVeto,
			       bool                         xwtVeto,
			       const std::vector<TString> & variables) :
  m_name(name),
  m_select(tagdef, wp, region, trigbit2016, lepVeto, xwtVeto)
{
  
  m_histMap.clear();

  // HistInfo constructor arguments:
  // 1) TString              : name (picked up by Reweighter)
  // 2) std::vector<TString> : tree variables to be connected
  // 3) std::vector<TString> : fill variables to be present in Event 
  //                           --> if not in tree, this will trigger calculateAdditionalVariables = true
  //                           --> leave empty will disable the check
   
  // variables
  if ( Utils::FindVariable("hcand1_m"         , variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand1_m"         , {"hcand1_m"            }, {"hcand1_m"            }), new hcand1_m         ) );
  if ( Utils::FindVariable("hcand2_m"         , variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand2_m"         , {"hcand2_m"            }, {"hcand2_m"            }), new hcand2_m         ) );
  if ( Utils::FindVariable("hcand1_pt"        , variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand1_pt"        , {"hcand1_pt"           }, {"hcand1_pt"           }), new hcand1_pt        ) );
  if ( Utils::FindVariable("hcand2_pt"        , variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand2_pt"        , {"hcand2_pt"           }, {"hcand2_pt"           }), new hcand2_pt        ) );
  if ( Utils::FindVariable("jet1_pt"          , variables) ) m_histMap.insert( std::make_pair( HistInfo("jet1_pt"          , {"hcand[12]_jet[12]_.*"}, {"jet1_pt"             }), new jet1_pt          ) );
  if ( Utils::FindVariable("jet2_pt"          , variables) ) m_histMap.insert( std::make_pair( HistInfo("jet2_pt"          , {"hcand[12]_jet[12]_.*"}, {"jet2_pt"             }), new jet2_pt          ) );
  if ( Utils::FindVariable("jet3_pt"          , variables) ) m_histMap.insert( std::make_pair( HistInfo("jet3_pt"          , {"hcand[12]_jet[12]_.*"}, {"jet3_pt"             }), new jet3_pt          ) );
  if ( Utils::FindVariable("jet4_pt"          , variables) ) m_histMap.insert( std::make_pair( HistInfo("jet4_pt"          , {"hcand[12]_jet[12]_.*"}, {"jet4_pt"             }), new jet4_pt          ) );
  if ( Utils::FindVariable("hcand1_hcand2_m"  , variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand1_hcand2_m"  , {"hcand1_hcand2_m"     }, {"hcand1_hcand2_m"     }), new hcand1_hcand2_m  ) );
  if ( Utils::FindVariable("hcand1_hcand2_pt" , variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand1_hcand2_pt" , {"hcand1_hcand2_pt"    }, {"hcand1_hcand2_pt"    }), new hcand1_hcand2_pt ) );
  if ( Utils::FindVariable("hcand1_hcand2_dR" , variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand1_hcand2_dR" , {"hcand1_hcand2_dR"    }, {"hcand1_hcand2_dR"    }), new hcand1_hcand2_dR ) );
  if ( Utils::FindVariable("jet_n"            , variables) ) m_histMap.insert( std::make_pair( HistInfo("jet_n"            , {"jet_n"               }, {"jet_n"               }), new jet_n            ) );
  if ( Utils::FindVariable("avgAbsHCJetEta"   , variables) ) m_histMap.insert( std::make_pair( HistInfo("avgAbsHCJetEta"   , {"avgAbsHCJetEta"      }, {"avgAbsHCJetEta"      }), new avgAbsHCJetEta   ) );
  if ( Utils::FindVariable("MEt"              , variables) ) m_histMap.insert( std::make_pair( HistInfo("MEt"              , {"MEt"                 }, {"MEt"                 }), new MEt              ) );
  if ( Utils::FindVariable("MEt_phi"          , variables) ) m_histMap.insert( std::make_pair( HistInfo("MEt_phi"          , {"MEt_phi"             }, {"MEt_phi"             }), new MEt_phi          ) );
  if ( Utils::FindVariable("GCdR_min"         , variables) ) m_histMap.insert( std::make_pair( HistInfo("GCdR_min"         , {"GCdR_min"            }, {"GCdR_min"            }), new GCdR_min         ) );
  if ( Utils::FindVariable("GCdR_max"         , variables) ) m_histMap.insert( std::make_pair( HistInfo("GCdR_max"         , {"GCdR_max"            }, {"GCdR_max"            }), new GCdR_max         ) );
  if ( Utils::FindVariable("meffHC"           , variables) ) m_histMap.insert( std::make_pair( HistInfo("meffHC"           , {"meffHC"              }, {"meffHC"              }), new meffHC           ) );
  if ( Utils::FindVariable("hcand1_m_hcand2_m", variables) ) m_histMap.insert( std::make_pair( HistInfo("hcand1_m_hcand2_m", {"hcand[12]_m"         }, {"hcand1_m", "hcand2_m"}), new hcand1_m_hcand2_m) );
  if ( Utils::FindVariable("MEt_meffHC"       , variables) ) m_histMap.insert( std::make_pair( HistInfo("MEt_meffHC"       , {"MEt", "meffHC"       }, {"MEt", "meffHC"       }), new MEt_meffHC       ) );
  if ( Utils::FindVariable("Xt1"              , variables) ) m_histMap.insert( std::make_pair( HistInfo("Xt1"              , {"Xt1"                 }, {"Xt1"                 }), new Xt1              ) );
  if ( Utils::FindVariable("xhh_SR"           , variables) ) m_histMap.insert( std::make_pair( HistInfo("xhh_SR"           , {"xhh_SR"              }, {"xhh_SR"              }), new xhh_SR           ) );

  
  // triggers
  if ( Utils::FindVariable("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25", variables) ) {
    m_histMap.insert( std::make_pair( HistInfo("HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25",
					       {"HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25"},
					       {"HLT_2j35_bmv2c2060_split_2j35_L14J15.0ETA25"}),
				      new HLT_2j35_bmv2c2060_split_2j35_L14J15_0ETA25) );
  }
  if ( Utils::FindVariable("HLT_j100_2j55_bmv2c2060_split", variables) ) {
    m_histMap.insert( std::make_pair( HistInfo("HLT_j100_2j55_bmv2c2060_split",
					       {"HLT_j100_2j55_bmv2c2060_split"},
					       {"HLT_j100_2j55_bmv2c2060_split"}),
				      new HLT_j100_2j55_bmv2c2060_split) );
  }
  if ( Utils::FindVariable("HLT_j225_bmv2c2060_split", variables) ) {
    m_histMap.insert( std::make_pair( HistInfo("HLT_j225_bmv2c2060_split",
					       {"HLT_j225_bmv2c2060_split"},
					       {"HLT_j225_bmv2c2060_split"}),
				      new HLT_j225_bmv2c2060_split) );
  }
  if ( Utils::FindVariable("OR2016", variables) ) {
    m_histMap.insert( std::make_pair( HistInfo("OR2016",
					       {"HLT_.*"},
					       {""}),
				      new OR2016) );
  }
  
}


HistCollection::HistCollection(const HistCollection & other) :
  m_name(other.m_name),
  m_select(other.m_select)
{

  m_histMap.clear();
  std::map<HistInfo, HistBase *>::const_iterator it    = other.m_histMap.begin();
  std::map<HistInfo, HistBase *>::const_iterator itEnd = other.m_histMap.end();
  for ( ; it != itEnd; ++it) {
    const HistInfo & info = it->first;
    const HistBase * hist = it->second;
    m_histMap.insert( std::make_pair(info, hist->Clone()) );
  }
  
}


HistCollection::~HistCollection()
{

  std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
  std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
  for ( ; it != itEnd; ++it) {
    HistBase * hist = it->second;
    delete hist;
  }

  m_histMap.clear();

}


void HistCollection::Fill(TTree * tree, TREE_MODE treeMode, REW_MODE rewMode, const std::map<int, Reweighter> * rewFcns)
{

  // check if tree is valid
  if ( ! tree ) {
    std::cout << "HistCollection::Fill() : Error - TTree is nullptr " << std::endl;
    return;
  }

  // set weightsRegion used for connecting/reading tree branches for the weights
  Region::TYPE weightsRegion = Region::NOREGION;
  if ( rewMode == REW_READ || rewMode == REW_READ_MUONLY ) {
    if ( m_select.REGION() == Region::INC       ) weightsRegion = Region::CR;
    if ( m_select.REGION() == Region::SR        ) weightsRegion = Region::CR;
    if ( m_select.REGION() == Region::CR        ) weightsRegion = Region::CR;
    if ( m_select.REGION() == Region::VR1       ) weightsRegion = Region::CRexclVR1;
    if ( m_select.REGION() == Region::VR2       ) weightsRegion = Region::CRexclVR2;
    if ( m_select.REGION() == Region::CRexclVR1 ) weightsRegion = Region::CRexclVR1;
    if ( m_select.REGION() == Region::CRexclVR2 ) weightsRegion = Region::CRexclVR2;
    if ( m_select.REGION() == Region::SRexclVR1 ) weightsRegion = Region::CRexclVR1;
    if ( m_select.REGION() == Region::SRexclVR2 ) weightsRegion = Region::CRexclVR2;
  }

  // determine which tree variables to connect
  std::vector<TString> treeVariables;
  if ( treeMode == CONNECT_SUB ) {

    std::cout << "HistCollection::Fill() : Registrering variables" << std::endl;
    
    std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
    std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
    for ( ; it != itEnd; ++it) {
      
      const HistInfo & info = it->first;
      const std::vector<TString> & treeVars = info.TreeVariables();
      
      for (size_t i = 0; i < treeVars.size(); ++i) {
	if (std::find(treeVariables.begin(), treeVariables.end(), treeVars.at(i)) == treeVariables.end()){
	  treeVariables.push_back(treeVars.at(i));
	}
      }
      
    }
  }

  // define Event object and connect branches
  Event event;
  event.ConnectAllVariables( tree , treeVariables , weightsRegion );

  // check if we need to calcualte additional variables
  bool calculateAdditionalVariables = false;
  std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
  std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
  for ( ; it != itEnd; ++it) {
      
    const HistInfo & info = it->first;
    const std::vector<TString> & fillVars = info.FillVariables();
    
    for (unsigned int i = 0; i < fillVars.size(); ++i) {
      if ( fillVars.at(i).Length() == 0 ) continue;
      if ( fillVars.at(i) == "xhh_SR"   ) continue;
      if ( fillVars.at(i) == "xhh_VR1"  ) continue;
      if ( fillVars.at(i) == "xhh_VR2"  ) continue;
      if ( fillVars.at(i) == "rhh_CR"   ) continue;
      if ( ! tree->GetBranch( fillVars.at(i) ) ) {
	calculateAdditionalVariables = true;
      }
    }
    
  }
  std::cout << "HistCollection::Fill() : calculateAdditionalVariables = " << calculateAdditionalVariables << std::endl;
  
  // prepare for loop over tree entries
  long maxEvent = tree->GetEntries();
  long reportFrac = maxEvent/(maxEvent > 100000 ? 10 : 1) + 1;
  std::cout << "HistCollection::Fill() : Looping over events (" << tree->GetName() << ") : "  << maxEvent << std::endl;
  std::clock_t start = std::clock();

  // Loop over ree entries
  for (long ievent = 0; ievent < maxEvent; ++ievent) {

    // print progress
    if( ievent > 0 && ievent % reportFrac == 0 ) {
      double duration     = (std::clock() - start)/static_cast<double>(CLOCKS_PER_SEC);    
      double frequency    = static_cast<double>(ievent) / duration;
      double timeEstimate = static_cast<double>(maxEvent - ievent) / frequency;
      std::cout << "HistCollection::Fill() : ---> processed : " << std::setw(4) << 100*ievent/maxEvent << "\%  ---  frequency : " << std::setw(6) << static_cast<int>(frequency) << " events/sec  ---  time : " << std::setw(4) << static_cast<int>(duration) << " sec  ---  remaining time : " << std::setw(4) << static_cast<int>(timeEstimate) << " sec"<< std::endl; 
    }
    
    // get event
    tree->GetEntry( ievent );

    // calculate region variables
    event.CalculateRegionVariables();
    
    // calculate additional variables
    if (calculateAdditionalVariables) event.CalculateAdditionalVariables();

    // selection
    if ( ! m_select( event ) ) continue;

    // apply kinematic weights
    if ( rewMode == REW_READ || rewMode == REW_READ_MUONLY )  {

      if ( weightsRegion == Region::CR ) {
	event.AddWeight( event.GetVar<float>("muQCD_R1D_CR"    ) );
	if ( rewMode == REW_READ ) event.AddWeight( event.GetVar<float>("kinWeight_R1D_CR") );
	//std::cout << event.GetVar<float>("muQCD_R1D_CR") << " " << event.GetVar<float>("kinWeight_R1D_CR") << std::endl;	
      }
      else if ( weightsRegion == Region::CRexclVR1 ) {
	event.AddWeight( event.GetVar<float>("muQCD_R1D_CRexclVR1") );
	if ( rewMode == REW_READ ) event.AddWeight( event.GetVar<float>("kinWeight_R1D_CRexclVR1") );
      }
      else if ( weightsRegion == Region::CRexclVR2 ) {
	event.AddWeight( event.GetVar<float>("muQCD_R1D_CRexclVR2") );
	if ( rewMode == REW_READ ) event.AddWeight( event.GetVar<float>("kinWeight_R1D_CRexclVR2") );
      }
      else {
	std::cout << "HistCollection::Fill() : Error - weightsRegion not recognised : " << Region::String( weightsRegion ).Data() << std::endl;
      }

    }
    else if ( rewMode == REW_FCN && rewFcns ) {
      
      std::map<int, Reweighter>::const_iterator it    = rewFcns->begin();
      std::map<int, Reweighter>::const_iterator itEnd = rewFcns->end();
      for ( ; it != itEnd; ++it) {

	const Reweighter & rew  = it->second;
	if ( rew.Init() == false ) continue;
	
	const TString & xvar = rew.Xvar();
	const TString & yvar = rew.Yvar();

	float x = 0.;
	float y = 0.;

	if ( xvar.Length() > 0 ) {
	  if      ( event.GetType( xvar ) == typeid(float).name() ) x = event.GetVar<float>( xvar );
	  else if ( event.GetType( xvar ) == typeid(int  ).name() ) x = event.GetVar<int>  ( xvar );
	  else  {
	    std::cout << "HistCollection::Fill() : Error - Type(xvar) = " << event.GetType( xvar ) << std::endl;
	    throw(0);
	  }
	}
	if ( yvar.Length() > 0 ) {
	  if      ( event.GetType( yvar ) == typeid(float).name() ) y = event.GetVar<float>( yvar );
	  else if ( event.GetType( yvar ) == typeid(int  ).name() ) y = event.GetVar<int>  ( yvar );
	  else  {
	    std::cout << "HistCollection::Fill() : Error - Type(yvar) = " << event.GetType( yvar ) << std::endl;
	    throw(0);
	  }
	}

	float weight = rew.Eval( x , y );
	event.AddWeight( weight );

      }
      
    }
    
    // fill histograms
    std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
    std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
    for ( ; it != itEnd; ++it) {
      HistBase * hist = it->second;
      hist->Fill( event );
    }

  }

  // print out
  double duration  = (std::clock() - start)/static_cast<double>(CLOCKS_PER_SEC);    
  double frequency = static_cast<double>(maxEvent) / duration;
  std::cout << "HistCollection::Fill() : ---> processed :  100\%  ---  frequency : " << std::setw(6) << static_cast<int>(frequency) << " events/sec  ---  time : " << std::setw(4) << static_cast<int>(duration) << " sec  ---  remaining time :    0 sec"<< std::endl; 

}


float HistCollection::MuQCD(const HistCollection & numerator, const HistCollection & denominator)
{

  std::map<HistInfo, HistBase *>::const_iterator it = numerator.m_histMap.begin();
  const HistInfo & info    = it->first;
  const HistBase * histNum = it->second;
  const HistBase * histDen = denominator.Histogram( info.Name() );
  
  float muQCD = histNum->Integral() / histDen->Integral();

  std::cout << "HistCollection::MuQCD() : histNum->Integral() = " << histNum->Integral() << ", histDen->Integral() = " << histDen->Integral() << ", muQCD = " << muQCD << std::endl;

  return muQCD;

}


void HistCollection::Divide(const HistCollection & other)
{

  std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
  std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
  for ( ; it != itEnd; ++it) {

    const HistInfo & info      = it->first;
    HistBase       * hist      = it->second;
    const HistBase * histOther = other.Histogram( info.Name() );
    
    hist->Divide( histOther );

  }

}


void HistCollection::Scale(float scale)
{

  std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
  std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
  for ( ; it != itEnd; ++it) {
    HistBase * hist = it->second;
    hist->Scale( scale );
  }

}


void HistCollection::Add(const HistCollection & other, float scale)
{

  std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
  std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
  for ( ; it != itEnd; ++it) {
    
    const HistInfo & info      = it->first;
    HistBase       * hist      = it->second;
    const HistBase * histOther = other.Histogram( info.Name() );
    
    hist->Add( histOther , scale );
    
  }

}


void HistCollection::Renormalize(const std::map<HistInfo, float> & renormScale)
{

  std::map<HistInfo, HistBase *>::iterator it    = m_histMap.begin();
  std::map<HistInfo, HistBase *>::iterator itEnd = m_histMap.end();
  for ( ; it != itEnd; ++it) {
    
    const HistInfo & info = it->first;
    HistBase       * hist = it->second;

    std::map<HistInfo, float>::const_iterator itScale = renormScale.find( info );
    if ( itScale == renormScale.end() ) {
      std::cout << "HistCollection::Renormalize() : Error - couldn't find scale for " << info.Name().Data() << std::endl;
      throw(0);
    }

    float scale = itScale->second;    
    hist->Scale( scale );
    
  }
  
}


const HistBase * HistCollection::Histogram(const TString & name) const 
{

  const HistBase * hist = 0;
  
  std::map<HistInfo, HistBase *>::const_iterator it    = m_histMap.begin();
  std::map<HistInfo, HistBase *>::const_iterator itEnd = m_histMap.end();
  for ( ; it != itEnd; ++it) {
    
    const HistInfo & info = it->first;

    if (info.Name() == name) {
      hist = it->second;
      break;
    }

  }

  if ( ! hist ) {
    std::cout << "HistCollection::Histogram() : Error - couldn't find histogram : " << name.Data() << std::endl;
    throw(0);
  }

  return hist;
  
}


const std::map<HistInfo, HistBase *> & HistCollection::HistMap() const
{

  return m_histMap;

}


HistCollection HistCollection::Ratio(const HistCollection & numerator, const HistCollection & denominator)
{

  HistCollection ratio( numerator );
  ratio.Divide( denominator );
  
  return ratio;
  
}


std::map<HistInfo, float> * HistCollection::RenormScale(const HistCollection & numerator, const HistCollection & denominator)
{

  std::map<HistInfo, float> * renormScale = new std::map<HistInfo, float>();
    
  std::map<HistInfo, HistBase *>::const_iterator it    = numerator.HistMap().begin();
  std::map<HistInfo, HistBase *>::const_iterator itEnd = numerator.HistMap().end();
  for ( ; it != itEnd; ++it) {
    
    const HistInfo & info = it->first;
    HistBase       * num  = it->second;
    const HistBase * den  = denominator.Histogram( info.Name() );   
    
    float scale = num->Integral() / den->Integral();
    renormScale->insert( std::make_pair(info, scale) );
    
  }
  
  return renormScale;
  
}


Region::TYPE HistCollection::REGION() const
{

  return m_select.REGION();
  
}

TagBit::TAGDEF HistCollection::TAGDEF() const
{

  return m_select.TAGDEF();
  
}


TagBit::WP HistCollection::WP() const
{

  return m_select.WP();

}


TrigBit::TYPE HistCollection::TRIGBIT() const
{

  return m_select.TRIGBIT();

}
