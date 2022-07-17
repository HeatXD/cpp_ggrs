#pragma once
#include "rust/cxx.h"

namespace GGRS {
  struct GGRSSessionInfo;
  struct GGRSPlayer;
  enum class GGRSErrorType : ::std::uint8_t;
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

#ifndef CXXBRIDGE1_ENUM_GGRS$GGRSErrorType
#define CXXBRIDGE1_ENUM_GGRS$GGRSErrorType
enum class GGRSErrorType : ::std::uint8_t {
  GGRSOk = 0,
  GGRSAddLocalPlayer = 1,
  GGRSSocketParse = 2,
  GGRSAddRemotePlayer = 3,
  GGRSAddSpectator = 4,
  GGRSPlayerTypeNotFound = 5,
  GGRSFailedSessionAlloc = 6,
  GGRSSessionCreation = 7,
  GGRSSocketBindToPort = 8,
  GGRSInvalidSessionType = 9,
  GGRSSessionStarted = 10,
  GGRSInvalidSessionPointer = 11,
  GGRSNotLocalPlayer = 12,
  GGRSAdvanceFrame = 13,
};
#endif // CXXBRIDGE1_ENUM_GGRS$GGRSErrorType

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

bool setup_ggrs_info(::GGRS::GGRSSessionInfo &info) noexcept;

bool add_player(::GGRS::GGRSSessionInfo &info, ::GGRS::GGRSPlayer player) noexcept;

bool setup_p2p_session(::GGRS::GGRSSessionInfo &info, ::std::uint16_t local_port, ::std::uint32_t fps, ::std::uint32_t input_delay, ::std::uint32_t max_prediction_frames) noexcept;

bool setup_spectator_session(::GGRS::GGRSSessionInfo &info, ::std::uint16_t local_port, ::rust::String host, ::std::uint32_t max_frames_behind, ::std::uint32_t catchup_speed) noexcept;

bool setup_synctest_session(::GGRS::GGRSSessionInfo &info, ::std::uint32_t check_distance, ::std::uint32_t input_delay) noexcept;

bool set_num_players(::GGRS::GGRSSessionInfo &info, ::std::uint32_t num) noexcept;

bool set_sparse_saving(::GGRS::GGRSSessionInfo &info, bool enable) noexcept;

::GGRS::GGRSSession *create_session(::GGRS::GGRSSessionInfo &info, ::GGRS::GGRSErrorType &err) noexcept;

::GGRS::GGRSErrorType poll_remote_clients(::GGRS::GGRSSession *session) noexcept;

::GGRS::GGRSErrorType add_local_input(::GGRS::GGRSSession *session, ::std::uint32_t player_handle, ::std::uint32_t input) noexcept;

::GGRS::GGRSSessionState get_current_state(::GGRS::GGRSSession *session, ::GGRS::GGRSErrorType &err) noexcept;

::rust::Vec<::GGRS::GGRSEvent> get_events(::GGRS::GGRSSession *session, ::GGRS::GGRSErrorType &err) noexcept;

::GGRS::GGRSFrameResult advance_frame(::GGRS::GGRSSession *session, ::GGRS::GGRSErrorType &err) noexcept;

::std::int32_t get_frames_ahead(::GGRS::GGRSSession *session, ::GGRS::GGRSErrorType &err) noexcept;

bool clean_session(::GGRS::GGRSSession *session) noexcept;
} // namespace GGRS
