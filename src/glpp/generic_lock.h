#ifndef GLPP_GENERIC_LOCK
#define GLPP_GENERIC_LOCK

#include <functional>

namespace glp {

class generic_lock {
public:
  generic_lock(std::function<void()> unlock_fn) noexcept;
  generic_lock(std::function<void()> lock_fn, std::function<void()> unlock_fn);
  ~generic_lock();

private:
  const std::function<void()> lock_fn;
  const std::function<void()> unlock_fn;
};

} // namespace glp

#endif
