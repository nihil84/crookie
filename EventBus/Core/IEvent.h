#ifndef IEVENT_H
#define IEVENT_H

// forward declarations


namespace crookie {

  class IEventHandler;

/**
 * @brief Event base interface
 *
 * Any EventBus Event needs to extend this class, to expose a public static
 * TYPE member of type EventType and to declare some other fancy stuff that you
 * can completely ignore by using the MACRO #BUS_EVENT(EventClass), and filling
 * its argument with your event class-name, as the axample below.
 *
 * @code{.cpp}
   // file MyEvent.h
   #include <IEvent.h>
 
   class MyEvent : public IEvent
   {
       BUS_EVENT(MyEvent)
 
   public:
 
       static const EventType TYPE;
       virtual int type() const { return TYPE; }
   }
   @endcode
 *
 * Always follow the example above and remember to instance somewhere in a .cpp
 * the static member @a TYPE as follows:
 * @code
   // file MyEvent.cpp
   const EventType MyEvent::TYPE;
   @endcode
 *
 * Any other detail of MyEvent class (constructor parameters, public methods,
 * data members, etc...) is up to you.
 *
 * @note
 * Event classes are in most cases pretty light objects (with few or none data
 * members and methods), if this is your case you can collect all @a TYPE
 * instances in a single ''event.cpp'' module.
 */
class IEvent
{
public:

    //! @brief Destructor
    virtual ~IEvent() { }

    //! @brief Unique event identifier
    virtual int type() const = 0;

    //! @brief Routine demanded to call the handler on last instance.
    //! @param handler [in]     handler on whom call onEvent(ActualEventType&)
    //! @note This pure virtual method is implemented by #BUS_EVENT(EventClass)
    virtual void dispatch(IEventHandler& handler) const = 0;
};

//! @brief Automatic Event type code generator.
class EventType
{
public:

    //! @brief Default constructor
    EventType()
        : type_(typecounter_++) { }

    //! @brief int implicit conversion operator
    operator int() const { return type_; }

private:

    static int typecounter_;
    int type_;
};
  
}

#endif
