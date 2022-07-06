#include "rust/cxx.h"

namespace rust {
inline namespace cxxbridge1 {
namespace detail {
template <typename T, typename = void *>
struct operator_new {
  void *operator()(::std::size_t sz) { return ::operator new(sz); }
};

template <typename T>
struct operator_new<T, decltype(T::operator new(sizeof(T)))> {
  void *operator()(::std::size_t sz) { return T::operator new(sz); }
};
} // namespace detail

template <typename T>
union ManuallyDrop {
  T value;
  ManuallyDrop(T &&value) : value(::std::move(value)) {}
  ~ManuallyDrop() {}
};

template <typename T>
union MaybeUninit {
  T value;
  void *operator new(::std::size_t sz) { return detail::operator_new<T>{}(sz); }
  MaybeUninit() {}
  ~MaybeUninit() {}
};

namespace {
namespace repr {
struct PtrLen final {
  void *ptr;
  ::std::size_t len;
};
} // namespace repr

template <>
class impl<Error> final {
public:
  static Error error(repr::PtrLen repr) noexcept {
    Error error;
    error.msg = static_cast<const char *>(repr.ptr);
    error.len = repr.len;
    return error;
  }
};
} // namespace
} // namespace cxxbridge1
} // namespace rust

namespace GGRS {
  struct GGRSSessionInfo;
  struct GGRSPlayer;
  enum class GGRSPlayerType : ::std::uint8_t;
  enum class GGRSSessionType : ::std::uint8_t;
  enum class GGRSSessionState : ::std::uint8_t;
  struct GGRSSession;
}

namespace GGRS {
#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSSessionInfo
#define CXXBRIDGE1_STRUCT_GGRS$GGRSSessionInfo
struct GGRSSessionInfo final {
  bool session_started;
  ::GGRS::GGRSSessionType session_type;
  ::std::uint32_t num_players;
  ::std::uint32_t fps;
  ::std::uint32_t input_delay;
  ::std::uint32_t max_prediction_frames;
  ::std::uint32_t max_frames_behind;
  ::std::uint32_t catchup_speed;
  ::std::uint32_t check_distance;
  ::std::uint16_t local_port;
  ::rust::String host;
  bool sparse_saving;
  ::rust::Vec<::GGRS::GGRSPlayer> players;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSSessionInfo

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

#ifndef CXXBRIDGE1_ENUM_GGRS$GGRSSessionState
#define CXXBRIDGE1_ENUM_GGRS$GGRSSessionState
enum class GGRSSessionState : ::std::uint8_t {
  Okay = 0,
  Syncing = 1,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSSessionState

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSSession
#define CXXBRIDGE1_STRUCT_GGRS$GGRSSession
struct GGRSSession final : public ::rust::Opaque {
  ~GGRSSession() = delete;

private:
  friend ::rust::layout;
  struct layout {
    static ::std::size_t size() noexcept;
    static ::std::size_t align() noexcept;
  };
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSSession

extern "C" {
::std::size_t GGRS$cxxbridge1$GGRSSession$operator$sizeof() noexcept;
::std::size_t GGRS$cxxbridge1$GGRSSession$operator$alignof() noexcept;

bool GGRS$cxxbridge1$setup_ggrs_info(::GGRS::GGRSSessionInfo &info) noexcept;

bool GGRS$cxxbridge1$add_player(::GGRS::GGRSSessionInfo &info, ::GGRS::GGRSPlayer *player) noexcept;

bool GGRS$cxxbridge1$setup_p2p_session(::GGRS::GGRSSessionInfo &info, ::std::uint16_t local_port, ::std::uint32_t fps, ::std::uint32_t input_delay, ::std::uint32_t max_prediction_frames) noexcept;

bool GGRS$cxxbridge1$setup_spectator_session(::GGRS::GGRSSessionInfo &info, ::std::uint16_t local_port, ::rust::String *host, ::std::uint32_t max_frames_behind, ::std::uint32_t catchup_speed) noexcept;

bool GGRS$cxxbridge1$setup_synctest_session(::GGRS::GGRSSessionInfo &info, ::std::uint32_t check_distance, ::std::uint32_t input_delay) noexcept;

bool GGRS$cxxbridge1$set_num_players(::GGRS::GGRSSessionInfo &info, ::std::uint32_t num) noexcept;

bool GGRS$cxxbridge1$set_sparse_saving(::GGRS::GGRSSessionInfo &info, bool enable) noexcept;

::rust::repr::PtrLen GGRS$cxxbridge1$create_session(::GGRS::GGRSSessionInfo &info, ::GGRS::GGRSSession **return$) noexcept;

bool GGRS$cxxbridge1$poll_remote_clients(::GGRS::GGRSSession *session) noexcept;

bool GGRS$cxxbridge1$add_local_input(::GGRS::GGRSSession *session, ::std::uint32_t player_handle, ::std::uint32_t input) noexcept;

bool GGRS$cxxbridge1$get_current_state(::GGRS::GGRSSession *session, ::GGRS::GGRSSessionState &out_state) noexcept;

::std::int32_t GGRS$cxxbridge1$test_lib(::std::int32_t num) noexcept;
} // extern "C"

::std::size_t GGRSSession::layout::size() noexcept {
  return GGRS$cxxbridge1$GGRSSession$operator$sizeof();
}

::std::size_t GGRSSession::layout::align() noexcept {
  return GGRS$cxxbridge1$GGRSSession$operator$alignof();
}

bool setup_ggrs_info(::GGRS::GGRSSessionInfo &info) noexcept {
  return GGRS$cxxbridge1$setup_ggrs_info(info);
}

bool add_player(::GGRS::GGRSSessionInfo &info, ::GGRS::GGRSPlayer player) noexcept {
  ::rust::ManuallyDrop<::GGRS::GGRSPlayer> player$(::std::move(player));
  return GGRS$cxxbridge1$add_player(info, &player$.value);
}

bool setup_p2p_session(::GGRS::GGRSSessionInfo &info, ::std::uint16_t local_port, ::std::uint32_t fps, ::std::uint32_t input_delay, ::std::uint32_t max_prediction_frames) noexcept {
  return GGRS$cxxbridge1$setup_p2p_session(info, local_port, fps, input_delay, max_prediction_frames);
}

bool setup_spectator_session(::GGRS::GGRSSessionInfo &info, ::std::uint16_t local_port, ::rust::String host, ::std::uint32_t max_frames_behind, ::std::uint32_t catchup_speed) noexcept {
  return GGRS$cxxbridge1$setup_spectator_session(info, local_port, &host, max_frames_behind, catchup_speed);
}

bool setup_synctest_session(::GGRS::GGRSSessionInfo &info, ::std::uint32_t check_distance, ::std::uint32_t input_delay) noexcept {
  return GGRS$cxxbridge1$setup_synctest_session(info, check_distance, input_delay);
}

bool set_num_players(::GGRS::GGRSSessionInfo &info, ::std::uint32_t num) noexcept {
  return GGRS$cxxbridge1$set_num_players(info, num);
}

bool set_sparse_saving(::GGRS::GGRSSessionInfo &info, bool enable) noexcept {
  return GGRS$cxxbridge1$set_sparse_saving(info, enable);
}

::GGRS::GGRSSession *create_session(::GGRS::GGRSSessionInfo &info) {
  ::rust::MaybeUninit<::GGRS::GGRSSession *> return$;
  ::rust::repr::PtrLen error$ = GGRS$cxxbridge1$create_session(info, &return$.value);
  if (error$.ptr) {
    throw ::rust::impl<::rust::Error>::error(error$);
  }
  return ::std::move(return$.value);
}

bool poll_remote_clients(::GGRS::GGRSSession *session) noexcept {
  return GGRS$cxxbridge1$poll_remote_clients(session);
}

bool add_local_input(::GGRS::GGRSSession *session, ::std::uint32_t player_handle, ::std::uint32_t input) noexcept {
  return GGRS$cxxbridge1$add_local_input(session, player_handle, input);
}

bool get_current_state(::GGRS::GGRSSession *session, ::GGRS::GGRSSessionState &out_state) noexcept {
  return GGRS$cxxbridge1$get_current_state(session, out_state);
}

::std::int32_t test_lib(::std::int32_t num) noexcept {
  return GGRS$cxxbridge1$test_lib(num);
}
} // namespace GGRS

extern "C" {
void cxxbridge1$rust_vec$GGRS$GGRSPlayer$new(const ::rust::Vec<::GGRS::GGRSPlayer> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSPlayer$drop(::rust::Vec<::GGRS::GGRSPlayer> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSPlayer$len(const ::rust::Vec<::GGRS::GGRSPlayer> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSPlayer$capacity(const ::rust::Vec<::GGRS::GGRSPlayer> *ptr) noexcept;
const ::GGRS::GGRSPlayer *cxxbridge1$rust_vec$GGRS$GGRSPlayer$data(const ::rust::Vec<::GGRS::GGRSPlayer> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSPlayer$reserve_total(::rust::Vec<::GGRS::GGRSPlayer> *ptr, ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSPlayer$set_len(::rust::Vec<::GGRS::GGRSPlayer> *ptr, ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSPlayer$truncate(::rust::Vec<::GGRS::GGRSPlayer> *ptr, ::std::size_t len) noexcept;
} // extern "C"

namespace rust {
inline namespace cxxbridge1 {
template <>
Vec<::GGRS::GGRSPlayer>::Vec() noexcept {
  cxxbridge1$rust_vec$GGRS$GGRSPlayer$new(this);
}
template <>
void Vec<::GGRS::GGRSPlayer>::drop() noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSPlayer$drop(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSPlayer>::size() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSPlayer$len(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSPlayer>::capacity() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSPlayer$capacity(this);
}
template <>
const ::GGRS::GGRSPlayer *Vec<::GGRS::GGRSPlayer>::data() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSPlayer$data(this);
}
template <>
void Vec<::GGRS::GGRSPlayer>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSPlayer$reserve_total(this, new_cap);
}
template <>
void Vec<::GGRS::GGRSPlayer>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSPlayer$set_len(this, len);
}
template <>
void Vec<::GGRS::GGRSPlayer>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$GGRS$GGRSPlayer$truncate(this, len);
}
} // namespace cxxbridge1
} // namespace rust
