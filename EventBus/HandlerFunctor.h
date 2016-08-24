//
//  HandlerFunctor.h
//  crookie
//
//  Created by Paolo Bernini on 24/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//

#ifndef HANDLERFUNCTOR_H
#define HANDLERFUNCTOR_H

#include "AEventHandler.h"

#include <functional>
#include <cassert>


namespace crookie {

/**
 * @brief Allow to register an handler fot @a EventType whitout deriving from
 * AEventHandler.
 *
 * The HandlerFunctor class will subscribe on the given bus for the event 
 * @a EventType and, until destroyed, it will deliver to the specified handler
 * function any @a EventType instance dispatched onto the bus.
 */
template <class EventType>
class HandlerFunctor : public AEventHandler<EventType>
{
public:
  
  //! Type of the event handled by this instance of the template.
  typedef EventType HandledEvent;
  
  //! Type of the handler function
  using HandlerFunction = std::function<void(const HandledEvent&)>;

  //! @brief Construct an empty functor.
  //! @note Empty functors cannot subscribe for events on any bus
  HandlerFunctor() {}
  
  //! @brief Construct an Handler with given @a fun subscribed to @a bus
  //! @param [in] bus   reference to the bus to subscribe for events.
  //! @param [in] fun   function to call back on events.
  HandlerFunctor(EventBus& bus, const HandlerFunction& fun)
    : AEventHandler<HandledEvent>(bus)
    , m_fun(fun)
  { }
  
  //! @brief Construct an Handler with given @fun not yet subscribed to any bus.
  //! @param [in] fun   function to call back on events.
  explicit HandlerFunctor(const HandlerFunction& fun)
    : m_fun(fun)
  { }
  
  //! @brief Convenience constructor to use for member functions as handler.
  //! This constructor is equivalent to
  //! HandlerFunction(EventBus&, const HandlerFunction&) where handler function
  //! is @a mfun and it has its class pointer binded to the instance @a obj.
  //! @param [in] bus   reference to the bus to subscribe for events.
  //! @param [in] obj   instance of the class the function is member of.
  //! @param [in] mfun  function to call on events.
  template <class T>
  HandlerFunctor(EventBus& bus, T* obj, void (T::*mfun)(const HandledEvent&))
    : HandlerFunctor(bus, std::bind(mfun, obj, std::placeholders::_1))
  { }
  
  //! @brief Convenience constructor to use for member functions as handler.
  //! This constructor is equivalent to
  //! HandlerFunction(const HandlerFunction&) where handler function
  //! is @a mfun and it has its class pointer binded to the instance @a obj.
  //! @param [in] obj   instance of the class the function is member of.
  //! @param [in] mfun  function to call on events.
  template <class T>
  HandlerFunctor(T* obj, void (T::*mfun)(const HandledEvent&))
    : HandlerFunctor(std::bind(mfun, obj, std::placeholders::_1))
  { }
  
  //! Move constructor
  HandlerFunctor(HandlerFunctor&& rv);
  
  //! Move assignement operator
  HandlerFunctor& operator =(HandlerFunctor&& rv);
  
  //! @brief Checks if a valid target is constained.
  //! @return True if this handler is valid, false otherwise.
  explicit operator bool() const { return (bool)m_fun; }
  
  //! @brief Subscribe this handler on given bus.
  //! If it was subscribed on another bus, the old subscription is removed.
  //! @param [in] bus   A reference to the bus where to perform the subscription
  //! @return A reference to the bus the handler was subscribed before or
  //! nullptr if none.
  //! @throw std::logic_error if trying to subscribe an invalid handler
  EventBus* subscribe(EventBus& bus)
  {
    if (!m_fun)
      throw std::logic_error("an empty HandlerFunctor cannot be used in a bus");
    
    return AEventHandler<HandledEvent>::subscribe(bus);
  }
  
  //! @brief Early unsubscribe from EventBus.
  bool unsubscribe()
  {
    if (!m_fun)
    {
      assert(AEventHandler<HandledEvent>::m_owner == nullptr);
      return true; // no subcriptions active
    }
    
    return AEventHandler<HandledEvent>::unsubscribe();
  }
  
  //! @brief Event bypass function
  virtual void onEvent(const HandledEvent& event) override
  {
    m_fun(event);
  }
  
private:
  
  HandlerFunction m_fun;
  
  HandlerFunctor(const HandlerFunctor&) = delete;
  HandlerFunctor& operator =(const HandlerFunctor&) = delete;
};
  

//- move constructor ---------------------------------------------------------//
template <class EventType>
HandlerFunctor<EventType>::HandlerFunctor(HandlerFunctor<EventType>&& rv)
{
  if (rv)
  {
    this->m_fun = rv.m_fun;
    this->subscribe(*rv.m_owner);
    
    rv.unsubscribe();
    rv.m_fun = HandlerFunction(); // reset rvalue functor
  }
}

  //- move operator ----------------------------------------------------------//
template <class EventType>
HandlerFunctor<EventType>& HandlerFunctor<EventType>::operator =(
      HandlerFunctor<EventType>&& rv)
{
  assert(this != &rv);
  
  this->unsubscribe();
  this->m_fun = rv.m_fun;
  
  if (rv)
  {
    this->subscribe(*rv.m_owner);
    
    rv.unsubscribe();
    rv.m_fun = HandlerFunction(); // reset rvalue functor
  }
  
  return *this;
}
  
} // end of namespace

#endif /* HANDLERFUNCTOR_H */
