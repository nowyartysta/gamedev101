module;

#include <format>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>

export module na_error:result;

import :error;

namespace na {

export template <typename T, typename E = SimpleError> class Result {
  std::variant<T, E> _value;

public:
  Result(T value) noexcept : _value(std::move(value)) {}
  Result(const Result &other) noexcept : _value(other._value) {}
  Result(E error) noexcept : _value(error) {}
  Result(Result &&other) noexcept : _value(std::move(other._value)) {}

  Result &operator=(const Result &other) noexcept {
    _value = other._value;
    return *this;
  }

  Result &operator=(Result &&other) noexcept {
    _value = std::move(other._value);
    return *this;
  }

  Result<void, E> dropValue() const noexcept {
    if (ok()) {
      return {};
    }
    return error();
  }

  bool ok() const noexcept { return _value.index() == 0; }

  bool failed() const noexcept { return _value.index() == 1; }

  T &value() noexcept { return std::get<T>(_value); }

  T &operator->() noexcept { return value(); }

  const T &operator->() const noexcept { return value(); }

  E &error() noexcept { return std::get<E>(_value); }

  const T &value() const noexcept { return std::get<T>(_value); }

  const E &error() const noexcept { return std::get<E>(_value); }

  template <class... Args>
  inline SimpleError errorWithPrefix(const char *fmt, Args &&...args) noexcept {
    std::string prefix = std::vformat(fmt, std::make_format_args(args...));
    return SimpleError("{}: {}", prefix, error().message());
  }

  operator bool() const noexcept { return ok(); }

  template <typename U>
  Result<U, E> andThen(std::function<Result<U, E>(T)> f) const noexcept {
    return ok() ? f(value()) : Result<U, E>(error());
  }

  template <typename U> Result<U, E> map(std::function<U(T)> f) const noexcept {
    return ok() ? Result<U, E>(f(value())) : Result<U, E>(error());
  }

  template <typename F>
  Result<T, F> mapError(std::function<F(E)> f) const noexcept {
    return ok() ? Result<T, F>(value()) : Result<T, F>(f(error()));
  }
};

export template <typename E> class Result<void, E> {
  std::optional<E> _error;

public:
  Result() noexcept : _error(std::nullopt) {}
  Result(E error) noexcept : _error(error) {}

  bool ok() const noexcept { return !failed(); }

  bool failed() const noexcept { return _error.has_value(); }

  E &error() noexcept { return _error.value(); }

  const E &error() const noexcept { return _error.value(); }

  template <class... Args>
  inline SimpleError errorWithPrefix(const char *fmt, Args &&...args) noexcept {
    std::string prefix = std::vformat(fmt, std::make_format_args(args...));
    return SimpleError("{}: {}", prefix, error().message());
  }

  operator bool() const noexcept { return ok(); }

  template <typename U>
  Result<U, E> andThen(std::function<Result<U, E>()> f) const noexcept {
    return ok() ? f() : Result<U, E>(error());
  }

  template <typename U> Result<U, E> map(std::function<U()> f) const noexcept {
    return ok() ? Result<U, E>(f()) : Result<U, E>(error());
  }

  template <typename F>
  Result<void, F> mapError(std::function<F(E)> f) const noexcept {
    return ok() ? Result<void, F>() : Result<void, F>(f(error()));
  }
};

export using VoidResult = Result<void, SimpleError>;

export template <typename T, typename E> class Result<std::unique_ptr<T>, E> {
  std::variant<std::unique_ptr<T>, E> _value;

public:
  Result() noexcept : _value(std::unique_ptr<T>(nullptr)) {}
  Result(std::unique_ptr<T> value) noexcept : _value(std::move(value)) {}
  Result(Result<std::unique_ptr<T>, E> &&other) noexcept
      : _value(std::move(other._value)) {}
  Result(E error) noexcept : _value(error) {}

  template <typename... Args>
  Result(Args &&...args) noexcept
      : _value(std::make_unique<T>(std::forward<Args>(args)...)) {}

  bool ok() const noexcept { return _value.index() == 0; }

  bool failed() const noexcept { return _value.index() == 1; }

  std::unique_ptr<T> &value() noexcept {
    return std::get<std::unique_ptr<T>>(_value);
  }

  T &operator->() noexcept { return value(); }

  const T &operator->() const noexcept { return value(); }

  E &error() noexcept { return std::get<E>(_value); }

  const T &value() const noexcept { return std::get<T>(_value); }

  const E &error() const noexcept { return std::get<E>(_value); }

  template <class... Args>
  inline SimpleError errorWithPrefix(const char *fmt, Args &&...args) noexcept {
    std::string prefix = std::vformat(fmt, std::make_format_args(args...));
    return SimpleError("{}: {}", prefix, error().message());
  }

  operator bool() const noexcept { return ok(); }

  template <typename U>
  Result<U, E>
  andThen(std::function<Result<U, E>(std::unique_ptr<T> &)> f) const noexcept {
    return ok() ? f(value()) : Result<U, E>(error());
  }

  template <typename U>
  Result<U, E> map(std::function<U(std::unique_ptr<T> &)> f) const noexcept {
    return ok() ? Result<U, E>(f(value())) : Result<U, E>(error());
  }

  Result<void, E>
  consume(std::function<void(std::unique_ptr<T> &)> f) noexcept {
    if (ok()) {
      f(value());
      return {};
    } else {
      return Result<void, E>(error());
    }
  }

  template <typename F>
  Result<T, F> mapError(std::function<F(E)> f) const noexcept {
    return ok() ? Result<T, F>(value()) : Result<T, F>(f(error()));
  }
};

export template <typename T>
using UniqueResult = Result<std::unique_ptr<T>, SimpleError>;

/* Maps a collection using provided mapFunc and respecting result behavior. */
export template <typename TInput, typename TInputIter, typename TOutput,
                 typename TOutputIter, typename E>
Result<void, E> map(TInputIter begin, TInputIter end, TOutputIter inserter,
                    std::function<Result<TOutput, E>(const TInput &)> mapFunc) {
  while (begin != end) {
    auto result = mapFunc(*begin++);
    if (result.failed()) {
      return result.errorWithPrefix("failed to map results");
    }
    *inserter++ = result.value();
  }
  return {};
}
} // namespace na