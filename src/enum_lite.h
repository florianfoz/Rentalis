#ifndef ENUM_LITE_H
#define ENUM_LITE_H

#include <QString>
#include <QStringLiteral>
#include <array>


// ============================================================================
// enum_lite - C++20, header-only, zero dependency
// ============================================================================

namespace enum_lite
{

// ============================================================================
// Static string list
// ============================================================================

template <qsizetype N>
struct static_string_list {
  std::array<const QString&, N> storage{};
  qsizetype                     size = 0;

  constexpr qsizetype count() const noexcept
  {
    return size;
  }

  constexpr bool empty() const noexcept
  {
    return size == 0;
  }

  constexpr const QString& operator[](qsizetype index) const noexcept
  {
    return storage[index];
  }

  constexpr auto begin() const noexcept
  {
    return storage.begin();
  }

  constexpr auto end() const noexcept
  {
    return storage.begin() + size;
  }
};


// ============================================================================
// String normalization
// ============================================================================


inline QString strip_enum_suffix(const QString& name) noexcept
{
  // A trailing '_' is commonly used to avoid C++ keyword collisions:
  //
  //   enum class Foo { class_ };
  //
  // The exposed name is simply "class".
  auto n = name;
  if (!name.isEmpty() && name.back() == '_') n.chop(1);

  return n;
}

inline QString trim(const QString& str) noexcept
{
  auto n = str;
  while (!str.isEmpty() && (str.front() == ' ' || str.front() == '\t')) {
    n.slice(1);
  }

  while (!str.isEmpty() && (str.back() == ' ' || str.back() == '\t')) {
    n.chop(1);
  }

  return n;
}


inline bool normalized_equal(const QString& input, const QString& name) noexcept
{
  auto in = input;
  auto n  = name;
  // "_foo" matches "foo".
  if (!input.isEmpty() && input.front() == '_') in.slice(1);

  // "foo_" is exposed as "foo".
  n = strip_enum_suffix(name);

  if (in.size() != n.size()) return false;

  for (qsizetype i = 0; i < in.size(); ++i) {
    QChar lhs = in[i];
    QChar rhs = n[i];

    // '-' and '_' are equivalent.
    if (lhs == '-') lhs = '_';

    if (rhs == '-') rhs = '_';

    if (lhs.toLower() != rhs.toLower()) return false;
  }

  return true;
}


// ============================================================================
// Preprocessor expansion
// ============================================================================
//
// The recursion itself is deliberately kept simple.  PP_EXPAND is only here
// because MSVC/GCC/Clang otherwise differ in how aggressively variadic macro
// arguments are expanded.
//

#define ENUM_LITE_PP_PARENS ()

#define ENUM_LITE_PP_EXPAND(...) ENUM_LITE_PP_EXPAND_1(__VA_ARGS__)

#define ENUM_LITE_PP_EXPAND_1(...)                                                                                     \
  ENUM_LITE_PP_EXPAND_2(ENUM_LITE_PP_EXPAND_2(ENUM_LITE_PP_EXPAND_2(ENUM_LITE_PP_EXPAND_2(__VA_ARGS__))))

#define ENUM_LITE_PP_EXPAND_2(...)                                                                                     \
  ENUM_LITE_PP_EXPAND_3(ENUM_LITE_PP_EXPAND_3(ENUM_LITE_PP_EXPAND_3(ENUM_LITE_PP_EXPAND_3(__VA_ARGS__))))

#define ENUM_LITE_PP_EXPAND_3(...)                                                                                     \
  ENUM_LITE_PP_EXPAND_4(ENUM_LITE_PP_EXPAND_4(ENUM_LITE_PP_EXPAND_4(ENUM_LITE_PP_EXPAND_4(__VA_ARGS__))))

#define ENUM_LITE_PP_EXPAND_4(...)                                                                                     \
  ENUM_LITE_PP_EXPAND_5(ENUM_LITE_PP_EXPAND_5(ENUM_LITE_PP_EXPAND_5(ENUM_LITE_PP_EXPAND_5(__VA_ARGS__))))

#define ENUM_LITE_PP_EXPAND_5(...) __VA_ARGS__


// ============================================================================
// Pair iteration
//
// Input:
//
//   NAME, VALUE, NAME, VALUE, NAME, VALUE
//
// Example:
//
//   RED, 1, GREEN, 2, BLUE, 4
// ============================================================================

#define ENUM_LITE_PP_FOR_EACH_PAIR(macro, ...)                                                                         \
  __VA_OPT__(ENUM_LITE_PP_EXPAND(ENUM_LITE_PP_FOR_EACH_PAIR_I(macro, __VA_ARGS__)))

#define ENUM_LITE_PP_FOR_EACH_PAIR_I(macro, name, value, ...)                                                          \
  macro(name, value) __VA_OPT__(ENUM_LITE_PP_FOR_EACH_PAIR_AGAIN ENUM_LITE_PP_PARENS(macro, __VA_ARGS__))

#define ENUM_LITE_PP_FOR_EACH_PAIR_AGAIN() ENUM_LITE_PP_FOR_EACH_PAIR_I


// ============================================================================
// Pair iteration with context
//
//   macro(Type, name, value)
// ============================================================================

#define ENUM_LITE_PP_FOR_EACH_PAIR_CTX(macro, context, ...)                                                            \
  __VA_OPT__(ENUM_LITE_PP_EXPAND(ENUM_LITE_PP_FOR_EACH_PAIR_CTX_I(macro, context, __VA_ARGS__)))

#define ENUM_LITE_PP_FOR_EACH_PAIR_CTX_I(macro, context, name, value, ...)                                             \
  macro(context, name, value)                                                                                          \
      __VA_OPT__(ENUM_LITE_PP_FOR_EACH_PAIR_CTX_AGAIN ENUM_LITE_PP_PARENS(macro, context, __VA_ARGS__))

#define ENUM_LITE_PP_FOR_EACH_PAIR_CTX_AGAIN() ENUM_LITE_PP_FOR_EACH_PAIR_CTX_I

// ============================================================================
// Regular enum generation
// ============================================================================

#define ENUM_LITE_DETAIL_DECLARE(name, value) name = value,

#define ENUM_LITE_DETAIL_NAME(name, value) #name,

#define ENUM_LITE_DETAIL_VALUES(name, value) ", " #name

#define ENUM_LITE_DETAIL_ELEM(Type, name, value) Type::name,

#define ENUM_LITE_DETAIL_TO_STRING(Type, name, value)                                                                  \
  case Type::name: return enum_lite::strip_enum_suffix(#name);

#define ENUM_LITE_DETAIL_FROM_STRING(Type, name, value)                                                                \
  if (enum_lite::normalized_equal(str, #name)) return Type::name;

#define ENUM_LITE_DETAIL_FLAG_IS_VALID_STRING(Type, name, value)                                                       \
  if (enum_lite::normalized_equal(token, #name)) known = true;

// ============================================================================
// Regular enum
// ============================================================================

#define ENUM_LITE_DEFINE_ENUM(Type, Underlying, ...)                                                                   \
  enum class Type : Underlying { NONE = 0, ENUM_LITE_PP_FOR_EACH_PAIR(ENUM_LITE_DETAIL_DECLARE, __VA_ARGS__) };        \
                                                                                                                       \
  inline const QStringList Type##_names = {"NONE", ENUM_LITE_PP_FOR_EACH_PAIR(ENUM_LITE_DETAIL_NAME, __VA_ARGS__)};    \
                                                                                                                       \
  inline const QString Type##_values = "NONE" ENUM_LITE_PP_FOR_EACH_PAIR(ENUM_LITE_DETAIL_VALUES, __VA_ARGS__);        \
                                                                                                                       \
  inline const auto Type##_all = {Type::NONE,                                                                          \
                                  ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_ELEM, Type, __VA_ARGS__)};           \
                                                                                                                       \
  inline QString Type##_to_str(Type value) noexcept                                                                    \
  {                                                                                                                    \
    switch (value) {                                                                                                   \
    case Type::NONE: return {}; ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_TO_STRING, Type, __VA_ARGS__)          \
    }                                                                                                                  \
                                                                                                                       \
    return {};                                                                                                         \
  }                                                                                                                    \
                                                                                                                       \
  inline Type Type##_from_str(const QString& str) noexcept                                                             \
  {                                                                                                                    \
    ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_FROM_STRING, Type, __VA_ARGS__)                                    \
                                                                                                                       \
    return Type::NONE;                                                                                                 \
  }                                                                                                                    \
                                                                                                                       \
  inline bool Type##_is_valid(const QString& str) noexcept                                                             \
  {                                                                                                                    \
    if (enum_lite::normalized_equal(str, "NONE")) return true;                                                         \
                                                                                                                       \
    ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_IS_VALID_STRING, Type, __VA_ARGS__)                                \
                                                                                                                       \
    return false;                                                                                                      \
  }


// ============================================================================
// Flags generation
// ============================================================================

#define ENUM_LITE_DETAIL_FLAG_OR_VALUE(name, value) | value

#define ENUM_LITE_DETAIL_FLAG_VALUE_STRING(name, value) "|" #name

#define ENUM_LITE_DETAIL_FLAG_TO_STRING(Type, name, val)                                                               \
  if (Type##_has_flag(value, Type::name)) {                                                                            \
    if (!result.empty()) result += separator;                                                                          \
                                                                                                                       \
    result += enum_lite::strip_enum_suffix(#name);                                                                     \
  }

#define ENUM_LITE_DETAIL_FLAG_TO_VECTOR(Type, name, val)                                                               \
  if (Type##_has_flag(value, Type::name)) result.storage[result.size++] = enum_lite::strip_enum_suffix(#name);

#define ENUM_LITE_DETAIL_FLAG_FROM_STRING(Type, name, value)                                                           \
  if (enum_lite::normalized_equal(token, #name)) result |= Type::name;

#define ENUM_LITE_DETAIL_IS_VALID_STRING(Type, name, value)                                                            \
  if (enum_lite::normalized_equal(str, #name)) return true;


// ============================================================================
// Flags
// ============================================================================

#define ENUM_LITE_DEFINE_FLAGS(Type, Underlying, ...)                                                                  \
  enum class Type : Underlying {                                                                                       \
    NONE = 0,                                                                                                          \
    ENUM_LITE_PP_FOR_EACH_PAIR(ENUM_LITE_DETAIL_DECLARE, __VA_ARGS__) ALL =                                            \
        0 ENUM_LITE_PP_FOR_EACH_PAIR(ENUM_LITE_DETAIL_FLAG_OR_VALUE, __VA_ARGS__)                                      \
  };                                                                                                                   \
                                                                                                                       \
  inline const auto Type##_names =                                                                                     \
      std::array{"NONE", ENUM_LITE_PP_FOR_EACH_PAIR(ENUM_LITE_DETAIL_NAME, __VA_ARGS__) const QString & {"ALL"}};      \
                                                                                                                       \
  inline const QString Type##_values =                                                                                 \
      "NONE" ENUM_LITE_PP_FOR_EACH_PAIR(ENUM_LITE_DETAIL_VALUES, __VA_ARGS__) ", ALL";                                 \
                                                                                                                       \
  constexpr Type operator|(Type lhs, Type rhs) noexcept                                                                \
  {                                                                                                                    \
    using U = std::underlying_type_t<Type>;                                                                            \
    return static_cast<Type>(static_cast<U>(lhs) | static_cast<U>(rhs));                                               \
  }                                                                                                                    \
                                                                                                                       \
  constexpr Type operator&(Type lhs, Type rhs) noexcept                                                                \
  {                                                                                                                    \
    using U = std::underlying_type_t<Type>;                                                                            \
    return static_cast<Type>(static_cast<U>(lhs) & static_cast<U>(rhs));                                               \
  }                                                                                                                    \
                                                                                                                       \
  constexpr Type operator^(Type lhs, Type rhs) noexcept                                                                \
  {                                                                                                                    \
    using U = std::underlying_type_t<Type>;                                                                            \
    return static_cast<Type>(static_cast<U>(lhs) ^ static_cast<U>(rhs));                                               \
  }                                                                                                                    \
                                                                                                                       \
  constexpr Type operator~(Type value) noexcept                                                                        \
  {                                                                                                                    \
    using U = std::underlying_type_t<Type>;                                                                            \
    return static_cast<Type>(~static_cast<U>(value));                                                                  \
  }                                                                                                                    \
                                                                                                                       \
  constexpr Type& operator|=(Type& lhs, Type rhs) noexcept                                                             \
  {                                                                                                                    \
    lhs = lhs | rhs;                                                                                                   \
    return lhs;                                                                                                        \
  }                                                                                                                    \
                                                                                                                       \
  constexpr Type& operator&=(Type& lhs, Type rhs) noexcept                                                             \
  {                                                                                                                    \
    lhs = lhs & rhs;                                                                                                   \
    return lhs;                                                                                                        \
  }                                                                                                                    \
                                                                                                                       \
  constexpr Type& operator^=(Type& lhs, Type rhs) noexcept                                                             \
  {                                                                                                                    \
    lhs = lhs ^ rhs;                                                                                                   \
    return lhs;                                                                                                        \
  }                                                                                                                    \
                                                                                                                       \
  constexpr bool Type##_has_flag(Type value, Type flag) noexcept                                                       \
  {                                                                                                                    \
    using U = std::underlying_type_t<Type>;                                                                            \
                                                                                                                       \
    const U f = static_cast<U>(flag);                                                                                  \
                                                                                                                       \
    if (f == 0) return static_cast<U>(value) == 0;                                                                     \
                                                                                                                       \
    return (static_cast<U>(value) & f) == f;                                                                           \
  }                                                                                                                    \
                                                                                                                       \
  inline QString Type##_to_str(Type value, char separator = '|')                                                       \
  {                                                                                                                    \
    if (value == Type::NONE) return "NONE";                                                                            \
                                                                                                                       \
    QString result;                                                                                                    \
                                                                                                                       \
    ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_FLAG_TO_STRING, Type, __VA_ARGS__)                                 \
                                                                                                                       \
    return result;                                                                                                     \
  }                                                                                                                    \
                                                                                                                       \
  inline auto Type##_to_vec_str(Type value) noexcept                                                                   \
  {                                                                                                                    \
    enum_lite::static_string_list<Type##_names.size()> result;                                                         \
                                                                                                                       \
    if (value == Type::NONE) {                                                                                         \
      result.storage[result.size++] = "NONE";                                                                          \
      return result;                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_FLAG_TO_VECTOR, Type, __VA_ARGS__)                                 \
                                                                                                                       \
    return result;                                                                                                     \
  }                                                                                                                    \
                                                                                                                       \
  inline Type Type##_from_str(const QString& str, char separator = '|') noexcept                                       \
  {                                                                                                                    \
    Type result = Type::NONE;                                                                                          \
                                                                                                                       \
    while (!str.empty()) {                                                                                             \
      const qsizetype pos   = str.find(separator);                                                                     \
      const QString   token = enum_lite::trim(str.substr(0, pos));                                                     \
                                                                                                                       \
      if (token == "NONE") {                                                                                           \
        /* NONE contributes no bits. */                                                                                \
      } else {                                                                                                         \
        ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_FLAG_FROM_STRING, Type, __VA_ARGS__)                           \
      }                                                                                                                \
                                                                                                                       \
      if (pos == QString::npos) break;                                                                                 \
                                                                                                                       \
      str.remove_prefix(pos + 1);                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    return result;                                                                                                     \
  }                                                                                                                    \
                                                                                                                       \
  inline bool Type##_is_valid(const QString& str, const QString& separator = "|") noexcept                             \
  {                                                                                                                    \
    if (str.empty()) return false;                                                                                     \
                                                                                                                       \
    while (!str.empty()) {                                                                                             \
      const qsizetype pos   = str.find(separator);                                                                     \
      const QString   token = enum_lite::trim(str.substr(0, pos));                                                     \
                                                                                                                       \
      if (token.empty()) return false;                                                                                 \
                                                                                                                       \
      bool known = false;                                                                                              \
                                                                                                                       \
      if (enum_lite::normalized_equal(token, "NONE")) {                                                                \
        known = true;                                                                                                  \
      } else {                                                                                                         \
        ENUM_LITE_PP_FOR_EACH_PAIR_CTX(ENUM_LITE_DETAIL_FLAG_IS_VALID_STRING, Type, __VA_ARGS__)                       \
      }                                                                                                                \
                                                                                                                       \
      if (!known) return false;                                                                                        \
                                                                                                                       \
      if (pos == std::string_view::npos) break;                                                                        \
                                                                                                                       \
      str.remove_prefix(pos + 1);                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    return true;                                                                                                       \
  }


#define DEFINE_ENUM(Type, Underlying, ...) ENUM_LITE_DEFINE_ENUM(Type, Underlying, __VA_ARGS__)

#define DEFINE_FLAGS(Type, Underlying, ...) ENUM_LITE_DEFINE_FLAGS(Type, Underlying, __VA_ARGS__)


} // namespace enum_lite

#endif // ENUM_LITE_H