#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

namespace rust {
inline namespace cxxbridge1 {
// #include "rust/cxx.h"

struct unsafe_bitcopy_t;

#ifndef CXXBRIDGE1_RUST_STRING
#define CXXBRIDGE1_RUST_STRING
class String final {
public:
  String() noexcept;
  String(const String &) noexcept;
  String(String &&) noexcept;
  ~String() noexcept;

  String(const std::string &);
  String(const char *);
  String(const char *, std::size_t);
  String(const char16_t *);
  String(const char16_t *, std::size_t);

  static String lossy(const std::string &) noexcept;
  static String lossy(const char *) noexcept;
  static String lossy(const char *, std::size_t) noexcept;
  static String lossy(const char16_t *) noexcept;
  static String lossy(const char16_t *, std::size_t) noexcept;

  String &operator=(const String &) &noexcept;
  String &operator=(String &&) &noexcept;

  explicit operator std::string() const;

  const char *data() const noexcept;
  std::size_t size() const noexcept;
  std::size_t length() const noexcept;
  bool empty() const noexcept;

  const char *c_str() noexcept;

  std::size_t capacity() const noexcept;
  void reserve(size_t new_cap) noexcept;

  using iterator = char *;
  iterator begin() noexcept;
  iterator end() noexcept;

  using const_iterator = const char *;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  bool operator==(const String &) const noexcept;
  bool operator!=(const String &) const noexcept;
  bool operator<(const String &) const noexcept;
  bool operator<=(const String &) const noexcept;
  bool operator>(const String &) const noexcept;
  bool operator>=(const String &) const noexcept;

  void swap(String &) noexcept;

  String(unsafe_bitcopy_t, const String &) noexcept;

private:
  struct lossy_t;
  String(lossy_t, const char *, std::size_t) noexcept;
  String(lossy_t, const char16_t *, std::size_t) noexcept;
  friend void swap(String &lhs, String &rhs) noexcept { lhs.swap(rhs); }

  std::array<std::uintptr_t, 3> repr;
};
#endif // CXXBRIDGE1_RUST_STRING

#ifndef CXXBRIDGE1_RUST_OPAQUE
#define CXXBRIDGE1_RUST_OPAQUE
class Opaque {
public:
  Opaque() = delete;
  Opaque(const Opaque &) = delete;
  ~Opaque() = delete;
};
#endif // CXXBRIDGE1_RUST_OPAQUE

#ifndef CXXBRIDGE1_IS_COMPLETE
#define CXXBRIDGE1_IS_COMPLETE
namespace detail {
namespace {
template <typename T, typename = std::size_t>
struct is_complete : std::false_type {};
template <typename T>
struct is_complete<T, decltype(sizeof(T))> : std::true_type {};
} // namespace
} // namespace detail
#endif // CXXBRIDGE1_IS_COMPLETE

#ifndef CXXBRIDGE1_LAYOUT
#define CXXBRIDGE1_LAYOUT
class layout {
  template <typename T>
  friend std::size_t size_of();
  template <typename T>
  friend std::size_t align_of();
  template <typename T>
  static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_size_of() {
    return T::layout::size();
  }
  template <typename T>
  static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_size_of() {
    return sizeof(T);
  }
  template <typename T>
  static
      typename std::enable_if<detail::is_complete<T>::value, std::size_t>::type
      size_of() {
    return do_size_of<T>();
  }
  template <typename T>
  static typename std::enable_if<std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_align_of() {
    return T::layout::align();
  }
  template <typename T>
  static typename std::enable_if<!std::is_base_of<Opaque, T>::value,
                                 std::size_t>::type
  do_align_of() {
    return alignof(T);
  }
  template <typename T>
  static
      typename std::enable_if<detail::is_complete<T>::value, std::size_t>::type
      align_of() {
    return do_align_of<T>();
  }
};

template <typename T>
std::size_t size_of() {
  return layout::size_of<T>();
}

template <typename T>
std::size_t align_of() {
  return layout::align_of<T>();
}
#endif // CXXBRIDGE1_LAYOUT
} // namespace cxxbridge1
} // namespace rust

namespace GGRS {
  struct GGRSPlayer;
  enum class GGRSPlayerType : ::std::uint8_t;
  enum class GGRSSessionType : ::std::uint8_t;
  struct GGRSSessionInfo;
}

namespace GGRS {
#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSPlayer
#define CXXBRIDGE1_STRUCT_GGRS$GGRSPlayer
struct GGRSPlayer final {
  ::std::uint32_t player_handle;
  ::GGRS::GGRSPlayerType player_type;
  ::rust::String socket_addr;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSPlayer

#ifndef CXXBRIDGE1_ENUM_GGRS$GGRSPlayerType
#define CXXBRIDGE1_ENUM_GGRS$GGRSPlayerType
enum class GGRSPlayerType : ::std::uint8_t {
  Local = 0,
  Remote = 1,
  Spectator = 2,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSPlayerType

#ifndef CXXBRIDGE1_ENUM_GGRS$GGRSSessionType
#define CXXBRIDGE1_ENUM_GGRS$GGRSSessionType
enum class GGRSSessionType : ::std::uint8_t {
  NotSet = 0,
  Peer2Peer = 1,
  Spectator = 2,
  Synctest = 3,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSSessionType

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSSessionInfo
#define CXXBRIDGE1_STRUCT_GGRS$GGRSSessionInfo
struct GGRSSessionInfo final : public ::rust::Opaque {
  ~GGRSSessionInfo() = delete;

private:
  friend ::rust::layout;
  struct layout {
    static ::std::size_t size() noexcept;
    static ::std::size_t align() noexcept;
  };
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSSessionInfo

bool setup_ggrs_info(::GGRS::GGRSSessionInfo &info) noexcept;

bool add_player(::GGRS::GGRSSessionInfo &info, ::GGRS::GGRSPlayer player) noexcept;

bool set_session_type(::GGRS::GGRSSessionInfo &info, ::GGRS::GGRSSessionType session_type) noexcept;

bool set_num_players(::GGRS::GGRSSessionInfo &info, ::std::uint32_t players) noexcept;

bool set_fps(::GGRS::GGRSSessionInfo &info, ::std::uint32_t fps) noexcept;

bool set_input_delay(::GGRS::GGRSSessionInfo &info, ::std::uint32_t delay) noexcept;

bool set_max_frames_behind(::GGRS::GGRSSessionInfo &info, ::std::uint32_t max_frames_behind) noexcept;

bool set_catchup_speed(::GGRS::GGRSSessionInfo &info, ::std::uint32_t catchup_speed) noexcept;

bool set_check_distance(::GGRS::GGRSSessionInfo &info, ::std::uint32_t check_distance) noexcept;

bool set_local_port(::GGRS::GGRSSessionInfo &info, ::std::uint16_t local_port) noexcept;
} // namespace GGRS
