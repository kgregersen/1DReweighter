// local includes
#include "EventSelector.h"


EventSelector::EventSelector() :
 m_tagdef(TagBit::NOTAGDEF),
 m_wp(TagBit::NOWP),
 m_region(Region::NOREGION),
 m_trigbit2016(TrigBit::ALLPASS),
 m_lepVeto(false),
 m_xwtVeto(false)
{

}


EventSelector::EventSelector(TagBit::TAGDEF tagdef, TagBit::WP wp, Region::TYPE region, TrigBit::TYPE trigbit2016, bool lepVeto, bool xwtVeto) :
  m_tagdef(tagdef),
  m_wp(wp),
  m_region(region),
  m_trigbit2016(trigbit2016),
  m_lepVeto(lepVeto),
  m_xwtVeto(xwtVeto)
{

}


EventSelector::EventSelector(const EventSelector & other) :
  m_tagdef(other.m_tagdef),
  m_wp(other.m_wp),
  m_region(other.m_region),
  m_trigbit2016(other.m_trigbit2016),
  m_lepVeto(other.m_lepVeto),
  m_xwtVeto(other.m_xwtVeto)
{

}


void EventSelector::SetLepVeto(bool apply)
{

  m_lepVeto = apply;
  
}


void EventSelector::SetXWtVeto(bool apply)
{

  m_xwtVeto = apply;
  
}


bool EventSelector::operator()(const Event & event) const
{

  // hadronic top veto
  float Xt1 = event.GetVar<float>("Xt1");
  if ( m_xwtVeto && Xt1 < 1.8 ) return false;

  // leptonic top veto
  int   ele_n        = event.GetVar<int>("electron_n");
  int   muo_n        = event.GetVar<int>("muon_n");
  float ele_veto_pt  = event.GetVar<float>("electron_veto_pt");
  float muon_veto_pt = event.GetVar<float>("muon_veto_pt");
  if ( m_lepVeto && ((ele_n > 0 && ele_veto_pt >= 20.) || (muo_n > 0 && muon_veto_pt >= 20.)) ) return false;
  
  // trigger selection
  if ( ! TrigBit::Pass2016( event, m_trigbit2016 ) ) return false;
  
  // tag selection
  if ( m_tagdef == TagBit::NOTAGDEF                                                    ) return false;
  if ( m_tagdef == TagBit::TAG2         && ! TagBit::Select2Tag        ( event, m_wp ) ) return false;
  if ( m_tagdef == TagBit::TAG4         && ! TagBit::Select4Tag        ( event, m_wp ) ) return false;
  if ( m_tagdef == TagBit::TAG2SPLIT    && ! TagBit::Select2TagSplit   ( event, m_wp ) ) return false;
  if ( m_tagdef == TagBit::TAG2NONSPLIT && ! TagBit::Select2TagNonSplit( event, m_wp ) ) return false;
  if ( m_tagdef == TagBit::TAG3         && ! TagBit::Select3Tag        ( event, m_wp ) ) return false;
  
  // region selection
  if ( m_region == Region::NOREGION                                        ) return false;
  if ( m_region == Region::SR        && ! Region::SelectSR       ( event ) ) return false;
  if ( m_region == Region::CR        && ! Region::SelectCR       ( event ) ) return false;
  if ( m_region == Region::VR1       && ! Region::SelectVR1      ( event ) ) return false;
  if ( m_region == Region::VR2       && ! Region::SelectVR2      ( event ) ) return false;
  if ( m_region == Region::CRexclVR1 && ! Region::SelectCRexclVR1( event ) ) return false;
  if ( m_region == Region::CRexclVR2 && ! Region::SelectCRexclVR2( event ) ) return false;
  if ( m_region == Region::SRexclVR1 && ! Region::SelectSRexclVR1( event ) ) return false;
  if ( m_region == Region::SRexclVR2 && ! Region::SelectSRexclVR2( event ) ) return false;

  // pass
  return true;
  
}
