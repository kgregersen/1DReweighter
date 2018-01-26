#ifndef __EVENTSELECTOR__
#define __EVENTSELECTOR__

// local includes
#include "TagBit.h"
#include "TrigBit.h"
#include "Region.h"
#include "Event.h"


class EventSelector{

public:

  EventSelector();
  EventSelector(TagBit::TAGDEF tagdef, TagBit::WP wp, Region::TYPE region, TrigBit::TYPE trigbit2016, bool lepVeto, bool xwtVeto);
  EventSelector(const EventSelector & other);
  
  bool operator()(const Event & event) const;

  Region::TYPE   REGION () const { return m_region; }
  TagBit::TAGDEF TAGDEF () const { return m_tagdef; }
  TagBit::WP     WP     () const { return m_wp; }
  TrigBit::TYPE  TRIGBIT() const { return m_trigbit2016; }

  void SetLepVeto(bool apply);
  void SetXWtVeto(bool apply);

  
private:
  
  const TagBit::TAGDEF m_tagdef;
  const TagBit::WP     m_wp;
  const Region::TYPE   m_region;
  const TrigBit::TYPE  m_trigbit2016;
  bool m_lepVeto;
  bool m_xwtVeto;
  
};


#endif
