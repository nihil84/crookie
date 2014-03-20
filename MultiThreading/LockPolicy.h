//
//  LockPolicy.h
//  crookie
//
//  Created by Paolo Bernini on 15/03/14.
//  Copyright (c) 2014 Paolo Bernini. All rights reserved.
//

#ifndef crookie_LockPolicy_h
#define crookie_LockPolicy_h

#include <mutex>


namespace crookie {
  
  /**
   * @brief Basic LockPolicy for LockedPtr with std::mutex.
   * 
   * Grants unique (write) ownership of shared data allowing for both read
   * write operations.
   *
   * @note
   * You can define your own LockPolicy/ies by following this class as template.
   */
  struct SimpleLock
  {
    //! Mutex type used for lock/unlock operations.
    typedef std::mutex Mutex;
    
    //! Dependent type definition(s).
    template <class T> struct Traits
    {
      //! Type of the pointer to use for data objects locked by this Lock
      typedef T* DataPtr;
    };
    
    //! @brief Perform a lock operation on given mutex.
    //! @param [in] mutex   mutex object to lock.
    static void lock(Mutex& mutex) { mutex.lock(); }
    
    //! @brief Perform an unlock operation on given mutex.
    //! @param [in] mutex   mutex object to unlock.
    static void unlock(Mutex& mutex) { mutex.unlock(); }
  };
  
  
  //! @todo implement a read/write (shared) mutex and relative locks
  
} // end of namespace

#endif
