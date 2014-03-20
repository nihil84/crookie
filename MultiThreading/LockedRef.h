//
//  LockedRef.h
//  crookie
//
//  Created by Paolo Bernini on 15/03/14.
//  Copyright (c) 2014 Paolo Bernini. All rights reserved.
//

#ifndef crookie_Handle_h
#define crookie_Handle_h

namespace crookie {
  
  /**
   * @brief Locked reference to complex data structures.
   * 
   */
  template <class T, class LockPolicy>
  class LockedRef
  {
  public:
    
    typedef T Data;
    
    typedef LockPolicy Locker;
    
    typedef typename LockPolicy::Mutex Mutex;
    
    typedef typename LockPolicy::template Traits<T>::DataPtr DataPtr;
    
    
    // constructor and destructor
    
    LockedRef(DataPtr data, Mutex& mutex)
      : data_(data), mutex_(mutex)
    { Locker::lock(mutex_); }
    
    ~LockedRef()
    {
      if (data_ != NULL) Locker::unlock(mutex_);
    }
    
    
    // move constructor and move assignement operator
    
    LockedRef(LockedRef&& temp)
      : data_(temp.data_), mutex_(temp.mutex_)
    {
      temp.data_ = NULL;
    }
    
    LockedRef& operator =(LockedRef&& temp)
    {
      this->data_ = temp.data_;
      this->mutex_ = temp.mutex_;
      temp.data_ = NULL;
    }
    
    // data access operators
    
    const Data* get() const { return data_; }
    Data* get() { return data_; }
    
    const Data* operator ->() const { return data_; }
    Data* operator ->() { return data_; }
    
  private:
    
    DataPtr data_;
    
    Mutex& mutex_;
    
    
    // copy and assignement disabled
    
    LockedRef(const LockedRef&);
    LockedRef& operator =(const LockedRef&);
  };

  
} // end of namespace

#endif
