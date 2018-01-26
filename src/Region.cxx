// local includes
#include "Region.h"
#include "Event.h"


bool Region::SelectSR(const Event & event)
{

  return event.GetVar<float>("xhh_SR") < 1.6;
  //SelectXhh(event, 120.0, 110.0,  1.6 ) ;
  //return SelectXhh(event, 120.0, 110.0,  2.0 ) ;

}


bool Region::SelectVR1(const Event & event)
{

  return event.GetVar<float>("xhh_VR1") < 1.4;
  //SelectXhh(event,  96.0,  88.0,  1.4 ) ;
  //return SelectXhh(event,  85.0,  80.0,  2.25 ) ;

}


bool Region::SelectVR2(const Event & event)
{

  return event.GetVar<float>("xhh_VR2") < 1.25;
  //SelectXhh(event, 149.0, 137.0,  1.25) ;
  //return SelectXhh(event, 155.0, 145.0,  1.5) ;

}


bool Region::SelectCR(const Event & event)
{

  return event.GetVar<float>("rhh_CR") < 55 && ! SelectSR(event);
  //SelectRhh(event, 126.0, 115.5, 55.0 ) && ! SelectSR(event) ;
  //return SelectRhh(event, 125.0, 115.0, 70.0 ) && ! SelectSR(event) ;

}


bool Region::SelectCRexclVR1(const Event & event)
{

  return SelectCR(event) && ! SelectVR1(event) ;

}


bool Region::SelectCRexclVR2(const Event & event)
{

  return SelectCR(event) && ! SelectVR2(event) ;

}


bool Region::SelectSRexclVR1(const Event & event)
{

  return SelectSR(event) && ! SelectVR1(event) ;

}


bool Region::SelectSRexclVR2(const Event & event)
{

  return SelectSR(event) && ! SelectVR2(event) ;

}
  
