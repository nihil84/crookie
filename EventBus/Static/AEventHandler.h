#ifndef STATIC_AEVENTHANDLER_HPP
#define STATIC_AEVENTHANDLER_HPP

#include "StaticBus.hpp"
#include "../Dinamic/AEventHandler.hpp"


namespace SGEB {
  
  //! @brief Basic class for event handlers on StaticBus
  template < unsigned int buscode, class EventType >
  class AEventHandler : public DBus::AEventHandler< EventType >
  {
  public:

    typedef StaticBus< buscode > Bus;   //!< Handler owner bus type

    AEventHandler()
        : DBus::AEventHandler< EventType >(Bus::instance())
    { }
  };

  
}

#endif // STATIC_AEVENTHANDLER_HPP
