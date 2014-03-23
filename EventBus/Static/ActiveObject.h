#ifndef STATIC_ACTIVEOBJECT_H
#define STATIC_ACTIVEOBJECT_H

#include "../Dynamic/ActiveObject.h"
#include "StaticBus.hpp"


namespace crookie {
namespace sbus {

  template < unsigned int buscode, class Event >
  class ActiveObject : public crookie::ActiveObject< Event >
  {
  public:

    typedef StaticBus< buscode > Bus;

    ActiveObject()
      : crookie::ActiveObject< Event >(Bus::instance())
    { }
  };

  
}}

#endif // STATIC_ACTIVEOBJECT_H
