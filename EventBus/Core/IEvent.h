#ifndef IEVENT_H
#define IEVENT_H

// forward declarations


namespace crookie {

class IEventHandler;
  
template <class EventType> class AEventHandler;
  

class IEvent
{
public:

  //! @brief Destructor
  virtual ~IEvent() { }

  //! @brief Unique event identifier
  virtual int type() const = 0;

  //! @brief Routine demanded to call the handler on last instance.
  //! @param handler [in]     handler on whom call onEvent(ActualEventType&)
  virtual void dispatch(IEventHandler& handler) const = 0;
  
protected:
  
  static int getTypeId()
  {
    static int typecounter = 0;
    return ++typecounter;
  }
};

  
} // end of namespace


#endif
