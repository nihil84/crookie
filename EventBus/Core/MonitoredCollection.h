//  MonitoredCollection.h
//
//  Created by Paolo Bernini on 20/01/13.
//  Copyright (c) 2013 IDS. All rights reserved.
//

#ifndef MONITOREDCOLLECTION_H
#define MONITOREDCOLLECTION_H

#include <list>
#include <mutex>

namespace crookie {

template < class C, class Lockable = std::mutex >
class LockedIterable
{
public:

  typedef typename C::const_iterator const_iterator;
  typedef const_iterator ConstIterator;

  LockedIterable(const C& collection, Lockable& mutex)
      : m_mutex(mutex), collection_(collection)
  { m_mutex.lock(); }

  ~LockedIterable()
  { m_mutex.unlock(); }

  const_iterator begin() const { return collection_.begin(); }

  const_iterator end() const { return collection_.end(); }

private:

  Lockable& m_mutex;
  const C& collection_;
};

/**
 * @brief Generic collection of elements with mutual exclusion access.
 * @tparam [in] T           type of collection elements.
 * @tparam [in] Collection  type of collection.
 */
template < typename T, class Collection = std::list<T> >
class MonitoredCollection
{
public:

  typedef T ElementType;
  typedef Collection CollectionType;
  typedef LockedIterable< CollectionType, std::mutex > Iterable;

  //! @brief Add an element to the collection.
  void add(const ElementType& element)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    collection_.push_back(element);
  }

  //! @brief Remove given element from the collection.
  void remove(const ElementType& element)
  {
    std::unique_lock<std::mutex> lock(mutex_);
    collection_.remove(element);
  }

  //! @brief Return a special object that allows to iterate over the
  //! collection in mutual exclusion.
  //! @note Collection remains locked until object destruction.
  Iterable getIterable()
  { return Iterable(collection_, mutex_); }

private:

  std::mutex mutex_;
  CollectionType collection_;
};


}

#endif
