#ifndef STATIC_AEVENTHANDLER_H
#define STATIC_AEVENTHANDLER_H

#include "StaticBus.h"
#include "../Dinamic/AEventHandler.h"


namespace crookie {
namespace sbus {
  
  //! @brief Basic class for event handlers on StaticBus
  template < unsigned int buscode, class EventType >
  class AEventHandler : public crookie::AEventHandler< EventType >
  {
  public:

    typedef StaticBus< buscode > Bus;   //!< Handler owner bus type

    AEventHandler()
      : crookie::AEventHandler< EventType >(Bus::instance())
    { }
  };

  
}}

#endif // STATIC_AEVENTHANDLER_H
