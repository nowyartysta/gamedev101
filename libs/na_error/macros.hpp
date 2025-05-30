#pragma once

#define ASSIGN_RESULT(var, expr)                                               \
  if (auto result = (expr); result.failed()) {                                 \
    result.errorWithPrefix("({}:{})", __FUNCTION__, __LINE__);                 \
  } else {                                                                     \
    var = result.value();                                                      \
  }

#define UNIQUE_RESULT(var, expr)                                               \
  auto __##var = (expr);                                                       \
  if (__##var.failed()) {                                                      \
    return __##var.errorWithPrefix("({}:{})", __FUNCTION__, __LINE__);         \
  }                                                                            \
  auto var = std::move(__##var.value());

#define AUTO_RESULT(var, expr)                                                 \
  auto __##var = (expr);                                                       \
  if (__##var.failed()) {                                                      \
    return __##var.errorWithPrefix("({}:{})", __FUNCTION__, __LINE__);         \
  }                                                                            \
  auto var = __##var.value();

#define CHECK_RESULT(expr)                                                     \
  if (auto result = (expr); result.failed()) {                                 \
    return result.errorWithPrefix("({}:{})", __FUNCTION__, __LINE__);          \
  }

#define ASSERT_NOT_NULL(var)                                                   \
  if (var == nullptr) {                                                        \
    return na::SimpleError("({}:{}): Failed assertion {} != null",             \
                           __FUNCTION__, __LINE__, #var);                      \
  }