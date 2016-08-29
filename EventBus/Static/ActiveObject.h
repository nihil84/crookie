#ifndef STATIC_ACTIVEOBJECT_H
#define STATIC_ACTIVEOBJECT_H

#include "StaticBus.h"
#include "../ActiveObject.h"


namespace crookie {
namespace sbus {

template < unsigned int buscode, class... EventTypes >
class ActiveObject : public crookie::ActiveObject< EventTypes... >
{
public:

  typedef StaticBus< buscode > Bus;

  ActiveObject()
    : crookie::ActiveObject< EventTypes... >(Bus::instance())
  { }
};

  
}}

#endif // STATIC_ACTIVEOBJECT_H
