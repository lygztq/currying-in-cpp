#include <type_traits>
#include <typeinfo>
#include <tuple>
#include <cstdint>
#include <functional>

template <typename Fn>
struct func_traits {};

template <typename R, typename... Args>
struct func_traits<R(Args...)> {
  using self_ptr_t = R(*)(Args...);
  using self_t = R(Args...);
  using return_t = R;
  using arg_ts = std::tuple<Args...>;

  template <size_t i>
  struct arg_ {
    using type = std::tuple_element_t<i, arg_ts>;
  };

  template <size_t i>
  using arg_t = typename arg_<i>::type;

  constexpr static size_t arity = sizeof...(Args);
};

template <typename R, typename... Args>
struct func_traits<R(*)(Args...)> : public func_traits<R(Args...)> {};

template <typename R, typename... Args>
struct func_traits<R(&)(Args...)> : public func_traits<R(Args...)> {};

template <typename Fn>
struct currying : public currying<typename func_traits<Fn>::self_t> {
  template <typename Callable>
  currying(Callable&& fn) : currying<typename func_traits<Fn>::self_t>(fn) {}
};

template <typename R, typename Arg0, typename... RemainArgs>
struct currying<R(Arg0, RemainArgs...)> {
  std::function<R(Arg0, RemainArgs...)> fn;

  template <typename Callable,
            typename std::enable_if_t<std::is_convertible_v<std::invoke_result_t<std::remove_reference_t<Callable>, Arg0, RemainArgs...>, R>>* = nullptr,
            typename std::enable_if_t<!std::is_same_v<std::remove_reference_t<Callable>, currying>>* = nullptr>
  currying(Callable&& fn)
  : fn(fn) {}

  auto operator()(Arg0 arg0) -> currying<R(RemainArgs...)> {
    return currying<R(RemainArgs...)>(
      [this, &arg0](RemainArgs... remain_args) -> R {
        return fn(arg0, remain_args...);
      }
    );
  }
};

template <typename R, typename Arg0>
struct currying<R(Arg0)> {
  std::function<R(Arg0)> fn;

  template <typename Callable,
            typename std::enable_if_t<std::is_convertible_v<std::invoke_result_t<std::remove_reference_t<Callable>, Arg0>, R>>* = nullptr,
            typename std::enable_if_t<!std::is_same_v<std::remove_reference_t<Callable>, currying>>* = nullptr>
  currying(Callable&& fn)
  : fn(fn) {}

  auto operator()(Arg0 arg0) -> R {
    return this->fn(arg0);
  }
};

template <typename Callable>
auto make_currying(Callable&& fn) -> currying<decltype(fn)> {
  return currying<decltype(fn)>(fn);
}
