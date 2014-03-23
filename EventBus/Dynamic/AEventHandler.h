#ifndef DYNAMIC_AEVENTHANDLER_HPP
#define DYNAMIC_AEVENTHANDLER_HPP

#include "../Core/IEventHandler.h"
#include "EventBus.h"


namespace crookie {

  /**
   * @brief Event handler base class
   * @tparam EventType    Type of the Event handled by this class.
   */
  template <class EventType>
  class AEventHandler : public IEventHandler
  {
  public:

      //! @brief Subscribes this handler to given bus.
      //! @param [in] bus   Bus on which subscribe for \a EventType events.
      AEventHandler(EventBus& bus)
          : owner_(bus.subscribe(*this))
      { }

      //! @brief Unsubscribe from EventBus.
      ~AEventHandler() { unsubscribe(); }

      //! @brief Return the type of handled events
      virtual int type() const { return EventType::TYPE; }

      //! @brief Handler routine.
      virtual void onEvent(const EventType& event) = 0;

  protected:

      //! @brief Early unsubscribe from EventBus.
      //! @warning NO re-subscribe is allowed.
      bool unsubscribe() { return owner_.unsubscribe(); }

  private:

      EventBus::Account owner_;
  };

} // end of namespace DBus




#endif // DYNAMIC_AEVENTHANDLER_HPP
