#ifndef __HISTCOLLECTION__
#define __HISTCOLLECTION__

// STL includes
#include <map>

// ROOT includes
#include "TString.h"

// local includes
#include "TagBit.h"
#include "TrigBit.h"
#include "Region.h"
#include "EventSelector.h"
#include "HistInfo.h"

// forward declarations
class HistBase;
class Reweighter;
class TTree;
class TH1;


class HistCollection {

public:

  enum REW_MODE {
    REW_OFF,
    REW_FCN,
    REW_READ,
    REW_READ_MUONLY
  };
  
  enum TREE_MODE {
    CONNECT_ALL,
    CONNECT_SUB
  };
  
  HistCollection(const TString & name);
  HistCollection(const TString & name, TagBit::TAGDEF tagdef, TagBit::WP wp, Region::TYPE region, TrigBit::TYPE trigbit, bool lepVeto, bool xwtVeto, const std::vector<TString> & variables);
  HistCollection(const HistCollection & other);
  ~HistCollection();

  void Fill(TTree * tree, TREE_MODE treeMode = CONNECT_ALL, REW_MODE rewMode = REW_OFF, const std::map<int, Reweighter> * rewFcns = 0);
  void Divide(const HistCollection & other);
  void Scale(float scale);
  void Add(const HistCollection & other, float scale);
  void Renormalize(const std::map<HistInfo, float> & renormScale);
  const HistBase * Histogram(const TString & name) const;
  const TString & Name() const { return m_name; }
  const std::map<HistInfo, HistBase *> & HistMap() const;

  Region::TYPE   REGION () const;
  TagBit::TAGDEF TAGDEF () const;
  TagBit::WP     WP     () const;
  TrigBit::TYPE  TRIGBIT() const;
  
  static HistCollection Ratio(const HistCollection & numerator, const HistCollection & denominator);
  static float MuQCD(const HistCollection & numerator, const HistCollection & denominator);
  static std::map<HistInfo, float> * RenormScale(const HistCollection & numerator, const HistCollection & denominator);
  
  
private:

  const TString m_name;
  EventSelector m_select;
  std::map<HistInfo, HistBase *> m_histMap;
  
};

#endif
