use std::{net::SocketAddr, fmt::{self, Display}};
use ggrs::{Config, P2PSession, SessionBuilder, SpectatorSession, SyncTestSession, PlayerType, UdpNonBlockingSocket, SessionState, GGRSEvent, PlayerHandle, GGRSRequest};
use wrapper::{GGRSPlayer, GGRSSessionType, GGRSSessionInfo, GGRSPlayerType, GGRSSessionState, GGRSEventType, GGRSFrameResult, GGRSFrameAction, GGRSFrameActionType, GGRSFrameActionInfo, GGRSInput, GGRSInputStatus};

#[cxx::bridge(namespace = "GGRS")]
mod wrapper {

    struct GGRSSessionInfo{
        session_started: bool,
        session_type: GGRSSessionType,
        num_players: u32,
        fps: u32,
        input_delay: u32,
        max_prediction_frames: u32,
        max_frames_behind: u32,
        catchup_speed: u32,
        check_distance: u32,
        local_port: u16,
        host: String,
        sparse_saving: bool,
        players: Vec<GGRSPlayer>
    }

    struct GGRSPlayer{
        player_handle: u32,
        player_type: GGRSPlayerType,
        socket_addr: String
    }

    enum GGRSPlayerType {
        Local,
        Remote,
        Spectator,
    }

    enum GGRSSessionType {
        NotSet,
        Peer2Peer,
        Spectator,
        Synctest,
    }

    enum GGRSSessionState {
        Running,
        Synchronizing,
    }

    enum GGRSEventType {
        Empty,
        Synchronizing,
        Synchronized,
        Disconnected,
        NetworkInterrupted,
        NetworkResumed,
        WaitRecommendation, 
    }

    struct GGRSEventInfo {
        addr: String,
        total: u32,
        count: u32,
        disconnect_timeout: u64, // should be 128 but its not supported yet by cxx
        skip_frames: u32,
    }

    struct GGRSEvent {
        event_type: GGRSEventType,
        event_info: GGRSEventInfo,
    }

    struct GGRSFrameResult{
        skip_frame: bool,
        actions: Vec<GGRSFrameAction>,
    }

    struct GGRSFrameAction {
        action_type: GGRSFrameActionType,
        action_info: GGRSFrameActionInfo,
    }

    enum GGRSFrameActionType{
        SaveGameState,
        LoadGameState,
        AdvanceFrame,
    }

    struct GGRSFrameActionInfo{
        frame: i32,
        inputs: Vec<GGRSInput>,
    }

    struct GGRSInput{
        input: u32,
        status: GGRSInputStatus,
    }

    enum GGRSInputStatus{
        Confirmed,
        Predicted,
        Disconnected,
    }

    extern "Rust" {
        type GGRSSession;
        // I like when my outwards facing functions return something. helps with testing
        // setup functions and helpers
        fn setup_ggrs_info(info: &mut GGRSSessionInfo) -> bool;
        fn add_player(info: &mut GGRSSessionInfo, player: GGRSPlayer) -> bool;
        fn setup_p2p_session(info: &mut GGRSSessionInfo, local_port: u16, fps: u32, input_delay: u32, max_prediction_frames: u32) -> bool;
        fn setup_spectator_session(info: &mut GGRSSessionInfo, local_port: u16, host: String, max_frames_behind: u32, catchup_speed: u32) -> bool;
        fn setup_synctest_session(info: &mut GGRSSessionInfo, check_distance: u32, input_delay: u32) -> bool;
        fn set_num_players(info: &mut GGRSSessionInfo, num: u32) -> bool;
        fn set_sparse_saving(info: &mut GGRSSessionInfo, enable: bool) -> bool;
        // session creation and event handling
        // since boxes don't want to work with cxx i have to use unsafe raw pointers.
        // looking for better solutions...
        fn create_session(info: &mut GGRSSessionInfo) -> Result<*mut GGRSSession>;
        unsafe fn poll_remote_clients(mut session: *mut GGRSSession) -> bool;
        unsafe fn add_local_input(mut session: *mut GGRSSession, player_handle: u32, input: u32) -> Result<bool>;
        unsafe fn get_current_state(mut session: *mut GGRSSession) -> GGRSSessionState;
        unsafe fn get_events(mut session: *mut GGRSSession) -> Vec<GGRSEvent>;
        unsafe fn advance_frame(mut session: *mut GGRSSession) -> Result<GGRSFrameResult>;
        unsafe fn get_frames_ahead(mut session: *mut GGRSSession) -> i32;
        // test
        fn test_lib(num: i32) -> i32;
    }
}

fn test_lib(num: i32) -> i32{
    num * 7
}

pub struct GGRSConfig;
impl Config for GGRSConfig {
    type Input = u32; // good for now         
    type State = u8; // good for now not going to use this anyways just gonna supply the user the frame number of action      
    type Address = SocketAddr; // maybe add a way that people can change their socket type. we'll see later.       
}

impl Default for GGRSFrameAction {
    fn default() -> Self {
        Self { 
            action_type: GGRSFrameActionType::AdvanceFrame,
            action_info: GGRSFrameActionInfo::default()
        }
    }
}

impl Default for GGRSFrameActionInfo {
    fn default() -> Self {
        Self { 
            frame: 0,
            inputs: Vec::new()
        }
    }
}

impl Default for GGRSFrameResult {
    fn default() -> Self {
        Self { 
            skip_frame: false,
            actions: Vec::new()
        }
    }
}

impl Default for wrapper::GGRSEvent {
    fn default() -> Self {
        Self { 
            event_type: wrapper::GGRSEventType::Empty,
            event_info: wrapper::GGRSEventInfo { 
                addr: "".to_string(),
                total: 0,
                count: 0,
                disconnect_timeout: 0,
                skip_frames: 0 
            }
        }
    }
}

impl Default for GGRSSession{
    fn default() -> Self {
        GGRSSession::NotSet
    }
}
pub enum GGRSSession {
    NotSet,
    Peer2Peer(P2PSession<GGRSConfig>),
    Spectator(SpectatorSession<GGRSConfig>),
    Synctest(SyncTestSession<GGRSConfig>)
}

impl Default for GGRSSessionInfo{
    fn default() -> Self {
        Self { 
            session_started: false,
            session_type: GGRSSessionType::NotSet,
            num_players: 2, // amount of local and remote players (non-spectator)
            fps: 60, // FPS defines the expected update frequency of this session
            input_delay: 0, // local player input delay
            max_prediction_frames: 8,
            max_frames_behind: 10, // If the spectator is more than this amount of frames behind, it will advance the game two steps at a time to catch up
            catchup_speed: 1, // The amount of frames the spectator advances in a single step if too far behind
            check_distance: 2, // distance where the synctest is triggerd
            local_port: 1234,
            sparse_saving: false, // enable encourage a more conservative saving pattern.
            host: String::new(),
            players: Vec::new(),
        }
    }
} 

impl GGRSSessionInfo {
    fn setup(&mut self){
        let tmp = GGRSSessionInfo::default();
        // set default values
        self.session_type = tmp.session_type;
        self.num_players = tmp.num_players;
        self.fps = tmp.fps;
        self.input_delay = tmp.input_delay;
        self.max_prediction_frames = tmp.max_prediction_frames;
        self.max_frames_behind = tmp.max_frames_behind;
        self.catchup_speed = tmp.catchup_speed;
        self.check_distance = tmp.check_distance;
        self.local_port = tmp.local_port;
        self.sparse_saving = tmp.sparse_saving;
        self.host = String::new();
        self.players = Vec::new();
        self.session_started = tmp.session_started;
    }

    fn set_num_players(&mut self, num_players: u32){
        self.num_players = num_players;
    }

    fn set_sparse_saving(&mut self, enable: bool){
        self.sparse_saving = enable;
    }

    fn add_player(&mut self, player: GGRSPlayer){
        self.players.push(player);
    }
}

// outward facing functions

fn setup_ggrs_info(info: &mut GGRSSessionInfo) -> bool{
    // should be called before any other function to set default values.
    info.setup();
    return true;
}

fn add_player(info: &mut GGRSSessionInfo, player: GGRSPlayer) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.add_player(player);
        return true;
    }
    return false;
}

fn setup_p2p_session(info: &mut GGRSSessionInfo, local_port: u16, fps: u32, input_delay: u32, max_prediction_frames: u32) -> bool{
    if info.session_type == GGRSSessionType::NotSet{
        info.session_type = GGRSSessionType::Peer2Peer;
        info.max_prediction_frames = max_prediction_frames;
        info.input_delay = input_delay;
        info.local_port = local_port;
        info.fps = fps;
        return true;
    }
    return false;
}

fn setup_spectator_session(info: &mut GGRSSessionInfo, local_port: u16, host: String, max_frames_behind: u32, catchup_speed: u32) -> bool{
    if info.session_type == GGRSSessionType::NotSet{
        info.session_type = GGRSSessionType::Spectator;
        info.max_frames_behind = max_frames_behind;
        info.catchup_speed = catchup_speed;
        info.local_port = local_port;
        info.host = host;
        return true;
    }
    return false;
}

fn setup_synctest_session(info: &mut GGRSSessionInfo, check_distance: u32, input_delay: u32) -> bool{
    if info.session_type == GGRSSessionType::NotSet{
        info.session_type = GGRSSessionType::Synctest;
        info.check_distance = check_distance;
        info.input_delay = input_delay;
        return true;
    }
    return false;
}

fn set_num_players(info: &mut GGRSSessionInfo, num: u32) -> bool{
    // should be set before calling any session setup
    if info.session_type == GGRSSessionType::NotSet{
        info.set_num_players(num);
        return true;
    }
    return false;
}

fn set_sparse_saving(info: &mut GGRSSessionInfo, enable: bool) -> bool{
    // should be set before calling any session setup
    if info.session_type == GGRSSessionType::NotSet{
        info.set_sparse_saving(enable);
        return true;
    }
    return false;
}

fn create_session(info: &mut GGRSSessionInfo) -> Result<*mut GGRSSession, Error>{
    if info.session_type != GGRSSessionType::NotSet && !info.session_started {
        let mut sess_build = SessionBuilder::<GGRSConfig>::new()
            .with_num_players(info.num_players as usize)
            .with_sparse_saving_mode(info.sparse_saving);

        // add players
        for p in &info.players{
            match p.player_type {
                GGRSPlayerType::Local => {
                    sess_build = match sess_build.add_player(PlayerType::Local, p.player_handle as PlayerHandle) {
                        Ok(it) => it,
                        Err(err) => return Err(Error{msg: err.to_string()}),
                    };
                },
                GGRSPlayerType::Remote => {
                    let sock: SocketAddr = match p.socket_addr.parse() {
                        Ok(it) => it,
                        Err(_) => return Err(Error{msg: "Error parsing remote player address".to_string()}),
                    };

                    sess_build = match sess_build.add_player(PlayerType::Remote(sock), p.player_handle as PlayerHandle) {
                        Ok(it) => it,
                        Err(err) => return Err(Error{msg: err.to_string()}),
                    };
                },
                GGRSPlayerType::Spectator => {
                    let sock: SocketAddr = match p.socket_addr.parse() {
                        Ok(it) => it,
                        Err(_) => return Err(Error{msg: "Error parsing remote player address".to_string()}),
                    };

                    sess_build = match sess_build.add_player(PlayerType::Spectator(sock), p.player_handle as PlayerHandle) {
                        Ok(it) => it,
                        Err(err) => return Err(Error{msg: err.to_string()}),
                    };
                },
                _ => return Err(Error{msg: "Error unsupported player type".to_string()})
            }
        }
        let session: GGRSSession;
        // create the session
        match info.session_type {
            GGRSSessionType::Peer2Peer => {
                sess_build = match sess_build.with_fps(info.fps as usize) {
                    Ok(it) => it,
                    Err(err) => return Err(Error{msg: err.to_string()}),
                };

                sess_build = sess_build
                    .with_input_delay(info.input_delay as usize) 
                    .with_max_prediction_window(info.max_prediction_frames as usize);

                let sock = match UdpNonBlockingSocket::bind_to_port(info.local_port) {
                    Ok(it) => it,
                    Err(_) => return Err(Error{msg: format!("Error Couldnt bind to port: {}", info.local_port)}),
                };

                session = GGRSSession::Peer2Peer(match sess_build.start_p2p_session(sock) {
                    Ok(it) => it,
                    Err(err) => return Err(Error{msg: err.to_string()}),
                });
            },
            GGRSSessionType::Spectator => {
                sess_build = match sess_build.with_max_frames_behind(info.max_frames_behind as usize) {
                    Ok(it) => it,
                    Err(err) => return Err(Error{msg: err.to_string()}),
                };

                sess_build = match sess_build.with_catchup_speed(info.catchup_speed as usize) {
                    Ok(it) => it,
                    Err(err) => return Err(Error{msg: err.to_string()}),
                };

                let sock = match UdpNonBlockingSocket::bind_to_port(info.local_port) {
                    Ok(it) => it,
                    Err(_) => return Err(Error{msg: format!("Error Couldnt bind to port: {}", info.local_port)}),
                };

                let host_addr: SocketAddr = match info.host.parse() {
                    Ok(it) => it,
                    Err(_) => return Err(Error{msg: "Error parsing host player address".to_string()}),
                };

                session = GGRSSession::Spectator(sess_build.start_spectator_session(host_addr, sock));
            },
            GGRSSessionType::Synctest => {
                sess_build = sess_build
                    .with_check_distance(info.check_distance as usize)
                    .with_input_delay(info.input_delay as usize);

                session = GGRSSession::Synctest(match sess_build.start_synctest_session() {
                    Ok(it) => it,
                    Err(err) => return Err(Error{msg: err.to_string()}),
                });
            },
            _ => return Err(Error{msg: "Error unsupported session type".to_string()})
        }
        info.session_started = true;
        // return the created session
        return Ok(Box::into_raw(Box::new(session)));
    }
    Err(Error{msg: "Error session already started or session type not set".to_string()})
}

#[allow(unused_assignments)]
fn poll_remote_clients(mut session: *mut GGRSSession) -> bool{
    let mut sess = unsafe { Box::from_raw(session)};
    match sess.as_mut() {
        GGRSSession::NotSet => (),
        GGRSSession::Peer2Peer( sess) => {sess.poll_remote_clients()},
        GGRSSession::Spectator( sess) => {sess.poll_remote_clients()},
        GGRSSession::Synctest(_) => (),
    }
    session = Box::into_raw(sess);
    return true;
}

#[allow(unused_assignments)]
fn add_local_input(mut session: *mut GGRSSession, player_handle: u32, input: u32) -> Result<bool, Error>{
    let mut sess = unsafe { Box::from_raw(session)};
    let mut has_failed = false;

    match sess.as_mut(){
        GGRSSession::NotSet => (),
        GGRSSession::Peer2Peer(sess) => { 
            if sess.add_local_input(player_handle as usize, input).is_err(){
                has_failed = true;
            }
        },
        GGRSSession::Spectator(_) => (),
        GGRSSession::Synctest(sess) => {
            if sess.add_local_input(player_handle as usize, input).is_err(){
                has_failed = true;
            }
        },
    }

    session = Box::into_raw(sess);

    if has_failed {
        return Err(Error{msg: "Error player is not a local player".to_string()})
    }
    Ok(true)
}

#[allow(unused_assignments)]
fn get_current_state(mut session: *mut GGRSSession) -> GGRSSessionState {
    let mut sess = unsafe { Box::from_raw(session)};
    let mut state = SessionState::Synchronizing;

    match sess.as_mut() {
        GGRSSession::NotSet => (),
        GGRSSession::Peer2Peer(sess) => {
           state = sess.current_state();
        },
        GGRSSession::Spectator(sess) => {
           state = sess.current_state();
        },
        GGRSSession::Synctest(_) => ()
    }

    session = Box::into_raw(sess);
    let ggrs_state: GGRSSessionState;

    match state {
        SessionState::Synchronizing => ggrs_state = GGRSSessionState::Synchronizing,
        SessionState::Running => ggrs_state = GGRSSessionState::Running,
    }

    return ggrs_state;
}

#[allow(unused_assignments)]
fn get_events(mut session: *mut GGRSSession) -> Vec<wrapper::GGRSEvent>{
    let mut sess = unsafe { Box::from_raw(session)};
    let mut ggrs_events = Vec::new();
    match sess.as_mut() {
        GGRSSession::NotSet => (),
        GGRSSession::Peer2Peer(sess) => {
            for ev in sess.events(){
                ggrs_events.push(ev);
            } 
        },
        GGRSSession::Spectator(sess) => {
            for ev in sess.events(){
                ggrs_events.push(ev);
            } 
        },
        GGRSSession::Synctest(_) => (),
    }
    session = Box::into_raw(sess);
    let mut result = Vec::new();
        for event in ggrs_events{
            let mut ev = wrapper::GGRSEvent::default();
            match event {
                GGRSEvent::Synchronizing { addr, total, count } => {
                    ev.event_type = GGRSEventType::Synchronizing;
                    ev.event_info.addr = addr.to_string();
                    ev.event_info.total = total;
                    ev.event_info.count = count;
                },
                GGRSEvent::Synchronized { addr } => {
                    ev.event_type = GGRSEventType::Synchronized;
                    ev.event_info.addr = addr.to_string();
                },
                GGRSEvent::Disconnected { addr } => {
                    ev.event_type = GGRSEventType::Disconnected;
                    ev.event_info.addr = addr.to_string();
                },
                GGRSEvent::NetworkInterrupted { addr, disconnect_timeout } => {
                    ev.event_type = GGRSEventType::NetworkInterrupted;
                    ev.event_info.addr = addr.to_string();
                    ev.event_info.disconnect_timeout = disconnect_timeout as u64;
                },
                GGRSEvent::NetworkResumed { addr } => {
                    ev.event_type = GGRSEventType::NetworkResumed;
                    ev.event_info.addr = addr.to_string();
                },
                GGRSEvent::WaitRecommendation { skip_frames } => {
                    ev.event_type = GGRSEventType::WaitRecommendation;
                    ev.event_info.skip_frames = skip_frames;
                },
            }
            result.push(ev);
    }
    return result;
}

#[allow(unused_assignments)]
fn advance_frame(mut session: *mut GGRSSession) -> Result<GGRSFrameResult, Error>{
    let mut sess = unsafe { Box::from_raw(session)};
    let mut result = GGRSFrameResult::default();
    match sess.as_mut(){
        GGRSSession::NotSet => (),
        GGRSSession::Peer2Peer(sess) => {
            match sess.advance_frame(){
                Ok(reqs) => {
                    handle_requests(reqs, &mut result);
                },
                Err(ggrs::GGRSError::PredictionThreshold) => result.skip_frame = true,
                Err(err) => return Err(Error{msg: err.to_string()})
            }
        },
        GGRSSession::Spectator(sess) => {
            match sess.advance_frame(){
                Ok(reqs) => {
                    handle_requests(reqs, &mut result);
                },
                Err(ggrs::GGRSError::PredictionThreshold) => result.skip_frame = true,
                Err(err) => return Err(Error{msg: err.to_string()})
            }
        },
        GGRSSession::Synctest(sess) => {
            match sess.advance_frame(){
                Ok(reqs) => {
                    handle_requests(reqs, &mut result);
                },
                Err(ggrs::GGRSError::PredictionThreshold) => result.skip_frame = true,
                Err(err) => return Err(Error{msg: err.to_string()})
            }
        },
    }
    session = Box::into_raw(sess);
    Ok(result)
}

fn handle_requests(reqs: Vec<GGRSRequest<GGRSConfig>>, result: &mut GGRSFrameResult){
    for req in reqs{
        match req{
            ggrs::GGRSRequest::SaveGameState { cell: _, frame } => {
                let mut act = GGRSFrameAction::default();
                act.action_info.frame = frame;
                act.action_type = GGRSFrameActionType::SaveGameState;
                result.actions.push(act);
            },
            ggrs::GGRSRequest::LoadGameState { cell: _, frame } => {
                let mut act = GGRSFrameAction::default();
                act.action_info.frame = frame;
                act.action_type = GGRSFrameActionType::LoadGameState;
                result.actions.push(act);
            },
            ggrs::GGRSRequest::AdvanceFrame { inputs } => {
                let mut act = GGRSFrameAction::default();
                act.action_type = GGRSFrameActionType::AdvanceFrame;
                for (input, status) in inputs{
                    let stat = match status{
                        ggrs::InputStatus::Confirmed => GGRSInputStatus::Confirmed,
                        ggrs::InputStatus::Predicted => GGRSInputStatus::Predicted,
                        ggrs::InputStatus::Disconnected => GGRSInputStatus::Disconnected,
                    };
                    act.action_info.inputs.push(GGRSInput{input, status: stat});
                }
                result.actions.push(act);
            },
        }
    }
}

#[allow(unused_assignments)]
fn get_frames_ahead(mut session: *mut GGRSSession) -> i32{
    let mut sess = unsafe { Box::from_raw(session)};
    let mut ahead = 0;
    match sess.as_mut() {
        GGRSSession::NotSet | GGRSSession::Spectator(_) | GGRSSession::Synctest(_) => (),
        GGRSSession::Peer2Peer(sess) => ahead = sess.frames_ahead(),
    }
    session = Box::into_raw(sess);
    return ahead;
}

#[derive(Debug)]
struct Error{
    msg: String
}

impl std::error::Error for Error {}

impl Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.write_str(format!("INTERNAL RUST ERROR! msg: {}", self.msg).as_str())
    }
}