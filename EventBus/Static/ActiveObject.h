#ifndef STATIC_ACTIVEOBJECT_HPP
#define STATIC_ACTIVEOBJECT_HPP

#include "../Dynamic/ActiveObject.hpp"
#include "StaticBus.hpp"


namespace SGEB {

    template < unsigned int buscode, class Event >
    class ActiveObject : public GEB::ActiveObject< Event >
    {
    public:

        typedef StaticBus< buscode > Bus;

        ActiveObject()
            : GEB::ActiveObject< Event >(Bus::instance())
        { }
    };

} // end of namespace SGEB

#endif // STATIC_ACTIVEOBJECT_HPP
