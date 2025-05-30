module;

#include <format>
#include <sstream>
#include <string>
#include <variant>

export module na_error:error;

namespace na {
export class SimpleError {
  std::variant<const char *, std::string> _message;

public:
  inline SimpleError(const char *message) noexcept : _message(message) {}
  inline SimpleError(const std::string &message) noexcept : _message(message) {}
  template <class... Args>
  inline SimpleError(const char *fmt, Args &&...args) noexcept
      : _message(std::vformat(fmt, std::make_format_args(args...))) {}

  inline const char *message() const noexcept {
    if (_message.index() == 0) {
      return std::get<const char *>(_message);
    }
    return std::get<std::string>(_message).c_str();
  }

  inline bool operator==(const SimpleError &other) const noexcept {
    return _message == other._message;
  }
};

export template <typename T> class Error {
  std::variant<T, SimpleError> _value;

public:
  inline Error(T value) noexcept : _value(value) {}
  inline Error(SimpleError error) noexcept : _value(error) {}
  inline Error(const char *message) noexcept : _value(SimpleError(message)) {}
  inline Error(const std::string &message) noexcept
      : _value(SimpleError(message)) {}

  inline std::string message() const noexcept {
    if (_value.index() == 0) {
      std::stringstream ss;
      ss << std::get<T>(_value);
      return ss.str();
    }
    return std::get<SimpleError>(_value).message();
  }

  inline bool operator==(const Error &other) const noexcept {
    return _value == other._value;
  }

  inline bool operator==(const T &other) const noexcept {
    return _value.index() == 0 && std::get<T>(_value) == other;
  }
};

} // namespace na

export template <> struct std::formatter<na::SimpleError, char> {
  constexpr auto parse(format_parse_context &ctx) noexcept {
    return ctx.begin();
  }

  auto format(const na::SimpleError &error,
              format_context &ctx) const noexcept {
    return format_to(ctx.out(), "{}", error.message());
  }
};

export std::ostream &operator<<(std::ostream &os,
                                const na::SimpleError &error) noexcept {
  os << error.message();
  return os;
}

export template <typename T>
std::ostream &operator<<(std::ostream &os, const na::Error<T> &error) noexcept {
  os << error.message();
  return os;
}