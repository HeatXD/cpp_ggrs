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
  enum class GGRSEventType : ::std::uint8_t;
  struct GGRSEventInfo;
  struct GGRSEvent;
  struct GGRSFrameResult;
  struct GGRSFrameAction;
  enum class GGRSFrameActionType : ::std::uint8_t;
  struct GGRSFrameActionInfo;
  struct GGRSInput;
  enum class GGRSInputStatus : ::std::uint8_t;
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
  Running = 0,
  Synchronizing = 1,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSSessionState

#ifndef CXXBRIDGE1_ENUM_GGRS$GGRSEventType
#define CXXBRIDGE1_ENUM_GGRS$GGRSEventType
enum class GGRSEventType : ::std::uint8_t {
  Empty = 0,
  Synchronizing = 1,
  Synchronized = 2,
  Disconnected = 3,
  NetworkInterrupted = 4,
  NetworkResumed = 5,
  WaitRecommendation = 6,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSEventType

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSEventInfo
#define CXXBRIDGE1_STRUCT_GGRS$GGRSEventInfo
struct GGRSEventInfo final {
  ::rust::String addr;
  ::std::uint32_t total;
  ::std::uint32_t count;
  ::std::uint64_t disconnect_timeout;
  ::std::uint32_t skip_frames;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSEventInfo

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSEvent
#define CXXBRIDGE1_STRUCT_GGRS$GGRSEvent
struct GGRSEvent final {
  ::GGRS::GGRSEventType event_type;
  ::GGRS::GGRSEventInfo event_info;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSEvent

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSFrameResult
#define CXXBRIDGE1_STRUCT_GGRS$GGRSFrameResult
struct GGRSFrameResult final {
  bool skip_frame;
  ::rust::Vec<::GGRS::GGRSFrameAction> actions;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSFrameResult

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSFrameActionInfo
#define CXXBRIDGE1_STRUCT_GGRS$GGRSFrameActionInfo
struct GGRSFrameActionInfo final {
  ::std::int32_t frame;
  ::rust::Vec<::GGRS::GGRSInput> inputs;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSFrameActionInfo

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSFrameAction
#define CXXBRIDGE1_STRUCT_GGRS$GGRSFrameAction
struct GGRSFrameAction final {
  ::GGRS::GGRSFrameActionType action_type;
  ::GGRS::GGRSFrameActionInfo action_info;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSFrameAction

#ifndef CXXBRIDGE1_ENUM_GGRS$GGRSFrameActionType
#define CXXBRIDGE1_ENUM_GGRS$GGRSFrameActionType
enum class GGRSFrameActionType : ::std::uint8_t {
  SaveGameState = 0,
  LoadGameState = 1,
  AdvanceFrame = 2,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSFrameActionType

#ifndef CXXBRIDGE1_STRUCT_GGRS$GGRSInput
#define CXXBRIDGE1_STRUCT_GGRS$GGRSInput
struct GGRSInput final {
  ::std::uint32_t input;
  ::GGRS::GGRSInputStatus status;

  using IsRelocatable = ::std::true_type;
};
#endif // CXXBRIDGE1_STRUCT_GGRS$GGRSInput

#ifndef CXXBRIDGE1_ENUM_GGRS$GGRSInputStatus
#define CXXBRIDGE1_ENUM_GGRS$GGRSInputStatus
enum class GGRSInputStatus : ::std::uint8_t {
  Confirmed = 0,
  Predicted = 1,
  Disconnected = 2,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSInputStatus

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

::rust::repr::PtrLen GGRS$cxxbridge1$add_local_input(::GGRS::GGRSSession *session, ::std::uint32_t player_handle, ::std::uint32_t input, bool *return$) noexcept;

::GGRS::GGRSSessionState GGRS$cxxbridge1$get_current_state(::GGRS::GGRSSession *session) noexcept;

void GGRS$cxxbridge1$get_events(::GGRS::GGRSSession *session, ::rust::Vec<::GGRS::GGRSEvent> *return$) noexcept;

::rust::repr::PtrLen GGRS$cxxbridge1$advance_frame(::GGRS::GGRSSession *session, ::GGRS::GGRSFrameResult *return$) noexcept;

::std::int32_t GGRS$cxxbridge1$get_frames_ahead(::GGRS::GGRSSession *session) noexcept;

bool GGRS$cxxbridge1$clean_session(::GGRS::GGRSSession *session) noexcept;
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

bool add_local_input(::GGRS::GGRSSession *session, ::std::uint32_t player_handle, ::std::uint32_t input) {
  ::rust::MaybeUninit<bool> return$;
  ::rust::repr::PtrLen error$ = GGRS$cxxbridge1$add_local_input(session, player_handle, input, &return$.value);
  if (error$.ptr) {
    throw ::rust::impl<::rust::Error>::error(error$);
  }
  return ::std::move(return$.value);
}

::GGRS::GGRSSessionState get_current_state(::GGRS::GGRSSession *session) noexcept {
  return GGRS$cxxbridge1$get_current_state(session);
}

::rust::Vec<::GGRS::GGRSEvent> get_events(::GGRS::GGRSSession *session) noexcept {
  ::rust::MaybeUninit<::rust::Vec<::GGRS::GGRSEvent>> return$;
  GGRS$cxxbridge1$get_events(session, &return$.value);
  return ::std::move(return$.value);
}

::GGRS::GGRSFrameResult advance_frame(::GGRS::GGRSSession *session) {
  ::rust::MaybeUninit<::GGRS::GGRSFrameResult> return$;
  ::rust::repr::PtrLen error$ = GGRS$cxxbridge1$advance_frame(session, &return$.value);
  if (error$.ptr) {
    throw ::rust::impl<::rust::Error>::error(error$);
  }
  return ::std::move(return$.value);
}

::std::int32_t get_frames_ahead(::GGRS::GGRSSession *session) noexcept {
  return GGRS$cxxbridge1$get_frames_ahead(session);
}

bool clean_session(::GGRS::GGRSSession *session) noexcept {
  return GGRS$cxxbridge1$clean_session(session);
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

void cxxbridge1$rust_vec$GGRS$GGRSFrameAction$new(const ::rust::Vec<::GGRS::GGRSFrameAction> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSFrameAction$drop(::rust::Vec<::GGRS::GGRSFrameAction> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSFrameAction$len(const ::rust::Vec<::GGRS::GGRSFrameAction> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSFrameAction$capacity(const ::rust::Vec<::GGRS::GGRSFrameAction> *ptr) noexcept;
const ::GGRS::GGRSFrameAction *cxxbridge1$rust_vec$GGRS$GGRSFrameAction$data(const ::rust::Vec<::GGRS::GGRSFrameAction> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSFrameAction$reserve_total(::rust::Vec<::GGRS::GGRSFrameAction> *ptr, ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSFrameAction$set_len(::rust::Vec<::GGRS::GGRSFrameAction> *ptr, ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSFrameAction$truncate(::rust::Vec<::GGRS::GGRSFrameAction> *ptr, ::std::size_t len) noexcept;

void cxxbridge1$rust_vec$GGRS$GGRSInput$new(const ::rust::Vec<::GGRS::GGRSInput> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSInput$drop(::rust::Vec<::GGRS::GGRSInput> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSInput$len(const ::rust::Vec<::GGRS::GGRSInput> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSInput$capacity(const ::rust::Vec<::GGRS::GGRSInput> *ptr) noexcept;
const ::GGRS::GGRSInput *cxxbridge1$rust_vec$GGRS$GGRSInput$data(const ::rust::Vec<::GGRS::GGRSInput> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSInput$reserve_total(::rust::Vec<::GGRS::GGRSInput> *ptr, ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSInput$set_len(::rust::Vec<::GGRS::GGRSInput> *ptr, ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSInput$truncate(::rust::Vec<::GGRS::GGRSInput> *ptr, ::std::size_t len) noexcept;

void cxxbridge1$rust_vec$GGRS$GGRSEvent$new(const ::rust::Vec<::GGRS::GGRSEvent> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSEvent$drop(::rust::Vec<::GGRS::GGRSEvent> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSEvent$len(const ::rust::Vec<::GGRS::GGRSEvent> *ptr) noexcept;
::std::size_t cxxbridge1$rust_vec$GGRS$GGRSEvent$capacity(const ::rust::Vec<::GGRS::GGRSEvent> *ptr) noexcept;
const ::GGRS::GGRSEvent *cxxbridge1$rust_vec$GGRS$GGRSEvent$data(const ::rust::Vec<::GGRS::GGRSEvent> *ptr) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSEvent$reserve_total(::rust::Vec<::GGRS::GGRSEvent> *ptr, ::std::size_t new_cap) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSEvent$set_len(::rust::Vec<::GGRS::GGRSEvent> *ptr, ::std::size_t len) noexcept;
void cxxbridge1$rust_vec$GGRS$GGRSEvent$truncate(::rust::Vec<::GGRS::GGRSEvent> *ptr, ::std::size_t len) noexcept;
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
template <>
Vec<::GGRS::GGRSFrameAction>::Vec() noexcept {
  cxxbridge1$rust_vec$GGRS$GGRSFrameAction$new(this);
}
template <>
void Vec<::GGRS::GGRSFrameAction>::drop() noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSFrameAction$drop(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSFrameAction>::size() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSFrameAction$len(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSFrameAction>::capacity() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSFrameAction$capacity(this);
}
template <>
const ::GGRS::GGRSFrameAction *Vec<::GGRS::GGRSFrameAction>::data() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSFrameAction$data(this);
}
template <>
void Vec<::GGRS::GGRSFrameAction>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSFrameAction$reserve_total(this, new_cap);
}
template <>
void Vec<::GGRS::GGRSFrameAction>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSFrameAction$set_len(this, len);
}
template <>
void Vec<::GGRS::GGRSFrameAction>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$GGRS$GGRSFrameAction$truncate(this, len);
}
template <>
Vec<::GGRS::GGRSInput>::Vec() noexcept {
  cxxbridge1$rust_vec$GGRS$GGRSInput$new(this);
}
template <>
void Vec<::GGRS::GGRSInput>::drop() noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSInput$drop(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSInput>::size() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSInput$len(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSInput>::capacity() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSInput$capacity(this);
}
template <>
const ::GGRS::GGRSInput *Vec<::GGRS::GGRSInput>::data() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSInput$data(this);
}
template <>
void Vec<::GGRS::GGRSInput>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSInput$reserve_total(this, new_cap);
}
template <>
void Vec<::GGRS::GGRSInput>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSInput$set_len(this, len);
}
template <>
void Vec<::GGRS::GGRSInput>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$GGRS$GGRSInput$truncate(this, len);
}
template <>
Vec<::GGRS::GGRSEvent>::Vec() noexcept {
  cxxbridge1$rust_vec$GGRS$GGRSEvent$new(this);
}
template <>
void Vec<::GGRS::GGRSEvent>::drop() noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSEvent$drop(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSEvent>::size() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSEvent$len(this);
}
template <>
::std::size_t Vec<::GGRS::GGRSEvent>::capacity() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSEvent$capacity(this);
}
template <>
const ::GGRS::GGRSEvent *Vec<::GGRS::GGRSEvent>::data() const noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSEvent$data(this);
}
template <>
void Vec<::GGRS::GGRSEvent>::reserve_total(::std::size_t new_cap) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSEvent$reserve_total(this, new_cap);
}
template <>
void Vec<::GGRS::GGRSEvent>::set_len(::std::size_t len) noexcept {
  return cxxbridge1$rust_vec$GGRS$GGRSEvent$set_len(this, len);
}
template <>
void Vec<::GGRS::GGRSEvent>::truncate(::std::size_t len) {
  return cxxbridge1$rust_vec$GGRS$GGRSEvent$truncate(this, len);
}
} // namespace cxxbridge1
} // namespace rust
