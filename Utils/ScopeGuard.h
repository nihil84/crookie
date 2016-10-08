//
//  ScopeGuard.h
//  crookie
//
//  Created by Paolo Bernini on 03/09/16.
//
//

#ifndef SCOPEGUARD_H
#define SCOPEGUARD_H

#include <functional>

namespace crookie {
  

class ScopeGuard
{
public:
  
  template<class Callable>
  ScopeGuard(Callable && undo_func)
    : m_f(std::forward<Callable>(undo_func))
  { }
  
  ScopeGuard(ScopeGuard && other)
    : m_f(std::move(other.m_f))
  { other.m_f = nullptr; }
  
  ~ScopeGuard() noexcept
  {
    if (m_f) m_f(); // must not throw
  }
  
  void dismiss() noexcept
  { m_f = nullptr; }
  
  ScopeGuard(const ScopeGuard&) = delete;
  void operator = (const ScopeGuard&) = delete;
  
private:
  std::function<void()> m_f;
};
  

} // end of namespace

#endif /* SCOPEGUARD_H */
