#ifndef BUCKETEXCEPTION_H
#define BUCKETEXCEPTION_H

#include <list>
#include <stdexcept>
#include <sstream>


namespace crookie {


  /**
   * @brief Exception collection made by a list of exceptions.
   *
   * This class is used by EventBus to collect exceptions raised by event
   * handlers: before propagating them the EventBus complete to process 
   * handlers, thus more than one exceptions may be raised.
   *
   * You should catch this exception type on each EventBus::dispatch() call but
   * you can defer it to any external environment when applicable.
   *
   * @tparam Source   Base type of exception sources (i.e.: event handlers)
   */
  template <class Source>
  class BucketException : public std::exception
  {
  private:

    struct Bucket
    {
        typedef std::pair< const Source*, std::exception > Pair;
        typedef std::list< Pair > List;

        List list;
    };

  public:

    typedef typename Bucket::List::const_iterator const_iterator;
    typedef typename Bucket::List::iterator iterator;

    //! @brief Constructs an empty bucket (list is not initialized).
    //! @param [in] depth   current dispatching depth.
    BucketException(int depth)
        : bucket_(NULL), depth_(depth)
    { }

    //! @brief Destructor
    ~BucketException() throw() { delete bucket_; }

    //! @brief Return current dispatching depth (given at constructor)
    int depth() const { return depth_; }

    //! @brief Return true if Bucket is empty (no exceptions inside)
    bool empty() const { return (bucket_ == NULL); }

    //! @brief Add given exception to the bucket.
    //! @param [in] source  source of the exception (typically an handler)
    //! @param [in] ex      exception to add to the bucket.
    //! @note exception is copied inside the bucket using its copy constructor.
    void add(const Source* source, const std::exception& ex);

    // NOTE: you should never use them on an empty Bucket
    // but an empty Bucket should not be thrown.

    //! @brief iterator to beginning of the list.
    const_iterator begin() const { return bucket_->list.begin(); }

    //! @brief iterator to end of the list
    const_iterator end() const { return bucket_->list.end(); }

  private:

    Bucket* bucket_;
    int depth_;
  };



  template < class Source >
  void BucketException< Source >::add(
          const Source* source,
          const BucketException::exception& ex
  ) {
    if (bucket_ == NULL) bucket_ = new Bucket();

    typedef const BucketException* ConstBucket;

    ConstBucket container = dynamic_cast<ConstBucket>(&ex);
    if (container != NULL)
    {
        std::ostringstream errmsg;
        errmsg << "BucketException unwinding from depth" << container->depth();

        bucket_->list.push_back(
            std::make_pair(source, std::runtime_error(errmsg.str())));

        // auto-merge buckets
        iterator it = container->bucket_->list.begin();
        for (; it!=container->bucket_->list.end(); ++it)
            bucket_->list.push_back(*it);
    }
    else
    {
        //! @note we make a copy of the exception object
        bucket_->list.push_back(std::make_pair(source, ex));
    }
  }
  
  
}

#endif
