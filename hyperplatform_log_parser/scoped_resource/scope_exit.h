// This implementation is copied from below with small modification.
//
// P0052R1 - Generic Scope Guard and RAII Wrapper for the Standard Library
// by Peter Sommerlad and Andrew L. Sandoval

#ifndef SCOPE_EXIT_H_
#define SCOPE_EXIT_H_
#include <exception>
#include <utility>
// modeled slightly after Andrescu’s talk and article(s)
namespace std {
namespace experimental {
template <typename EF>
struct scope_exit {
  // construction
  explicit scope_exit(EF f) noexcept : exit_function{f} {
    static_assert(std::is_nothrow_copy_constructible<EF>{}, "must be copyable");
  }
  // move ctor for factory
  scope_exit(scope_exit &&rhs) noexcept
      : exit_function{rhs.exit_function},
        execute_on_destruction_flag{rhs.execute_on_destruction_flag} {
    rhs.release();
  }
  // release
  ~scope_exit() {
    if (execute_on_destruction_flag) exit_function();
  }
  void release() noexcept { execute_on_destruction_flag = false; }
  scope_exit(scope_exit const &) = delete;
  scope_exit &operator=(scope_exit const &) = delete;
  scope_exit &operator=(scope_exit &&) = delete;

 private:
  EF exit_function;                        // exposition only
  bool execute_on_destruction_flag{true};  // exposition only
};

template <typename EF>
auto make_scope_exit(EF exit_function) noexcept {
  return scope_exit<EF>(exit_function);
}
template <typename EF>
auto make_scope_fail(EF exit_function) noexcept {
  return make_scope_exit([ =, ec = std::uncaught_exceptions() ]() {
    if (ec < uncaught_exceptions()) exit_function();
  });
}
template <typename EF>
auto make_scope_success(EF exit_function) noexcept {
  return make_scope_exit([ =, ec = std::uncaught_exceptions() ]() {
    if (!(ec < uncaught_exceptions())) exit_function();
  });
}
}
}
#endif /∗ SCOPE EXIT H */
