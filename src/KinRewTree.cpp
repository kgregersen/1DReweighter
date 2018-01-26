// local includes
#include "HistCollection.h"
#include "Event.h"
#include "TagBit.h"
#include "TrigBit.h"
#include "Region.h"
#include "Reweighter.h"
#include "Hist1D.h"
#include "Hist2D.h"
#include "HistInfo.h"

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
#include "TF1.h"
#include "TSpline.h"

// STL includes
#include <iostream>
#include <map>
#include <utility>
#include <sys/stat.h>


//
// forward declarations
//
void UpdateTree(TTree * tree,
		const std::map<int, Reweighter> * rewFcns,
		float muQCD,
		const TString & region);

void Plot(const HistCollection & h_2tag_data,
	  const HistCollection & h_2tag_ttLep,
	  const HistCollection & h_2tag_ttHad,
	  const HistCollection & h_2tag_Zvv,
	  const HistCollection & h_4tag_data,
	  const HistCollection & h_4tag_ttLep,
	  const HistCollection & h_4tag_ttHad,
	  const HistCollection & h_4tag_Zvv,
	  const Reweighter * rew,
	  TString reg,
	  int itr,
	  int var,
	  TString OUTPREFIX);


//
// main program
//
int main(int argc, char * argv[])
{


  //========================================================================================================
  //----------------------------->>>>>>>>>>      SETTINGS       <<<<<<<<<<----------------------------------
  //========================================================================================================

  //TFile * inputFile = new TFile("../files/Processed_EventAnaResults_resolved_171104_Copy_partiallyDataDriven.root", "update");  
  //TFile * inputFile = new TFile("../files/Processed_EventAnaResults_resolved_171104_Copy_fullyDataDriven.root", "update");  
  //TFile * inputFile = new TFile("../files/Processed_EventAnaResults_resolved_171104_Copy_partiallyDataDriven2.root", "update");  
  //TFile * inputFile = new TFile("../files/Processed_EventAnaResults_resolved_171104_Copy_fullyDataDriven2.root", "update");  
  TFile * inputFile = new TFile("../files/Processed_EventAnaResults_resolved_171104_Copy_partiallyDataDriven_LepTTBarScaled_1p8.root", "update");  

  // Switch for using MC
  bool useMC = true;
  //bool useMC = false;
  float ttLep4TagScale = 1.8;
  
  // Output directory for plots
  //TString OUTDIR = "diagnostics_partiallyDataDriven/Input171104";
  //TString OUTDIR = "diagnostics_fullyDataDriven/Input171104";
  //TString OUTDIR = "diagnostics_partiallyDataDriven2/Input171104";
  //TString OUTDIR = "diagnostics_fullyDataDriven2/Input171104";
  TString OUTDIR = "diagnostics_partiallyDataDriven_LepTTBarScaled_1p8/Input171104";

  // Regions used to derive reweighting
  std::vector<Region::TYPE> regions;
  regions.push_back( Region::CR        );
  regions.push_back( Region::CRexclVR1 );
  regions.push_back( Region::CRexclVR2 );

  // Number of iterations
  const int Nitr = 3;

  // Add variables to the reweighting here:
  // - The Reweighter constructor takes 3 arguments:
  // ---> 1 : the name of the corresponding histogram in HistCollection
  // ---> 2 : the name of the 'x' variable in Event
  // ---> 3 : the name of the 'y' variable iun Event (for 2D reweighting)
  // - The integer in the map denotes the order in which the variables are used for reweighting
  std::map<int, Reweighter> rewFcns;
  rewFcns.insert( std::make_pair(  0 , Reweighter("jet_n"           , "jet_n"           , "") ) );
  rewFcns.insert( std::make_pair(  1 , Reweighter("jet3_pt"         , "jet3_pt"         , "") ) );
  rewFcns.insert( std::make_pair(  2 , Reweighter("jet4_pt"         , "jet4_pt"         , "") ) );
  rewFcns.insert( std::make_pair(  3 , Reweighter("avgAbsHCJetEta"  , "avgAbsHCJetEta"  , "") ) );
  rewFcns.insert( std::make_pair(  4 , Reweighter("GCdR_min"        , "GCdR_min"        , "") ) );
  rewFcns.insert( std::make_pair(  5 , Reweighter("GCdR_max"        , "GCdR_max"        , "") ) );
  rewFcns.insert( std::make_pair(  6 , Reweighter("hcand1_pt"       , "hcand1_pt"       , "") ) );
  rewFcns.insert( std::make_pair(  7 , Reweighter("hcand2_pt"       , "hcand2_pt"       , "") ) );
  rewFcns.insert( std::make_pair(  8 , Reweighter("hcand1_hcand2_pt", "hcand1_hcand2_pt", "") ) );
  rewFcns.insert( std::make_pair(  9 , Reweighter("meffHC"          , "meffHC"          , "") ) );
  rewFcns.insert( std::make_pair( 10 , Reweighter("MEt"             , "MEt"             , "") ) );
  rewFcns.insert( std::make_pair( 11 , Reweighter("hcand1_m"        , "hcand1_m"        , "") ) );
  rewFcns.insert( std::make_pair( 12 , Reweighter("hcand2_m"        , "hcand2_m"        , "") ) );

  // variables to plot
  std::vector<TString> variables = {"jet_n"           ,
				    "jet3_pt"         ,
				    "jet4_pt"         ,
				    "avgAbsHCJetEta"  ,
				    "GCdR_min"        ,
				    "GCdR_max"        ,
				    "hcand1_pt"       ,
				    "hcand2_pt"       ,
				    "hcand1_hcand2_pt",
				    "meffHC"          ,
				    "MEt"             ,
				    "hcand1_m"        ,
				    "hcand2_m"        };
    
  
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

  const int Nreg = regions.size();
  const int Nvar = rewFcns.size();

  for (int iItr = 0; iItr < Nitr - 1; ++iItr) {
    for (int iRew = 0; iRew < Nvar; ++iRew) {
      const Reweighter & rew = rewFcns.find( iRew )->second;
      int iRewFcn = Nvar*(1 + iItr) + iRew;
      std::cout << "main() : Adding Reweighter (" << iRewFcn << "," << rew.Xvar() << ")" << std::endl;
      rewFcns.insert( std::make_pair( iRewFcn , Reweighter( rew ) ) );
    }
  }  

  for (int reg = 0; reg < Nreg; ++reg) {  
    
    TTree * t_2tag_data  = static_cast<TTree *>( inputFile->Get( "data_SemiTagged"                              ) );
    TTree * t_4tag_data  = static_cast<TTree *>( inputFile->Get( "data_FullyTagged"                             ) );
    TTree * t_2tag_ttLep = static_cast<TTree *>( inputFile->Get( "TTBarNonAllHad_Nominal_SemiTagged"            ) );
    TTree * t_4tag_ttLep = static_cast<TTree *>( inputFile->Get( "TTBarNonAllHad_Nominal_FullyTagged"           ) );
    TTree * t_2tag_ttHad = static_cast<TTree *>( inputFile->Get( "TTBarAllHad_Nominal_SemiTagged"               ) );
    TTree * t_4tag_ttHad = static_cast<TTree *>( inputFile->Get( "TTBarAllHad_Nominal_FullyTagged"              ) );
    TTree * t_2tag_Zvv_1 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt0_70_Nominal_SemiTagged"        ) );
    TTree * t_4tag_Zvv_1 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt0_70_Nominal_FullyTagged"       ) );
    TTree * t_2tag_Zvv_2 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt70_140_Nominal_SemiTagged"      ) );
    TTree * t_4tag_Zvv_2 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt70_140_Nominal_FullyTagged"     ) );
    TTree * t_2tag_Zvv_3 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt140_280_Nominal_SemiTagged"     ) );
    TTree * t_4tag_Zvv_3 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt140_280_Nominal_FullyTagged"    ) );
    TTree * t_2tag_Zvv_4 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt280_500_Nominal_SemiTagged"     ) );
    TTree * t_4tag_Zvv_4 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt280_500_Nominal_FullyTagged"    ) );
    TTree * t_2tag_Zvv_5 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt500_700_Nominal_SemiTagged"     ) );
    TTree * t_4tag_Zvv_5 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt500_700_Nominal_FullyTagged"    ) );
    TTree * t_2tag_Zvv_6 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt700_1000_Nominal_SemiTagged"    ) );
    TTree * t_4tag_Zvv_6 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt700_1000_Nominal_FullyTagged"   ) );
    TTree * t_2tag_Zvv_7 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt1000_2000_Nominal_SemiTagged"   ) );
    TTree * t_4tag_Zvv_7 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt1000_2000_Nominal_FullyTagged"  ) );
    TTree * t_2tag_Zvv_8 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt2000_E_CMS_Nominal_SemiTagged"  ) );
    TTree * t_4tag_Zvv_8 = static_cast<TTree *>( inputFile->Get( "Znunu_BFilt_Pt2000_E_CMS_Nominal_FullyTagged" ) );
    
    float muQCD = 1.;

    int iRewFcn = 0;
    for (int itr = 0; itr <= Nitr; ++itr) {
      for (int var = 0; var < (itr == Nitr ? 1 : Nvar); ++var) {
	
	std::cout << "main() : "
		  << "Region = "    << Region::String( regions.at(reg) ) << " (" << reg+1 << "/" << Nreg << ")  "
		  << "Iteration = " << (itr == Nitr ? itr : itr + 1) << "/" << Nitr << "  "
		  << "Variable = "  << (rewFcns.find( var )->second).Xvar().Data() << " (" << var+1 << "/" << (itr == Nitr ? 1 : Nvar) << ")"
		  << (itr == Nitr ? "  (final plots)" : " ")
		  << std::endl; 

	TagBit::WP    WP      = TagBit::WP70;
	Region::TYPE  REG     = regions.at( reg );
	TrigBit::TYPE TRIG    = TrigBit::OR2016;
	bool          LepVeto = true;
	bool          XWtVeto = true;
	  
	HistCollection h_2tag_data("h_2tag_data", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	HistCollection h_4tag_data("h_4tag_data", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	h_2tag_data.Fill( t_2tag_data , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	h_4tag_data.Fill( t_4tag_data , HistCollection::CONNECT_ALL , HistCollection::REW_OFF, 0 );
	
	HistCollection h_2tag_ttLep("h_2tag_ttLep", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	HistCollection h_4tag_ttLep("h_4tag_ttLep", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	if ( useMC ) {
	  h_2tag_ttLep.Fill( t_2tag_ttLep , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_4tag_ttLep.Fill( t_4tag_ttLep , HistCollection::CONNECT_ALL , HistCollection::REW_OFF, 0 );
	  h_4tag_ttLep.Scale( ttLep4TagScale );
	}

	HistCollection h_2tag_ttHad("h_2tag_ttHad", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	HistCollection h_4tag_ttHad("h_4tag_ttHad", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	if ( useMC ) {
	  h_2tag_ttHad.Fill( t_2tag_ttHad , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_4tag_ttHad.Fill( t_4tag_ttHad , HistCollection::CONNECT_ALL , HistCollection::REW_OFF, 0 );
	}

	HistCollection h_2tag_Zvv("h_2tag_Zvv", TagBit::TAG2, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	HistCollection h_4tag_Zvv("h_4tag_Zvv", TagBit::TAG4, WP, REG, TRIG, LepVeto, XWtVeto, variables);
	if ( useMC ) {
	  h_2tag_Zvv.Fill( t_2tag_Zvv_1 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_2tag_Zvv.Fill( t_2tag_Zvv_2 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_2tag_Zvv.Fill( t_2tag_Zvv_3 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_2tag_Zvv.Fill( t_2tag_Zvv_4 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_2tag_Zvv.Fill( t_2tag_Zvv_5 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_2tag_Zvv.Fill( t_2tag_Zvv_6 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_2tag_Zvv.Fill( t_2tag_Zvv_7 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_2tag_Zvv.Fill( t_2tag_Zvv_8 , HistCollection::CONNECT_ALL , HistCollection::REW_FCN , var == 0 && itr == 0 ? 0 : &rewFcns );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_1 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_2 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_3 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_4 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_5 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_6 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_7 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	  h_4tag_Zvv.Fill( t_4tag_Zvv_8 , HistCollection::CONNECT_ALL , HistCollection::REW_OFF , 0 );
	}
		
	HistCollection h_2tag( h_2tag_data );
	HistCollection h_4tag( h_4tag_data );
	if ( useMC ) {
	  h_2tag.Add( h_2tag_ttLep , -1);
	  h_2tag.Add( h_2tag_ttHad , -1);
	  h_2tag.Add( h_2tag_Zvv   , -1);
	  h_4tag.Add( h_4tag_ttLep , -1);
	  h_4tag.Add( h_4tag_ttHad , -1);
	  h_4tag.Add( h_4tag_Zvv   , -1);
	}
	
	if ( itr == 0 && var == 0 ) muQCD = HistCollection::MuQCD( h_4tag , h_2tag );
	h_2tag.Scale( muQCD );
	h_2tag_data.Scale( muQCD );
	h_2tag_ttLep.Scale( muQCD );
	h_2tag_ttHad.Scale( muQCD );
	h_2tag_Zvv.Scale( muQCD );

	HistCollection ratio = HistCollection::Ratio( h_4tag , h_2tag );

	Reweighter * rew = 0;
	if ( itr != Nitr ) { // itr == Nitr is the extra loop to get plots of the last reweighting, i.e. we don't calculate the reweighting for this iteration
	  std::map<int, Reweighter>::iterator itRew = rewFcns.find( iRewFcn );
	  if ( itRew == rewFcns.end() ) {
	    std::cout << "main() : Error - couldn't find Reweighter, iRewFcn = " << iRewFcn << std::endl;
	    continue;
	  }
	  rew = &(itRew->second);
	  rew->Initialise( ratio );
	  ++iRewFcn;
	}
	
	Plot(h_2tag_data, h_2tag_ttLep, h_2tag_ttHad, h_2tag_Zvv, h_4tag_data, h_4tag_ttLep, h_4tag_ttHad, h_4tag_Zvv, rew, Region::String(regions.at(reg)), itr, var, vecOUTDIR.at(reg));
	
      }
    }
    
    UpdateTree( t_2tag_data , &rewFcns , muQCD , Region::String(regions.at(reg)) );
    if ( useMC ) {
      UpdateTree( t_2tag_ttLep , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_ttHad , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_1 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_2 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_3 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_4 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_5 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_6 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_7 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
      UpdateTree( t_2tag_Zvv_8 , &rewFcns , muQCD , Region::String(regions.at(reg)) );
    }
    
    delete t_2tag_data;
    delete t_4tag_data;
    delete t_2tag_ttLep;
    delete t_4tag_ttLep;
    delete t_2tag_ttHad;
    delete t_4tag_ttHad;
    delete t_2tag_Zvv_1;
    delete t_4tag_Zvv_1;
    delete t_2tag_Zvv_2;
    delete t_4tag_Zvv_2;
    delete t_2tag_Zvv_3;
    delete t_4tag_Zvv_3;
    delete t_2tag_Zvv_4;
    delete t_4tag_Zvv_4;
    delete t_2tag_Zvv_5;
    delete t_4tag_Zvv_5;
    delete t_2tag_Zvv_6;
    delete t_4tag_Zvv_6;
    delete t_2tag_Zvv_7;
    delete t_4tag_Zvv_7;
    delete t_2tag_Zvv_8;
    delete t_4tag_Zvv_8;
    
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
	    TH1F * ttHad_4tag,
	    TH1F * Zvv_4tag,
	    const Reweighter * rew,
	    TString reg,
	    int itr,
	    int var,
	    TString dirname);

void Plot2D(TH2F * data_2tag,
	    TH2F * ttLep_2tag,
	    TH2F * ttHad_2tag,
	    TH2F * Zvv_2tag,
	    TH2F * data_4tag,
	    TH2F * ttLep_4tag,
	    TH2F * ttHad_4tag,
	    TH2F * Zvv_4tag,
	    const Reweighter * rew,
	    TString reg,
	    int itr,
	    int var,
	    TString dirname);  

void Plot(const HistCollection & h_2tag_data,
	  const HistCollection & h_2tag_ttLep,
	  const HistCollection & h_2tag_ttHad,
	  const HistCollection & h_2tag_Zvv,
	  const HistCollection & h_4tag_data,
	  const HistCollection & h_4tag_ttLep,
	  const HistCollection & h_4tag_ttHad,
	  const HistCollection & h_4tag_Zvv,
	  const Reweighter * rew,
	  TString reg,
	  int itr,
	  int var,
	  TString dirname)
{
  
  const std::map<HistInfo, HistBase *> histMap_2tag_data  = h_2tag_data.HistMap(); 
  std::map<HistInfo, HistBase *>::const_iterator it    = histMap_2tag_data.begin();
  std::map<HistInfo, HistBase *>::const_iterator itEnd = histMap_2tag_data.end();
  for ( ; it != itEnd; ++it) {

    const HistInfo & info  = it->first;  
    const TString  & hname = info.Name();  
    
    bool is1D = dynamic_cast<const Hist1D *>(it->second) == 0 ? false : true;
    bool is2D = dynamic_cast<const Hist2D *>(it->second) == 0 ? false : true;

    if ( is1D ) {
      
      TH1F * data_2tag  = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( it->second                      )->Get()->Clone());
      TH1F * ttLep_2tag = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_2tag_ttLep.Histogram( hname ) )->Get()->Clone());
      TH1F * ttHad_2tag = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_2tag_ttHad.Histogram( hname ) )->Get()->Clone());
      TH1F * Zvv_2tag   = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_2tag_Zvv  .Histogram( hname ) )->Get()->Clone());
      TH1F * data_4tag  = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_data .Histogram( hname ) )->Get()->Clone());
      TH1F * ttLep_4tag = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_ttLep.Histogram( hname ) )->Get()->Clone());
      TH1F * ttHad_4tag = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_ttHad.Histogram( hname ) )->Get()->Clone());
      TH1F * Zvv_4tag   = static_cast<TH1F *>(dynamic_cast<const Hist1D *>( h_4tag_Zvv  .Histogram( hname ) )->Get()->Clone());

      Plot1D( data_2tag , ttLep_2tag , ttHad_2tag , Zvv_2tag , data_4tag , ttLep_4tag , ttHad_4tag , Zvv_4tag , rew , reg , itr , var , dirname);

    }
    else if ( is2D ) {

      TH2F * data_2tag  = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( it->second                      )->Get()->Clone());
      TH2F * ttLep_2tag = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_2tag_ttLep.Histogram( hname ) )->Get()->Clone());
      TH2F * ttHad_2tag = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_2tag_ttHad.Histogram( hname ) )->Get()->Clone());
      TH2F * Zvv_2tag   = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_2tag_Zvv  .Histogram( hname ) )->Get()->Clone());
      TH2F * data_4tag  = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_data .Histogram( hname ) )->Get()->Clone());
      TH2F * ttLep_4tag = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_ttLep.Histogram( hname ) )->Get()->Clone());
      TH2F * ttHad_4tag = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_ttHad.Histogram( hname ) )->Get()->Clone());
      TH2F * Zvv_4tag   = static_cast<TH2F *>(dynamic_cast<const Hist2D *>( h_4tag_Zvv  .Histogram( hname ) )->Get()->Clone());

      Plot2D( data_2tag , ttLep_2tag , ttHad_2tag , Zvv_2tag , data_4tag , ttLep_4tag , ttHad_4tag , Zvv_4tag , rew , reg , itr , var , dirname);

    }
    else {
      std::cout << "Plot() : Error - Couldn't determine histogram type for hname = " << hname.Data() << std::endl;
      continue;
    }
    
  }
        
}


void PrepareTH1F(TH1F * h,  int color, int style, int fill, int rebin = 1)
{

  if ( ! h ) return;

  h->SetDirectory(0);
  
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
	    const Reweighter * rew,
	    TString reg,
	    int itr,
	    int var,
	    TString dirname)
{

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
  
  TH1 * frame1 = p1->DrawFrame(data_4tag->GetXaxis()->GetXmin(), 0.01, data_4tag->GetXaxis()->GetXmax(), data_4tag->GetMaximum()*2.);
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
  PrepareTH1F(data_4tag , kBlack      , kSolid , 0, rebin);
  PrepareTH1F(QCD       , kYellow - 4 , kSolid , 1, rebin);
  PrepareTH1F(ttLep_4tag, kAzure - 9  , kSolid , 1, rebin);
  PrepareTH1F(ttHad_4tag, kAzure + 1  , kSolid , 1, rebin);
  PrepareTH1F(Zvv_4tag  , kGreen - 7  , kSolid , 1, rebin);
  PrepareTH1F(ttLep_2tag, kMagenta - 4, kDashed, 0, rebin);
  PrepareTH1F(ttHad_2tag, kMagenta + 1, kDashed, 0, rebin);
  PrepareTH1F(Zvv_2tag  , kGreen - 7  , kSolid , 1, rebin);
  
  data_4tag->SetMarkerStyle(20);
  data_4tag->SetMarkerColor(kBlack);
  data_4tag->SetMarkerSize();
  data_4tag->SetMarkerStyle(20);

  THStack * pred = new THStack("prediction", "prediction");
  if ( Zvv_2tag   && Zvv_2tag  ->Integral() > 0 ) { Zvv_2tag  ->Scale( Zvv_4tag  ->Integral() / Zvv_2tag  ->Integral() ); pred->Add( Zvv_2tag   ); }
  else if ( Zvv_4tag   ) pred->Add( Zvv_4tag   );
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
  tex.DrawLatex(0.36, 0.75, TString::Format(": %0.1f", static_cast<TH1F*>(pred->GetStack()->Last())->Integral(0, -1)));
  
  p2->cd();

  TH1 * frame2 = p2->DrawFrame(data_4tag->GetXaxis()->GetXmin(), (itr == 0 && var == 0 ? 0.01 : 0.31), data_4tag->GetXaxis()->GetXmax(), (itr == 0 && var == 0 ? 2.99 : 1.69));
  frame2->GetXaxis()->SetTitle( data_4tag->GetXaxis()->GetTitle() );
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
  
  TH1F * ratio = static_cast<TH1F *>(data_4tag->Clone());
  ratio->Divide( static_cast<TH1F *>(pred->GetStack()->Last()) );
  ratio->SetLineWidth(1);
  ratio->SetLineColor(kBlack);
  ratio->SetMarkerColor(kBlack);
  ratio->SetMarkerStyle(20);

  ratio->Draw("samepe");

  TLine line(data_4tag->GetXaxis()->GetXmin(), 1.0, data_4tag->GetXaxis()->GetXmax(), 1.0);
  line.SetLineColor(kBlack);
  line.SetLineStyle(kDashed);
  line.Draw("same");

  TH1F         * hist     = nullptr;
  TF1          * fit      = nullptr;
  TSpline5     * spline   = nullptr;
  TGraphErrors * gSmooth  = nullptr;
  TGraphErrors * gCustom  = nullptr;
  TGraphErrors * gCustom2 = nullptr;
  TGraphErrors * gHybrid  = nullptr;
  
  if ( rew ) {
    
    if ( rew->Xvar() == data_4tag->GetName() ) {
      
      if (rew->Method() == Reweighter::HIST1D) {
	hist = static_cast<TH1F *>(rew->GetHist1D()->Clone());
	hist->SetLineColor(kAzure);
	hist->SetLineWidth(2);
	hist->Draw("histsame");
      }
      
      if (rew->Method() == Reweighter::LINFIT) {
	fit = static_cast<TF1 *>(rew->GetFit()->Clone());
	fit->SetLineColor(kBlue - 4);
	fit->SetLineWidth(2);
	fit->Draw("same");
      }
      
      if (rew->Method() == Reweighter::SPLINE) {
	spline = static_cast<TSpline5 *>(rew->GetSpline()->Clone());
	spline->SetLineColor(kOrange);
	spline->SetLineWidth(2);
	spline->Draw("same");
      }
      
      if (rew->Method() == Reweighter::SMOOTH) {
	gSmooth = static_cast<TGraphErrors *>(rew->GetSmooth()->Clone());
	gSmooth->SetLineColor(kGreen + 1);
	gSmooth->SetLineWidth(2);
	gSmooth->Draw("lsame");
      }
      
      if (rew->Method() == Reweighter::CUSTOM) {
	gCustom = static_cast<TGraphErrors *>(rew->GetCustom()->Clone());
	gCustom2 = new TGraphErrors;
	int p = 0;
	for (int i = 1; i <= ratio->GetNbinsX(); ++i) {
	  if ( ratio->GetBinContent(i) == 0. ) continue;
	  float x = ratio->GetBinCenter(i);
	  gCustom2->SetPoint(p, x, gCustom->Eval(x));
	  ++p;
	}
	gCustom ->SetLineColor(kMagenta);
	gCustom2->SetLineColor(kMagenta);
	gCustom->SetLineWidth(2);
	gCustom2->SetLineWidth(1);
	gCustom->Draw("lsame");
	gCustom2->Draw("lsame");
      }
      
      if (rew->Method() == Reweighter::HYBRIDCL) {
	gHybrid = static_cast<TGraphErrors *>(rew->GetHybrid()->Clone());
	gHybrid->SetLineColor(kRed);
	gHybrid->SetLineWidth(2);
	gHybrid->Draw("lsame");
      }

    }
    
  }

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
  
 
  c->SaveAs( TString::Format("%s/H1D/pdf/%s_rew-%d-%d__LinScale.pdf"  , dirname.Data(), data_4tag->GetName(), itr, var) );
  c->SaveAs( TString::Format("%s/H1D/eps/%s_rew-%d-%d__LinScale.eps"  , dirname.Data(), data_4tag->GetName(), itr, var) );
  c->SaveAs( TString::Format("%s/H1D/root/%s_rew-%d-%d__LinScale.root", dirname.Data(), data_4tag->GetName(), itr, var) );

  frame1->SetMaximum(data_4tag->GetMaximum()*5000.);
  p1->SetLogy();
  c->SaveAs( TString::Format("%s/H1D/pdf/%s_rew-%d-%d__LogScale.pdf"  , dirname.Data(), data_4tag->GetName(), itr, var) );
  c->SaveAs( TString::Format("%s/H1D/eps/%s_rew-%d-%d__LogScale.eps"  , dirname.Data(), data_4tag->GetName(), itr, var) );
  c->SaveAs( TString::Format("%s/H1D/root/%s_rew-%d-%d__LogScale.root", dirname.Data(), data_4tag->GetName(), itr, var) );
    
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
  delete pred;
  delete ratio;
  delete hist;
  delete fit;
  delete spline;
  delete gSmooth;
  delete gCustom;
  delete gCustom2;
  delete gHybrid;
  delete p1;
  delete p2;
  delete c;

}


void PrepareTH2F(TH2F * h)
{

  if ( ! h ) return;

  h->SetDirectory(0);
  
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
	    const Reweighter * rew,
	    TString reg,
	    int itr,
	    int var,
	    TString dirname)
{

  
  TCanvas * c = new TCanvas("c", "c", 1000, 800);
  c->cd();

  PrepareTH2F( data_4tag  );
  PrepareTH2F( data_2tag  );
  PrepareTH2F( ttLep_4tag );
  PrepareTH2F( ttHad_4tag );
  PrepareTH2F( Zvv_4tag   );
  
  TString hname = data_4tag->GetName();

  data_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/Data_%s_rew-%d-%d__LinScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/Data_%s_rew-%d-%d__LinScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/Data_%s_rew-%d-%d__LinScale.root", dirname.Data(), hname.Data(), itr, var) );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/Data_%s_rew-%d-%d__LogScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/Data_%s_rew-%d-%d__LogScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/Data_%s_rew-%d-%d__LogScale.root", dirname.Data(), hname.Data(), itr, var) );
  
  data_2tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/Multijet_%s_rew-%d-%d__LinScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/Multijet_%s_rew-%d-%d__LinScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/Multijet_%s_rew-%d-%d__LinScale.root", dirname.Data(), hname.Data(), itr, var) );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/Multijet_%s_rew-%d-%d__LogScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/Multijet_%s_rew-%d-%d__LogScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/Multijet_%s_rew-%d-%d__LogScale.root", dirname.Data(), hname.Data(), itr, var) );
  
  ttLep_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/ttLep_4tag_%s_rew-%d-%d__LinScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/ttLep_4tag_%s_rew-%d-%d__LinScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/ttLep_4tag_%s_rew-%d-%d__LinScale.root", dirname.Data(), hname.Data(), itr, var) );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/ttLep_4tag_%s_rew-%d-%d__LogScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/ttLep_4tag_%s_rew-%d-%d__LogScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/ttLep_4tag_%s_rew-%d-%d__LogScale.root", dirname.Data(), hname.Data(), itr, var) );
  
  ttHad_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/ttHad_4tag_%s_rew-%d-%d__LinScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/ttHad_4tag_%s_rew-%d-%d__LinScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/ttHad_4tag_%s_rew-%d-%d__LinScale.root", dirname.Data(), hname.Data(), itr, var) );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/ttHad_4tag_%s_rew-%d-%d__LogScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/ttHad_4tag_%s_rew-%d-%d__LogScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/ttHad_4tag_%s_rew-%d-%d__LogScale.root", dirname.Data(), hname.Data(), itr, var) );

  Zvv_4tag->Draw("colz");
  c->SetLogz(0);
  c->SaveAs( TString::Format("%s/H2D/pdf/Zvv_4tag_%s_rew-%d-%d__LinScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/Zvv_4tag_%s_rew-%d-%d__LinScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/Zvv_4tag_%s_rew-%d-%d__LinScale.root", dirname.Data(), hname.Data(), itr, var) );
  c->SetLogz();
  c->SaveAs( TString::Format("%s/H2D/pdf/Zvv_4tag_%s_rew-%d-%d__LogScale.pdf"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/eps/Zvv_4tag_%s_rew-%d-%d__LogScale.eps"  , dirname.Data(), hname.Data(), itr, var) );
  c->SaveAs( TString::Format("%s/H2D/root/Zvv_4tag_%s_rew-%d-%d__LogScale.root", dirname.Data(), hname.Data(), itr, var) );


  
  delete c;
  delete data_2tag;
  delete ttLep_2tag;
  delete ttHad_2tag;
  delete Zvv_2tag;
  delete data_4tag;
  delete ttLep_4tag;
  delete ttHad_4tag;
  delete Zvv_4tag;
 
}


void UpdateTree(TTree * tree,
		const std::map<int, Reweighter> * rewFcns,
		float muQCD,
		const TString & region)
{

  if ( ! tree ) {
    std::cout << "updateTree() : Tree is nullptr " << std::endl;
    return;
  }

  std::vector<TString> variables_empty;
  Event event;
  event.ConnectAllVariables( tree , variables_empty );

  float kinWeight;
  TBranch * b_kinWeight_R1D = tree->Branch("kinWeight_R1D_" + region, &kinWeight);
  TBranch * b_muQCD         = tree->Branch("muQCD_R1D_"     + region, &muQCD    );
  
  std::cout << "UpdateTree() : Looping over events (" << tree->GetName() << ") : "  << tree->GetEntries() << std::endl;
  
  for (long ievent = 0; ievent < tree->GetEntries(); ++ievent) {
    
    // get event
    tree->GetEntry(ievent);

    // calculate additional variables
    event.CalculateAdditionalVariables();

    // get weight
    kinWeight = 1.;
    if ( rewFcns ) {
      std::map<int, Reweighter>::const_iterator it    = rewFcns->begin();
      std::map<int, Reweighter>::const_iterator itEnd = rewFcns->end();
      for ( ; it != itEnd; ++it) {
	const Reweighter & rew  = it->second;
	const TString    & xvar = rew.Xvar();
	const TString    & yvar = rew.Yvar();
	float x = 0.;
	float y = 0.;
	if ( xvar.Length() > 0 ) {
	  if      ( event.GetType( xvar ) == typeid(float).name() ) x = event.GetVar<float>( xvar );
	  else if ( event.GetType( xvar ) == typeid(int  ).name() ) x = event.GetVar<int>  ( xvar );
	  else  {
	    std::cout << "UpdateTree() : Error - Type(xvar) = " << event.GetType( xvar ) << std::endl;
	    throw(0);
	  }
	}
	if ( yvar.Length() > 0 ) {
	  if      ( event.GetType( yvar ) == typeid(float).name() ) y = event.GetVar<float>( yvar );
	  else if ( event.GetType( yvar ) == typeid(int  ).name() ) y = event.GetVar<int>  ( yvar );
	  else  {
	    std::cout << "UpdateTree() : Error - Type(yvar) = " << event.GetType( yvar ) << std::endl;
	    throw(0);
	  }
	}
	kinWeight *= rew.Eval( x , y );
	// std::cout << "xvar = " << xvar.Data() << " = " << x << "  rew.Eval() = " << rew.Eval( x , y ) << "   kinWeight = " << kinWeight << std::endl;
      }
    }
    
    // fill branch
    b_kinWeight_R1D->Fill();
    b_muQCD        ->Fill();
    
  }

  tree->Write();
  
}
