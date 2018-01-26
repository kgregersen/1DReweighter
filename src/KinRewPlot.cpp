// local includes
#include "HistCollection.h"
#include "Event.h"
#include "TagBit.h"
#include "TrigBit.h"
#include "Region.h"
#include "Hist1D.h"
#include "Hist2D.h"

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TH1F.h"
#include "TH2F.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TGraphErrors.h"

// STL includes
#include <iostream>
#include <map>
#include <utility>
#include <sys/stat.h>


//
// forward declarations
//
void Plot(const HistCollection & h_2tag_data,
	  const HistCollection & h_2tag_ttLep,
	  const HistCollection & h_2tag_ttHad,
	  const HistCollection & h_2tag_Zvv,
	  const HistCollection & h_4tag_data,
	  const HistCollection & h_4tag_ttLep,
	  const HistCollection & h_4tag_ttHad,
	  const HistCollection & h_4tag_Zvv,
	  const HistCollection & h_4tag_HinoM150,
	  const HistCollection & h_4tag_HinoM300,
	  TString reg,
	  TString OUTPREFIX,
	  TString OUTPOSTFIX);


//
// main program
//
int main(int argc, char * argv[])
{


  //========================================================================================================
  //----------------------------->>>>>>>>>>      SETTINGS       <<<<<<<<<<----------------------------------
  //========================================================================================================

  //TFile * inputFile = new TFile("../files/Processed_EventAnaResults_resolved_171104_Copy_partiallyDataDriven2.root", "read");
  TFile * inputFile = new TFile("../files/Processed_EventAnaResults_resolved_171104_Copy_fullyDataDriven2.root", "read");

  TString tree_ext = ";4";

  // Output directory for plots
  //TString OUTDIR = "plots_partiallyDataDriven/Input171104";
  TString OUTDIR = "plots_fullyDataDriven/Input171104";
  
  // Switch for using MC
  //bool useMC = true;
  bool useMC = false;

  // Regions to plot
  std::vector<Region::TYPE> regions;
  regions.push_back( Region::CR        );
  // regions.push_back( Region::CRexclVR1 );
  // regions.push_back( Region::CRexclVR2 );
  regions.push_back( Region::SR        );
  // regions.push_back( Region::VR1       );
  // regions.push_back( Region::VR2       );
  // regions.push_back( Region::SRexclVR1 );
  // regions.push_back( Region::SRexclVR2 );

  // variables to plot
  std::vector<TString> variables = {"jet_n"             ,
				    "bjet_n"            ,
				    "jet1_pt"           ,
				    "jet2_pt"           ,
				    "jet3_pt"           ,
				    "jet4_pt"           ,
				    "avgAbsHCJetEta"    ,
				    "GCdR_min"          ,
				    "GCdR_max"          ,
				    "hcand1_pt"         ,
				    "hcand2_pt"         ,
				    "hcand1_hcand2_pt"  ,
				    "hcand1_hcand2_m"   ,
				    "hcand1_hcand2_dR"  ,
				    "meffHC"            ,
				    "meff"              ,
				    "MEt"               ,
				    "MEt_phi"           ,
				    "hcand1_m"          ,
				    "hcand2_m"          ,
				    "hcand1_m_hcand2_m" ,
				    "MEt_meffHC"        };

  //========================================================================================================
  //--------------------------------------------------------------------------------------------------------
  //========================================================================================================

  
  std::vector<TString> vecOUTDIR;
  
  for (unsigned int r = 0; r< regions.size(); ++r) {  
    vecOUTDIR.push_back( TString::Format("%s/%s", OUTDIR.Data(), Region::String(regions.at(r)).Data()) );
    const char * dirname = vecOUTDIR.back().Data();
    struct stat dir;
    char command[500];
    if (stat(dirname,&dir) != 0) {
      std::cout << "main() : Creating directories ./" << dirname << std::endl;
      sprintf(command,"mkdir -p %s/H1D/eps",dirname);
      system(command);
      sprintf(command,"mkdir -p %s/H1D/pdf",dirname);
      system(command);
      sprintf(command,"mkdir -p %s/H1D/root",dirname);
      system(command);
      sprintf(command,"mkdir -p %s/H2D/eps",dirname);
      system(command);
      sprintf(command,"mkdir -p %s/H2D/pdf",dirname);
      system(command);
      sprintf(command,"mkdir -p %s/H2D/root",dirname);
      system(command);
    }
    else {
      std::cout << "main() : Removing files in ./" << dirname << std::endl;
      sprintf(command,"rm %s/H1D/pdf/*.pdf 2> /dev/null",dirname);
      system(command);
      sprintf(command,"rm %s/H1D/eps/*.eps 2> /dev/null",dirname);
      system(command);     
      sprintf(command,"rm %s/H1D/root/*.root 2> /dev/null",dirname);
      system(command);
      sprintf(command,"rm %s/H2D/pdf/*.pdf 2> /dev/null",dirname);
      system(command);
      sprintf(command,"rm %s/H2D/eps/*.eps 2> /dev/null",dirname);
      system(command);     
      sprintf(command,"rm %s/H2D/root/*.root 2> /dev/null",dirname);
      system(command);     
    }
  } 
  

  // renormalise to match data as appropriate
  std::map<HistInfo, float> * renormScale = 0;
  std::map<HistInfo, float> * renormScale_muOnly = 0;
  std::map<HistInfo, float> * renormScale_CR = 0;
  std::map<HistInfo, float> * renormScale_CRexclVR1 = 0;
  std::map<HistInfo, float> * renormScale_CRexclVR2 = 0;
  std::map<HistInfo, float> * renormScale_muOnly_CR = 0;
  std::map<HistInfo, float> * renormScale_muOnly_CRexclVR1 = 0;
  std::map<HistInfo, float> * renormScale_muOnly_CRexclVR2 = 0;

  const int Nreg = regions.size();
  
  for (int reg = 0; reg < Nreg; ++reg) {  

    TTree * t_2tag_data     = static_cast<TTree *>( inputFile->Get( "data_SemiTagged"                             + tree_ext                ) );
    TTree * t_4tag_data     = static_cast<TTree *>( inputFile->Get( "data_FullyTagged"                                                      ) );
    TTree * t_2tag_ttLep    = static_cast<TTree *>( inputFile->Get( "TTBarNonAllHad_Nominal_SemiTagged"           + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_ttLep    = static_cast<TTree *>( inputFile->Get( "TTBarNonAllHad_Nominal_FullyTagged"                                    ) );
    TTree * t_2tag_ttHad    = static_cast<TTree *>( inputFile->Get( "TTBarAllHad_Nominal_SemiTagged"              + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_ttHad    = static_cast<TTree *>( inputFile->Get( "TTBarAllHad_Nominal_FullyTagged"                                       ) );
    TTree * t_2tag_Zvv_1    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt0_70_Nominal_SemiTagged"       + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_1    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt0_70_Nominal_FullyTagged"                                ) );
    TTree * t_2tag_Zvv_2    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt70_140_Nominal_SemiTagged"     + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_2    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt70_140_Nominal_FullyTagged"                              ) );
    TTree * t_2tag_Zvv_3    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt140_280_Nominal_SemiTagged"    + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_3    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt140_280_Nominal_FullyTagged"                             ) );
    TTree * t_2tag_Zvv_4    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt280_500_Nominal_SemiTagged"    + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_4    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt280_500_Nominal_FullyTagged"                             ) );
    TTree * t_2tag_Zvv_5    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt500_700_Nominal_SemiTagged"    + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_5    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt500_700_Nominal_FullyTagged"                             ) );
    TTree * t_2tag_Zvv_6    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt700_1000_Nominal_SemiTagged"   + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_6    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt700_1000_Nominal_FullyTagged"                            ) );
    TTree * t_2tag_Zvv_7    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt1000_2000_Nominal_SemiTagged"  + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_7    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt1000_2000_Nominal_FullyTagged"                           ) );
    TTree * t_2tag_Zvv_8    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt2000_E_CMS_Nominal_SemiTagged" + (useMC ? tree_ext : "") ) );
    TTree * t_4tag_Zvv_8    = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt2000_E_CMS_Nominal_FullyTagged"                          ) );
    TTree * t_4tag_HinoM130 = static_cast<TTree *>( inputFile->Get( "GGM_Hino_M130_Nominal_FullyTagged"                                     ) );
    TTree * t_4tag_HinoM150 = static_cast<TTree *>( inputFile->Get( "GGM_Hino_M150_Nominal_FullyTagged"                                     ) );
    TTree * t_4tag_HinoM200 = static_cast<TTree *>( inputFile->Get( "GGM_Hino_M200_Nominal_FullyTagged"                                     ) );
    TTree * t_4tag_HinoM300 = static_cast<TTree *>( inputFile->Get( "GGM_Hino_M300_Nominal_FullyTagged"                                     ) );
    TTree * t_4tag_HinoM400 = static_cast<TTree *>( inputFile->Get( "GGM_Hino_M400_Nominal_FullyTagged"                                     ) );
    TTree * t_4tag_HinoM600 = static_cast<TTree *>( inputFile->Get( "GGM_Hino_M600_Nominal_FullyTagged"                                     ) );
    TTree * t_4tag_HinoM800 = static_cast<TTree *>( inputFile->Get( "GGM_Hino_M800_Nominal_FullyTagged"                                     ) );
	
    std::cout << "main() : Region = " << Region::String( regions.at(reg) ) << " (" << reg+1 << "/" << Nreg << ")  " << std::endl; 

    TagBit::WP    WP      = TagBit::WP70;
    Region::TYPE  REG     = regions.at( reg );
    TrigBit::TYPE TRIG    = TrigBit::OR2016;
    bool          LepVeto = true;
    bool          XWtVeto = true;

    HistCollection h_4tag_data("h_4tag_data", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_data("h_2tag_data", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_data_mu_only("h_2tag_data_mu_only", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    h_4tag_data.Fill( t_4tag_data , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
    h_2tag_data.Fill( t_2tag_data , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
    h_2tag_data_mu_only.Fill( t_2tag_data , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );

    HistCollection h_4tag_ttLep("h_4tag_ttLep", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_ttLep("h_2tag_ttLep", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_ttLep_mu_only("h_2tag_ttLep_mu_only", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    if ( useMC ) {
      h_4tag_ttLep.Fill( t_4tag_ttLep , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_2tag_ttLep.Fill( t_2tag_ttLep , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_ttLep_mu_only.Fill( t_2tag_ttLep , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
    }
    
    HistCollection h_4tag_ttHad("h_4tag_ttHad", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_ttHad("h_2tag_ttHad", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_ttHad_mu_only("h_2tag_ttHad_mu_only", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    if ( useMC ) {
      h_4tag_ttHad.Fill( t_4tag_ttHad , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_2tag_ttHad.Fill( t_2tag_ttHad , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_ttHad_mu_only.Fill( t_2tag_ttHad , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
    }
    
    HistCollection h_4tag_Zvv("h_4tag_Zvv", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_Zvv("h_2tag_Zvv", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_2tag_Zvv_mu_only("h_2tag_Zvv_mu_only", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    if ( useMC ) {
      h_4tag_Zvv.Fill( t_4tag_Zvv_1 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_4tag_Zvv.Fill( t_4tag_Zvv_2 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_4tag_Zvv.Fill( t_4tag_Zvv_3 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_4tag_Zvv.Fill( t_4tag_Zvv_4 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_4tag_Zvv.Fill( t_4tag_Zvv_5 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_4tag_Zvv.Fill( t_4tag_Zvv_6 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_4tag_Zvv.Fill( t_4tag_Zvv_7 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_4tag_Zvv.Fill( t_4tag_Zvv_8 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF  , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_1 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_2 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_3 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_4 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_5 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_6 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_7 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv.Fill( t_2tag_Zvv_8 , HistCollection::CONNECT_SUB , HistCollection::REW_READ , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_1 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_2 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_3 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_4 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_5 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_6 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_7 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
      h_2tag_Zvv_mu_only.Fill( t_2tag_Zvv_8 , HistCollection::CONNECT_SUB , HistCollection::REW_READ_MUONLY , 0 );
    }


    //
    // Renormalise
    //
    //  Scale multijet component:
    //  n4/n2 = n4data / (a*n2data - n2MC + n4MC) ==>
    //      1 = n4data / (a*n2data - n2MC + n4MC) ==>
    //      a = (n4data - n4MC + n2MC)/n2data
    //
    HistCollection h_4tag( h_4tag_data );
    HistCollection h_4tag_mu_only( h_4tag_data );
    if ( useMC ) {
      h_4tag.Add( h_4tag_ttLep , -1);
      h_4tag.Add( h_4tag_ttHad , -1);
      h_4tag.Add( h_4tag_Zvv   , -1);
      h_4tag.Add( h_2tag_ttLep , 1);
      h_4tag.Add( h_2tag_ttHad , 1);
      h_4tag.Add( h_2tag_Zvv   , 1);
      
      h_4tag_mu_only.Add( h_4tag_ttLep , -1);
      h_4tag_mu_only.Add( h_4tag_ttHad , -1);
      h_4tag_mu_only.Add( h_4tag_Zvv   , -1);
      h_4tag_mu_only.Add( h_2tag_ttLep_mu_only , 1);
      h_4tag_mu_only.Add( h_2tag_ttHad_mu_only , 1);
      h_4tag_mu_only.Add( h_2tag_Zvv_mu_only   , 1);
    }
    
    if ( regions.at(reg) == Region::CR || regions.at(reg) == Region::SR ) {
      if ( regions.at(reg) == Region::CR ) {
	renormScale_CR        = HistCollection::RenormScale(h_4tag, h_2tag_data);
	renormScale_muOnly_CR = HistCollection::RenormScale(h_4tag_mu_only, h_2tag_data_mu_only);
      }
      renormScale        = renormScale_CR;
      renormScale_muOnly = renormScale_muOnly_CR;
    }
    else if ( regions.at(reg) == Region::CRexclVR1 || regions.at(reg) == Region::SRexclVR1 || regions.at(reg) == Region::VR1 ) {
      if ( regions.at(reg) == Region::CRexclVR1 ) {
	renormScale_CRexclVR1        = HistCollection::RenormScale(h_4tag, h_2tag_data);
	renormScale_muOnly_CRexclVR1 = HistCollection::RenormScale(h_4tag_mu_only, h_2tag_data_mu_only);
      }
      renormScale        = renormScale_CRexclVR1;
      renormScale_muOnly = renormScale_muOnly_CRexclVR1;
    }
    else if ( regions.at(reg) == Region::CRexclVR2 || regions.at(reg) == Region::SRexclVR2 || regions.at(reg) == Region::VR2 ) {
      if ( regions.at(reg) == Region::CRexclVR2 ) {
	renormScale_CRexclVR2        = HistCollection::RenormScale(h_4tag, h_2tag_data);
	renormScale_muOnly_CRexclVR2 = HistCollection::RenormScale(h_4tag_mu_only, h_2tag_data_mu_only);
      }
      renormScale        = renormScale_CRexclVR2;
      renormScale_muOnly = renormScale_muOnly_CRexclVR2;
    }

    if ( ! renormScale || ! renormScale_muOnly ) {
      std::cout << "main() : renormScale = " << renormScale << ", renormScale_muOnly = " << renormScale_muOnly << std::endl;
      return 0;
    }
    
    h_2tag_data.Renormalize( *renormScale );
    h_2tag_data_mu_only.Renormalize( *renormScale_muOnly );

    
    //
    // signal
    //
    
    HistCollection h_4tag_HinoM130("h_4tag_HinoM130", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_4tag_HinoM150("h_4tag_HinoM150", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_4tag_HinoM200("h_4tag_HinoM200", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_4tag_HinoM300("h_4tag_HinoM300", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_4tag_HinoM400("h_4tag_HinoM400", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_4tag_HinoM600("h_4tag_HinoM600", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    HistCollection h_4tag_HinoM800("h_4tag_HinoM800", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
    h_4tag_HinoM130.Fill( t_4tag_HinoM130 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF , 0 );
    h_4tag_HinoM150.Fill( t_4tag_HinoM150 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF , 0 );
    h_4tag_HinoM200.Fill( t_4tag_HinoM200 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF , 0 );
    h_4tag_HinoM300.Fill( t_4tag_HinoM300 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF , 0 );
    h_4tag_HinoM400.Fill( t_4tag_HinoM400 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF , 0 );
    h_4tag_HinoM600.Fill( t_4tag_HinoM600 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF , 0 );
    h_4tag_HinoM800.Fill( t_4tag_HinoM800 , HistCollection::CONNECT_SUB , HistCollection::REW_OFF , 0 );


    //
    // plot
    //
    
    Plot( h_2tag_data, h_2tag_ttLep, h_2tag_ttHad, h_2tag_Zvv, h_4tag_data, h_4tag_ttLep, h_4tag_ttHad, h_4tag_Zvv, h_4tag_HinoM150 , h_4tag_HinoM300 , Region::String(regions.at(reg)) , vecOUTDIR.at(reg) , "");
    Plot( h_2tag_data_mu_only, h_2tag_ttLep_mu_only, h_2tag_ttHad_mu_only, h_2tag_Zvv_mu_only, h_4tag_data, h_4tag_ttLep, h_4tag_ttHad, h_4tag_Zvv, h_4tag_HinoM150 , h_4tag_HinoM300 , Region::String(regions.at(reg)) , vecOUTDIR.at(reg), "mu_only");
    
  } 
  
  delete inputFile;

  return 0;

}








void Plot1D(TH1F * data_2tag,
	    TH1F * ttLep_2tag,
	    TH1F * ttHad_2tag,
	    TH1F * Zvv_2tag,
	    TH1F * data_4tag,
	    TH1F * ttLep_4tag,
	    TH1F * ttHad_4pplottag,
	    TH1F * Zvv_4tag,
	    TH1F * HinoM150_4tag,
	    TH1F * HinoM300_4tag,
	    TString reg,
	    TString dirname,
	    TString OUTPOSTFIX);

void Plot2D(TH2F * data_2tag,
	    TH2F * ttLep_2tag,
	    TH2F * ttHad_2tag,
	    TH2F * Zvv_2tag,
	    TH2F * data_4tag,
	    TH2F * ttLep_4tag,
	    TH2F * ttHad_4tag,
	    TH2F * Zvv_4tag,
	    TH2F * HinoM130_4tag,
	    TH2F * HinoM300_4tag,
	    TString reg,
	    TString dirname,
	    TString OUTPOSTFIX);

void Plot(const HistCollection & h_2tag_data,
	  const HistCollection & h_2tag_ttLep,
	  const HistCollection & h_2tag_ttHad,
	  const HistCollection & h_2tag_Zvv,
	  const HistCollection & h_4tag_data,
	  const HistCollection & h_4tag_ttLep,
	  const HistCollection & h_4tag_ttHad,
	  const HistCollection & h_4tag_Zvv,
	  const HistCollection & h_4tag_HinoM150,
	  const HistCollection & h_4tag_HinoM300,
	  TString reg,
	  TString dirname,
	  TString OUTPOSTFIX)
{
  
  const std::map<HistInfo, HistBase *> histMap_2tag_data  = h_2tag_data.HistMap(); 
  std::map<HistInfo, HistBase *>::const_iterator it    = histMap_2tag_data.begin();
  std::map<HistInfo, HistBase *>::const_iterator itEnd = histMap_2tag_data.end();
  for ( ; it != itEnd; ++it) {

    const HistInfo & info = it->first;
    const TString & hname = info.Name();  
    
    bool is1D = dynamic_cast<const Hist1D *>(it->second) == 0 ? false : true;
    bool is2D = dynamic_cast<const Hist2D *>(it->second) == 0 ? false : true;

    if ( is1D ) {
      
      TH1F * data_2tag     = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( it->second                         )->Get()->Clone());
      TH1F * ttLep_2tag    = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_2tag_ttLep   .Histogram( hname ) )->Get()->Clone());
      TH1F * ttHad_2tag    = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_2tag_ttHad   .Histogram( hname ) )->Get()->Clone());
      TH1F * Zvv_2tag      = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_2tag_Zvv     .Histogram( hname ) )->Get()->Clone());
      TH1F * data_4tag     = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_data    .Histogram( hname ) )->Get()->Clone());
      TH1F * ttLep_4tag    = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_ttLep   .Histogram( hname ) )->Get()->Clone());
      TH1F * ttHad_4tag    = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_ttHad   .Histogram( hname ) )->Get()->Clone());
      TH1F * Zvv_4tag      = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_Zvv     .Histogram( hname ) )->Get()->Clone());
      TH1F * HinoM150_4tag = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_HinoM150.Histogram( hname ) )->Get()->Clone());
      TH1F * HinoM300_4tag = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_HinoM300.Histogram( hname ) )->Get()->Clone());

      Plot1D( data_2tag , ttLep_2tag , ttHad_2tag , Zvv_2tag , data_4tag , ttLep_4tag , ttHad_4tag , Zvv_4tag , HinoM150_4tag , HinoM300_4tag , reg , dirname , OUTPOSTFIX );

    }
    else if ( is2D ) {

      TH2F * data_2tag     = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( it->second                         )->Get()->Clone());
      TH2F * ttLep_2tag    = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_2tag_ttLep   .Histogram( hname ) )->Get()->Clone());
      TH2F * ttHad_2tag    = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_2tag_ttHad   .Histogram( hname ) )->Get()->Clone());
      TH2F * Zvv_2tag      = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_2tag_Zvv     .Histogram( hname ) )->Get()->Clone());
      TH2F * data_4tag     = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_data    .Histogram( hname ) )->Get()->Clone());
      TH2F * ttLep_4tag    = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_ttLep   .Histogram( hname ) )->Get()->Clone());
      TH2F * ttHad_4tag    = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_ttHad   .Histogram( hname ) )->Get()->Clone());
      TH2F * Zvv_4tag      = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_Zvv     .Histogram( hname ) )->Get()->Clone());
      TH2F * HinoM150_4tag = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_HinoM150.Histogram( hname ) )->Get()->Clone());
      TH2F * HinoM300_4tag = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_HinoM300.Histogram( hname ) )->Get()->Clone());

      Plot2D( data_2tag , ttLep_2tag , ttHad_2tag , Zvv_2tag , data_4tag , ttLep_4tag , ttHad_4tag , Zvv_4tag , HinoM150_4tag, HinoM300_4tag , reg , dirname , OUTPOSTFIX );

    }
    else {
      std::cout << "Plot() : Error - Couldn't determine histogram type for hname = " << hname.Data() << std::endl;
      continue;
    }
    
  }
        
}


void PrepareTH1F(TH1F * h, const TString & name, int color, int style, int fill, int rebin = 1)
{

  if ( ! h ) return;

  h->SetDirectory(0);

  //h->SetName( TString::Format("%s_%s", h->GetName(), name.Data()) );
  h->Rebin(rebin);
  h->SetStats(0);
  h->SetTitle("");
  h->GetYaxis()->SetTitleOffset(1.15);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
  h->SetLineColor(color);
  h->SetLineStyle(style);
  h->SetLineWidth(1);
  if ( fill ) {
    h->SetLineColor(kBlack);
    h->SetFillColor(color);
  }
  
}


void Plot1D(TH1F * data_2tag,
	    TH1F * ttLep_2tag,
	    TH1F * ttHad_2tag,
	    TH1F * Zvv_2tag,
	    TH1F * data_4tag,
	    TH1F * ttLep_4tag,
	    TH1F * ttHad_4tag,
	    TH1F * Zvv_4tag,
	    TH1F * HinoM150_4tag,
	    TH1F * HinoM300_4tag,
	    TString reg,
	    TString dirname,
	    TString OUTPOSTFIX)
{

  TString varName = data_4tag->GetName();
  
  TFile * out = new TFile(TString::Format("%s/H1D/root/%s%s.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" +  OUTPOSTFIX).Data() : ""), "recreate");
  out->cd();
  data_2tag ->SetName("data_2tag" );
  ttLep_2tag->SetName("ttLep_2tag");
  ttHad_2tag->SetName("ttHad_2tag");
  Zvv_2tag  ->SetName("Zvv_2tag"  );
  data_4tag ->SetName("data_4tag" );
  ttLep_4tag->SetName("ttLep_4tag");
  ttHad_4tag->SetName("ttHad_4tag");
  Zvv_4tag  ->SetName("Zvv_4tag"  );
  HinoM150_4tag->SetName("HinoM150_4tag");
  HinoM300_4tag->SetName("HinoM300_4tag");
  data_2tag ->Write();
  ttLep_2tag->Write();
  ttHad_2tag->Write();
  Zvv_2tag  ->Write();
  data_4tag ->Write();
  ttLep_4tag->Write();
  ttHad_4tag->Write();
  Zvv_4tag  ->Write();
  HinoM150_4tag->Write();
  HinoM300_4tag->Write();
  out->Close();  
  delete out;

  TCanvas * c = new TCanvas("c", "c", 1200, 800);
  c->cd();
  TPad * p1 = new TPad("p1", "p1", 0.0, 0.3, 1.0, 1.0);
  TPad * p2 = new TPad("p2", "p2", 0.0, 0.0, 1.0, 0.3);
  p1->Draw();
  p2->Draw();

  p1->SetTicks(1,1);
  p1->SetTopMargin(0.05);
  p1->SetBottomMargin(0.02);
  p1->SetRightMargin(0.08);

  p2->SetTicks(1,1);
  p2->SetBottomMargin(0.3);
  p2->SetTopMargin(0.05);
  p2->SetRightMargin(0.08);

  p1->cd();
  
  TH1 * frame1 = p1->DrawFrame(data_2tag->GetXaxis()->GetXmin(), 0.01, data_2tag->GetXaxis()->GetXmax(), data_2tag->GetMaximum()*2.);
  frame1->GetYaxis()->SetTitle("Number of events");
  frame1->GetYaxis()->CenterTitle();
  frame1->GetYaxis()->SetTitleSize(0.05);
  frame1->GetYaxis()->SetTitleOffset(0.975);
  frame1->GetXaxis()->SetLabelSize(0.);
  frame1->GetYaxis()->SetLabelSize(0.05);

  TH1F * QCD = static_cast<TH1F *>(data_2tag->Clone());
  // QCD->Add(ttLep_4tag, -1.);
  // QCD->Add(ttHad_4tag, -1.);
  // QCD->Add(Zvv_4tag  , -1.);
  QCD->Add(ttLep_2tag, -1.);
  QCD->Add(ttHad_2tag, -1.);
  QCD->Add(Zvv_2tag  , -1.);
  
  int rebin = 1;
  PrepareTH1F(data_4tag , "Data"      , kBlack      , kSolid , 0, rebin);
  PrepareTH1F(QCD       , "QCD"       , kYellow - 4 , kSolid , 1, rebin);
  PrepareTH1F(ttLep_4tag, "ttLep_4tag", kAzure - 9  , kSolid , 1, rebin);
  PrepareTH1F(ttHad_4tag, "ttHad_4tag", kAzure + 1  , kSolid , 1, rebin);
  PrepareTH1F(Zvv_4tag  , "Zvv_4tag"  , kGreen - 7  , kSolid , 1, rebin);
  PrepareTH1F(ttLep_2tag, "ttLep_2tag", kMagenta - 4, kDashed, 0, rebin);
  PrepareTH1F(ttHad_2tag, "ttHad_2tag", kMagenta + 1, kDashed, 0, rebin);
  PrepareTH1F(Zvv_2tag  , "Zvv_2tag"  , kGreen - 7  , kSolid , 1, rebin);
  
  data_4tag->SetMarkerStyle(20);
  data_4tag->SetMarkerColor(kBlack);
  data_4tag->SetMarkerSize();
  data_4tag->SetMarkerStyle(20);

  THStack * pred = new THStack("prediction", "prediction");
  if ( Zvv_2tag  && Zvv_2tag->Integral() > 0 ) { Zvv_2tag->Scale( Zvv_4tag->Integral() / Zvv_2tag->Integral() ); pred->Add( Zvv_2tag ); }
  else if ( Zvv_4tag ) pred->Add( Zvv_4tag   );
  if ( ttLep_2tag && ttLep_2tag->Integral() > 0 ) { ttLep_2tag->Scale( ttLep_4tag->Integral() / ttLep_2tag->Integral() ); }//pred->Add( ttLep_2tag ); }
  if ( ttHad_2tag && ttHad_2tag->Integral() > 0 ) { ttHad_2tag->Scale( ttHad_4tag->Integral() / ttHad_2tag->Integral() ); }//pred->Add( ttHad_2tag ); }
  if ( ttLep_4tag ) pred->Add( ttLep_4tag );
  if ( ttHad_4tag ) pred->Add( ttHad_4tag );
  pred->Add( QCD );
  pred->Draw("histsame");
  data_4tag->Draw("pesame");

  // ttHad_2tag->Add( ttLep_4tag );
  // ttHad_2tag->Add( Zvv_2tag   );
  // ttLep_2tag->Add( Zvv_2tag   );  
  // ttHad_2tag->Draw("histsame");
  // ttLep_2tag->Draw("histsame");
  
  gPad->RedrawAxis();

  TLegend leg(0.70, 0.64, 0.85, 0.89);
  leg.SetFillColor(0);
  leg.SetLineColor(0);
  leg.SetTextSize(0.045);
  leg.AddEntry(data_4tag, "Data", "pe");
  if (ttLep_4tag || ttHad_4tag) leg.AddEntry(QCD, "Multijet", "f");
  else leg.AddEntry(QCD, "Background", "f");
  if (ttLep_4tag && ttLep_4tag->Integral() > 0.) leg.AddEntry(ttLep_4tag, "t#bar{t} (leptonic)", "f");
  if (ttHad_4tag && ttHad_4tag->Integral() > 0.) leg.AddEntry(ttHad_4tag, "t#bar{t} (hadronic)", "f");
  if (Zvv_4tag   && Zvv_4tag->Integral()   > 0.) leg.AddEntry(Zvv_4tag  , "Z#rightarrow #nu#nu + jets", "f");
  leg.Draw("same");

  TLatex tex;
  tex.SetNDC();
  tex.SetTextSize(0.05);
  tex.SetTextFont(42);
  tex.DrawLatex(0.21,0.86, reg);
  tex.SetTextSize(0.04);
  tex.DrawLatex(0.21, 0.80, "Data events");
  tex.DrawLatex(0.36, 0.80, TString::Format(": %0.0f", data_4tag->Integral(0, -1)));
  tex.DrawLatex(0.21, 0.75, "Background events");
  tex.DrawLatex(0.36, 0.75, TString::Format(": %0.1f", static_cast<TH1F*>(pred->GetStack()->Last())->Integral(0, 1e6)));
  
  p2->cd();

  TH1 * frame2 = p2->DrawFrame(data_2tag->GetXaxis()->GetXmin(), 0.31, data_2tag->GetXaxis()->GetXmax(), 1.69);
  frame2->GetXaxis()->SetTitle( data_2tag->GetXaxis()->GetTitle() );
  frame2->GetXaxis()->CenterTitle();
  frame2->GetYaxis()->SetTitle("data / pred");
  frame2->GetYaxis()->CenterTitle();
  frame2->GetXaxis()->SetTitleSize(0.11);
  frame2->GetXaxis()->SetTitleOffset(1.15);
  frame2->GetXaxis()->SetLabelSize(0.12);
  frame2->GetXaxis()->SetTickLength(0.08);
  frame2->GetYaxis()->SetTitleSize(0.1);
  frame2->GetYaxis()->SetTitleOffset(0.45);
  frame2->GetYaxis()->SetLabelSize(0.11);
  frame2->GetYaxis()->SetTickLength(0.02);
  frame2->GetYaxis()->SetNdivisions(505);
  
  TH1F * ratio = data_4tag->Integral() > 0 ? static_cast<TH1F *>(data_4tag->Clone()) : static_cast<TH1F *>(pred->GetStack()->Last()->Clone());
  ratio->Divide( static_cast<TH1F *>(pred->GetStack()->Last()) );
  ratio->SetLineWidth(1);
  ratio->SetLineColor(kBlack);
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerStyle(20);

  ratio->Draw("samepe");

  TLine line(data_2tag->GetXaxis()->GetXmin(), 1.0, data_2tag->GetXaxis()->GetXmax(), 1.0);
  line.SetLineColor(kBlack);
  line.SetLineStyle(kDashed);
  line.Draw("same");

  float chi2 = 0;
  int ndf = 0;
  for (int i = 1; i <= ratio->GetNbinsX(); ++i) {
    float r   = ratio->GetBinContent(i);
    float r_e = ratio->GetBinError(i);
    if ( r > 0. && r_e > 0.) {
      chi2 += std::pow((r-1.)/r_e, 2.);
      ndf += 1;
    }
  }
  tex.SetTextSize(0.1);
  tex.DrawLatex(0.12,0.75, TString::Format("#chi^{2}/ndf = %0.1f/%d = %0.1f", chi2, ndf, chi2/ndf));
  
 
  c->SaveAs( TString::Format("%s/H1D/pdf/%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H1D/eps/%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H1D/root/canvas_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  frame1->SetMaximum(data_2tag->GetMaximum()*5000.);
  p1->SetLogy();
  c->SaveAs( TString::Format("%s/H1D/pdf/%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H1D/eps/%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H1D/root/canvas_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  
  delete frame1;
  delete frame2;
  delete data_2tag;
  delete ttLep_2tag;
  delete ttHad_2tag;
  delete Zvv_2tag;
  delete data_4tag;
  delete ttLep_4tag;
  delete ttHad_4tag;
  delete Zvv_4tag;
  delete QCD;
  delete HinoM150_4tag;
  delete HinoM300_4tag;
  delete pred;
  delete ratio;
  delete p1;
  delete p2;
  delete c;

}


void PrepareTH2F(TH2F * h, const TString & name)
{

  if ( ! h ) return;

  h->SetDirectory(0);
  
  h->SetName( TString::Format("%s_%s", h->GetName(), name.Data()) );

  h->SetStats(0);
  h->SetTitle("");

  h->GetZaxis()->SetTitle("Number of events");
  h->GetZaxis()->CenterTitle();
  h->GetZaxis()->SetTitleSize(0.05);
  h->GetZaxis()->SetTitleOffset(0.975);

  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleOffset(0.975);

  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetXaxis()->SetTitleOffset(0.975);
  
}


void Plot2D(TH2F * data_2tag,
	    TH2F * ttLep_2tag,
	    TH2F * ttHad_2tag,
	    TH2F * Zvv_2tag,
	    TH2F * data_4tag,
	    TH2F * ttLep_4tag,
	    TH2F * ttHad_4tag,
	    TH2F * Zvv_4tag,
	    TH2F * HinoM150_4tag,
	    TH2F * HinoM300_4tag,
	    TString reg,
	    TString dirname,
	    TString OUTPOSTFIX)
{

  TString varName = data_4tag->GetName();

  TFile * out = new TFile(TString::Format("%s/H2D/root/%s%s.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : ""), "recreate");
  out->cd();
  data_2tag ->SetName("data_2tag" );
  ttLep_2tag->SetName("ttLep_2tag");
  ttHad_2tag->SetName("ttHad_2tag");
  Zvv_2tag  ->SetName("Zvv_2tag"  );
  data_4tag ->SetName("data_4tag" );
  ttLep_4tag->SetName("ttLep_4tag");
  ttHad_4tag->SetName("ttHad_4tag");
  Zvv_4tag  ->SetName("Zvv_4tag"  );
  HinoM150_4tag->SetName("HinoM150_4tag");
  HinoM300_4tag->SetName("HinoM300_4tag");
  data_2tag ->Write();
  ttLep_2tag->Write();
  ttHad_2tag->Write();
  Zvv_2tag  ->Write();
  data_4tag ->Write();
  ttLep_4tag->Write();
  ttHad_4tag->Write();
  Zvv_4tag  ->Write();
  HinoM150_4tag->Write();
  HinoM300_4tag->Write();
  out->Close();  
  delete out;
  
  TCanvas * c = new TCanvas("c", "c", 1000, 800);
  c->cd();


  PrepareTH2F(data_4tag    , "data_4tag"    );
  PrepareTH2F(data_2tag    , "2data_2tag"   );
  PrepareTH2F(ttLep_4tag   , "ttLep_4tag"   );
  PrepareTH2F(ttHad_4tag   , "ttHad_4tag"   );
  PrepareTH2F(Zvv_4tag     , "Zvv_4tag"     );
  PrepareTH2F(HinoM150_4tag, "HinoM150_4tag");
  PrepareTH2F(HinoM300_4tag, "HinoM300_4tag");
  
  data_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/Data_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/Data_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/Data_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/Data_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/Data_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/Data_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  
  data_2tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/Multijet_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/Multijet_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/Multijet_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/Multijet_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/Multijet_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/Multijet_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  
  ttLep_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/ttLep_4tag_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/ttLep_4tag_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/ttLep_4tag_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/ttLep_4tag_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/ttLep_4tag_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/ttLep_4tag_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  
  ttHad_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/ttHad_4tag_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/ttHad_4tag_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/ttHad_4tag_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/ttHad_4tag_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/ttHad_4tag_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/ttHad_4tag_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  Zvv_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/Zvv_4tag_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/Zvv_4tag_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/Zvv_4tag_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/Zvv_4tag_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/Zvv_4tag_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/Zvv_4tag_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  TH2F * SB_HinoM150     = static_cast<TH2F *>( data_2tag->Clone() );
  TH2F * SB_HinoM300     = static_cast<TH2F *>( data_2tag->Clone() );
  TH2F * SRootB_HinoM150 = static_cast<TH2F *>( data_2tag->Clone() );
  TH2F * SRootB_HinoM300 = static_cast<TH2F *>( data_2tag->Clone() );
  for (int xbin = 0; xbin <= SB_HinoM150->GetNbinsX() + 1; ++xbin) {
    for (int ybin = 0; ybin <= SB_HinoM150->GetNbinsY() + 1; ++ybin) {
      float s1 = HinoM150_4tag->GetBinContent(xbin, ybin);
      float s2 = HinoM150_4tag->GetBinContent(xbin, ybin);
      float b = data_2tag->GetBinContent(xbin, ybin) + ttLep_4tag->GetBinContent(xbin, ybin) + ttHad_4tag->GetBinContent(xbin, ybin) + Zvv_4tag->GetBinContent(xbin, ybin);
      if ( b > 0 ) {
	SB_HinoM150->SetBinContent(xbin, ybin, s1/b);
	SB_HinoM300->SetBinContent(xbin, ybin, s2/b);
	SRootB_HinoM150->SetBinContent(xbin, ybin, s1/sqrt(b));
	SRootB_HinoM300->SetBinContent(xbin, ybin, s2/sqrt(b));
      }
      else {
	SB_HinoM150->SetBinContent(xbin, ybin, 0);
	SB_HinoM300->SetBinContent(xbin, ybin, 0);
	SRootB_HinoM150->SetBinContent(xbin, ybin, 0);
	SRootB_HinoM300->SetBinContent(xbin, ybin, 0);
      }
    }
  }
  
  SB_HinoM150->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/SB_HinoM150_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SB_HinoM150_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SB_HinoM150_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/SB_HinoM150_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SB_HinoM150_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SB_HinoM150_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  SB_HinoM300->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/SB_HinoM300_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SB_HinoM300_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SB_HinoM300_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/SB_HinoM300_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SB_HinoM300_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SB_HinoM300_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  SRootB_HinoM150->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/SRootB_HinoM150_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SRootB_HinoM150_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SRootB_HinoM150_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/SRootB_HinoM150_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SRootB_HinoM150_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SRootB_HinoM150_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  SRootB_HinoM300->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/SRootB_HinoM300_%s%s__LinScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SRootB_HinoM300_%s%s__LinScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SRootB_HinoM300_%s%s__LinScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/SRootB_HinoM300_%s%s__LogScale.pdf"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/eps/SRootB_HinoM300_%s%s__LogScale.eps"  , dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );
  c->SaveAs( TString::Format("%s/H2D/root/SRootB_HinoM300_%s%s__LogScale.root", dirname.Data(), varName.Data(), OUTPOSTFIX.Length() > 0 ? ("_" + OUTPOSTFIX).Data() : "") );

  
  delete c;
  delete data_2tag;
  delete ttLep_2tag;
  delete ttHad_2tag;
  delete Zvv_2tag;
  delete data_4tag;
  delete ttLep_4tag;
  delete ttHad_4tag;
  delete Zvv_4tag;
  delete HinoM150_4tag;
  delete HinoM300_4tag;
  delete SB_HinoM150;
  delete SB_HinoM300;
  delete SRootB_HinoM150;
  delete SRootB_HinoM300;
 
}
