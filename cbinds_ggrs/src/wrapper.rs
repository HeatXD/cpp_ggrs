use crate::glue::{UGGRSBuilder, UGGRSEvents, UGGRSPlayerType, UGGRSRequests, UGGRSSesstionState};
use interoptopus::ffi_function;
use interoptopus::patterns::string::AsciiPointer;
use std::net::SocketAddr;

#[ffi_function]
#[no_mangle]
pub extern "C" fn add_local_input(
    build: &mut UGGRSBuilder,
    player_handle: u32,
    input: u16,
) -> bool {
    if build.add_local_input(player_handle, input).is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn handle_requests(build: &mut UGGRSBuilder, callbacks: &mut UGGRSRequests) -> bool {
    if build.handle_requests(callbacks).is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn handle_events(build: &mut UGGRSBuilder, callbacks: &mut UGGRSEvents) -> bool {
    if build.handle_events(callbacks).is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn current_state(build: &mut UGGRSBuilder) -> UGGRSSesstionState {
    let res = build.current_state();
    if res.is_err() {
        return UGGRSSesstionState::Error;
    }
    return res.unwrap();
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn frames_ahead(build: &mut UGGRSBuilder) -> i32 {
    let res = build.frames_ahead();
    if res.is_err() {
        return 0;
    }
    return res.unwrap();
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn poll_remote_clients(build: &mut UGGRSBuilder) -> bool {
    if build.poll_remote_clients().is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn start_p2p_session(build: &mut UGGRSBuilder, local_port: u16) -> bool {
    if build.start_p2p_session(local_port).is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn start_spectator_session(
    build: &mut UGGRSBuilder,
    host_addr: AsciiPointer,
    local_port: u16,
) -> bool {
    let addr = host_addr.as_str();
    if addr.is_err() {
        return false;
    }
    let addr = addr.unwrap();
    let host = addr.parse::<SocketAddr>();
    if host.is_err() {
        return false;
    }
    let host = host.unwrap();
    if build.start_spectator_session(host, local_port).is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn start_synctest_session(build: &mut UGGRSBuilder) -> bool {
    if build.start_synctest_session().is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn setup_p2p_session(
    build: &mut UGGRSBuilder,
    num_players: u32,
    fps: u32,
    input_delay: u32,
    max_pred: u32,
) -> bool {
    if build
        .setup_p2p(num_players, fps, input_delay, max_pred)
        .is_err()
    {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn setup_spectator_session(
    build: &mut UGGRSBuilder,
    num_players: u32,
    max_frames_behind: u32,
    catchup_speed: u32,
) -> bool {
    if build
        .setup_spectator(num_players, max_frames_behind, catchup_speed)
        .is_err()
    {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn setup_synctest_session(
    build: &mut UGGRSBuilder,
    num_players: u32,
    check_distance: u32,
    input_delay: u32,
) -> bool {
    if build
        .setup_synctest(num_players, check_distance, input_delay)
        .is_err()
    {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn create_session(build: &mut UGGRSBuilder) -> bool {
    // create session
    if build.create_session().is_err() {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn add_player(
    build: &mut UGGRSBuilder,
    player_type: UGGRSPlayerType,
    player_handle: u32,
    socket_addr: AsciiPointer,
) -> bool {
    let addr = socket_addr.as_str();
    if addr.is_err() {
        return false;
    }
    let addr = addr.unwrap();
    if build
        .handle_player(player_type, player_handle, addr)
        .is_err()
    {
        return false;
    }
    return true;
}

#[ffi_function]
#[no_mangle]
pub extern "C" fn cleanup_session(build: &mut UGGRSBuilder) -> bool {
    if build.cleanup_session().is_err() {
        return false;
    }
    return true;
}
