//
//  MultiThreading/LockedPtr.h
//  crookie
//
//  Created by Paolo Bernini on 15/03/14.
//  Copyright (c) 2014 Paolo Bernini. All rights reserved.
//

#ifndef crookie_LockedPtr_h
#define crookie_LockedPtr_h

namespace crookie {
  
  /**
   * @brief Locked reference to complex data structures.
   * 
   * This class provides a straightforward method to give locked access to 
   * any complex data structure to the users of the owner of the data structure
   * without delegating the user itself to make explicit lock() and unlock() 
   * around the read or write operations.
   *
   * @b Usage:
   *
   * @code{.cpp}
     #include <LockPolicy.h>
     #include <LockedPtr.h>
   
     using namespace crookie;
   
     struct MyData { int a, b; };
   
     class MyManager
     {
       MyData data;
       crookie::SimpleLock::Mutex mutex;
     public:
       typedef LockedPtr<MyData, SimpleLock> DataHandle;
       DataHandle getData() { return DataHandle(&data, mutex); }
     };
   
     void foo()
     {
       [...]
       MyManager::DataHandle handle = someInstance.getData();
       // locked operations on shared data here
       // autorealease at block exit !!
     }
     @endcode
   *
   * @tparam T  Type of the shared data structure.
   * @tparam LockPolicy Policy class for mutual exclusion management of shared
   * data. It must expose:
   * @li @c Mutex the type of the mutex used for lock/unlock ops.
   * @li @c lock(Mutex&) static function that locks a mutex of the type above.
   * @li @c unlock(Mutex&) static function that unlocks same mutex.
   * @li @c Traits<T>::DataPtr type of pointer to an instance of the locked 
   * object (i.e. T*, or T const* for shared/read locks).
   *
   * @note
   * LockedPtr is completely Lock agnostic: crookie already provides a 
   * SimpleLock LockPolicy based on c++11 mutexes but you can define your own
   * LockPolicies using any kind of mutex provided by your developing framework
   * (for example: shared_lock from c++14 or QReadWriteLock from Qt), Simply 
   * note that to implement an effective LockedPtr with shared/read semantic
   * you shall define two LockPolicies: one that performs a shared/read lock
   * and one that performs a unique/read lock, thus requesting specific 
   * LockerPtr with shared or unique locker based on what is needed.
   */
  template <class T, class LockPolicy>
  class LockedPtr
  {
  public:
    
    //! @brief Name alias for shared data type.
    typedef T Data;
    
    //! @brief Name alias for lock policy structure.
    typedef LockPolicy Locker;
    
    //! @brief Type of Mutex, provided by LockPolicy.
    typedef typename LockPolicy::Mutex Mutex;
    
    //! @brief Type of Pointer to shared Data object, provided by LockPolicy.
    typedef typename LockPolicy::template Traits<T>::DataPtr DataPtr;
    
    
    // constructor and destructor
    
    //! @brief Construct a locked reference to given @a data (blocking).
    //! @param [in] data    pointer to shared data structure.
    //! @param [in] mutex   mutex to use to acquire/release locks on given data.
    LockedPtr(DataPtr data, Mutex& mutex) noexcept
      : data_(data), mutex_(mutex)
    {
      Locker::lock(mutex_);
    }
    
    //! @brief Unlock the data mutex
    ~LockedPtr()
    {
      if (data_ != NULL) Locker::unlock(mutex_);
    }
    
    
    // move constructor and move assignement operator
    
    //! @brief Move constructor. Ensure data ownership from temporary objects
    //! to the effective reference instance.
    LockedPtr(LockedPtr&& temp)
      : data_(temp.data_), mutex_(temp.mutex_)
    {
      temp.data_ = NULL;
    }
    
    //! @brief Move assignement operator. Ensure data ownership from temporary
    //! objects to the effective reference instance.
    LockedPtr& operator =(LockedPtr&& temp)
    {
      this->data_ = temp.data_;
      this->mutex_ = temp.mutex_;
      temp.data_ = NULL;
    }
    
    // data access operators
    
    //! Return const pointer to referenced data object.
    const Data* get() const noexcept { return data_; }
    
    //! Return pointer to refereced data object
    Data* get() noexcept { return data_; }
    
    //! const arrow operator
    const Data* operator ->() const { return data_; }
    
    //! arrow operator
    Data* operator ->() { return data_; }
    
  private:
    
    DataPtr data_;  //!< Pointer to shared data object
    Mutex& mutex_;  //!< Mutex instance for locking/unlocking operations
    
    
    // copy and assignement disabled
    
    LockedPtr(const LockedPtr&);
    LockedPtr& operator =(const LockedPtr&);
  };

  
} // end of namespace

#endif
