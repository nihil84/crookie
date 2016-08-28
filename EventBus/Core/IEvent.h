#ifndef IEVENT_H
#define IEVENT_H

// forward declarations


namespace crookie {

class IEventDispatcher;
  

class IEvent
{
public:
  
  static const int INVALID_TYPE = -1;

  //! @brief Destructor
  virtual ~IEvent() { }

  //! @brief Unique event identifier
  virtual int type() const = 0;

  //! @brief Routine demanded to call the handler on last instance.
  //! @param handler [in]     handler on whom call onEvent(ActualEventType&)
  virtual void dispatch(IEventDispatcher& dispatcher) const = 0;
  
protected:
  
  static int getTypeId()
  {
    static int typecounter = 0;
    return ++typecounter;
  }
};

  
} // end of namespace


#endif
