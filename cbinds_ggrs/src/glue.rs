use ggrs::GGRSRequest::{AdvanceFrame, LoadGameState, SaveGameState};
use ggrs::{
    Config, GGRSError, GGRSEvent, GGRSRequest, P2PSession, PlayerHandle, PlayerType,
    SessionBuilder, SessionState, SpectatorSession, SyncTestSession, UdpNonBlockingSocket,
};
use interoptopus::patterns::slice::FFISlice;
use interoptopus::patterns::string::AsciiPointer;
use interoptopus::{callback, ffi_type};
use std::collections::vec_deque::Drain;
use std::ffi::{CString, NulError};
use std::net::SocketAddr;

#[derive(Debug)]
pub struct UGGRSConfig;
impl Config for UGGRSConfig {
    type Input = u16;
    type State = u32;
    type Address = SocketAddr;
}

#[ffi_type]
#[repr(C)]
#[derive(PartialEq)]
pub enum UGGRSPlayerType {
    Local,
    Remote,
    Spectator,
}

#[ffi_type]
#[repr(C)]
#[derive(PartialEq)]
pub enum UGGRSSessionType {
    P2P,
    Spectator,
    SyncTest,
}

#[ffi_type]
#[repr(C)]
#[derive(PartialEq)]
pub enum UGGRSSesstionState {
    Synchronizing,
    Running,
    Error,
}

impl Default for UGGRSSessionType {
    fn default() -> Self {
        UGGRSSessionType::P2P
    }
}

#[ffi_type]
#[repr(C)]
pub struct UGGRSSessionInfo {
    pub player_count: u32,
    pub max_pred_window: u32,
    pub fps: u32,
    pub input_delay: u32,
    pub check_distance: u32,
    pub max_frames_behind: u32,
    pub catchup_speed: u32,
}

impl Default for UGGRSSessionInfo {
    fn default() -> Self {
        Self {
            player_count: 2,
            max_pred_window: 8,
            fps: 60,
            input_delay: 2,
            check_distance: 2,
            max_frames_behind: 10,
            catchup_speed: 1,
        }
    }
}

callback!(AdvFrame(inputs: FFISlice<u16>) -> bool);
callback!(LoadAndSaveFrame(frame: i32) -> bool);

#[ffi_type]
#[repr(C)]
pub struct UGGRSRequests {
    pub advance_frame: AdvFrame,
    pub load_game_state: LoadAndSaveFrame,
    pub save_game_state: LoadAndSaveFrame,
}

callback!(SyncingEvent(addr: AsciiPointer, total: u32, count: u32));
callback!(AddrEvent(addr: AsciiPointer));
callback!(NetworkInterruptedEvent(addr: AsciiPointer, timeout: u64));
callback!(WaitRecommendationEvent(timeout: u32));

#[ffi_type]
#[repr(C)]
pub struct UGGRSEvents {
    pub syncing_event: SyncingEvent,
    pub synced_event: AddrEvent,
    pub disconnected_event: AddrEvent,
    pub network_interrupted_event: NetworkInterruptedEvent,
    pub network_resumed_event: AddrEvent,
    pub wait_recommendation_event: WaitRecommendationEvent,
}

#[ffi_type]
#[repr(C)]
pub struct UGGRSBuilder {
    pub info: UGGRSSessionInfo,
    pub sess_type: UGGRSSessionType,
    pub session: *const i32,
    pub session_started: bool,
}

impl UGGRSBuilder {
    pub fn add_local_input(&mut self, player_handle: u32, input: u16) -> Result<(), GGRSError> {
        if !self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "No session started".to_string(),
            });
        }
        if self.sess_type != UGGRSSessionType::P2P {
            return Err(GGRSError::InvalidRequest {
                info: "Session doesnt have this functionality!".to_string(),
            });
        }
        let mut sess = unsafe { Box::from_raw(self.session as *mut P2PSession<UGGRSConfig>) };
        if sess.add_local_input(player_handle as usize, input).is_err() {
            self.session = Box::into_raw(sess) as *const i32;
            return Err(GGRSError::InvalidRequest {
                info: "Error adding local input!".to_string(),
            });
        }
        self.session = Box::into_raw(sess) as *const i32;
        Ok(())
    }
    //
    pub fn handle_requests(&mut self, callbacks: &UGGRSRequests) -> Result<(), GGRSError> {
        if !self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "No session started".to_string(),
            });
        }
        match self.sess_type {
            UGGRSSessionType::P2P => {
                let mut sess =
                    unsafe { Box::from_raw(self.session as *mut P2PSession<UGGRSConfig>) };
                let reqs = sess.advance_frame();
                if reqs.is_err() {
                    // handle frame skip
                    match reqs {
                        Err(GGRSError::PredictionThreshold) => {
                            // put it back early
                            self.session = Box::into_raw(sess) as *const i32;
                            return Ok(());
                        }
                        _ => {
                            self.session = Box::into_raw(sess) as *const i32;
                            // return err
                            return Err(GGRSError::InvalidRequest {
                                info: "Couldnt process requests properly".to_string(),
                            });
                        }
                    }
                }
                let reqs = reqs.unwrap();
                if self.process_requests(callbacks, reqs).is_err() {
                    // put it back early
                    self.session = Box::into_raw(sess) as *const i32;
                    // return err
                    return Err(GGRSError::InvalidRequest {
                        info: "Couldnt process requests properly".to_string(),
                    });
                }
                // and put it back
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::Spectator => {
                let mut sess =
                    unsafe { Box::from_raw(self.session as *mut SpectatorSession<UGGRSConfig>) };
                let reqs = sess.advance_frame();
                if reqs.is_err() {
                    // handle frame skip
                    match reqs {
                        Err(GGRSError::PredictionThreshold) => {
                            // put it back early
                            self.session = Box::into_raw(sess) as *const i32;
                            return Ok(());
                        }
                        _ => {
                            self.session = Box::into_raw(sess) as *const i32;
                            // return err
                            return Err(GGRSError::InvalidRequest {
                                info: "Couldnt process requests properly".to_string(),
                            });
                        }
                    }
                }
                let reqs = reqs.unwrap();
                if self.process_requests(callbacks, reqs).is_err() {
                    self.session = Box::into_raw(sess) as *const i32;
                    return Err(GGRSError::InvalidRequest {
                        info: "Couldnt process requests properly".to_string(),
                    });
                }
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::SyncTest => {
                let mut sess =
                    unsafe { Box::from_raw(self.session as *mut SyncTestSession<UGGRSConfig>) };
                let reqs = sess.advance_frame();
                if reqs.is_err() {
                    self.session = Box::into_raw(sess) as *const i32;
                    return Err(GGRSError::InvalidRequest {
                        info: "Couldnt process requests properly".to_string(),
                    });
                }
                let reqs = reqs.unwrap();
                if self.process_requests(callbacks, reqs).is_err() {
                    self.session = Box::into_raw(sess) as *const i32;
                    return Err(GGRSError::InvalidRequest {
                        info: "Couldnt process requests properly".to_string(),
                    });
                }
                self.session = Box::into_raw(sess) as *const i32;
            }
        }
        Ok(())
    }
    //
    fn process_requests(
        &mut self,
        callbacks: &UGGRSRequests,
        requests: Vec<GGRSRequest<UGGRSConfig>>,
    ) -> Result<(), GGRSError> {
        for req in requests {
            let res = match req {
                SaveGameState { frame, .. } => callbacks.save_game_state.call(frame),
                LoadGameState { frame, .. } => callbacks.load_game_state.call(frame),
                AdvanceFrame { inputs } => {
                    let mut game_input = Vec::new();
                    for (inp, _) in inputs {
                        game_input.push(inp);
                    }
                    callbacks
                        .advance_frame
                        .call(FFISlice::from_slice(&game_input[..]))
                }
            };
            // the request should return true if the callback is implemented
            if !res {
                return Err(GGRSError::InvalidRequest {
                    info: "One of the mandatory requests is not implemented!".to_string(),
                });
            }
        }
        Ok(())
    }
    //
    pub fn handle_events(&mut self, callbacks: &UGGRSEvents) -> Result<(), GGRSError> {
        if !self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "No session started".to_string(),
            });
        }
        match self.sess_type {
            UGGRSSessionType::P2P => {
                let mut sess =
                    unsafe { Box::from_raw(self.session as *mut P2PSession<UGGRSConfig>) };
                if self.process_events(callbacks, sess.events()).is_err() {
                    // put it back early
                    self.session = Box::into_raw(sess) as *const i32;
                    return Err(GGRSError::InvalidRequest {
                        info: "Couldnt process events properly".to_string(),
                    });
                }
                // and put it back
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::Spectator => {
                let mut sess =
                    unsafe { Box::from_raw(self.session as *mut SpectatorSession<UGGRSConfig>) };
                if self.process_events(callbacks, sess.events()).is_err() {
                    // put it back early
                    self.session = Box::into_raw(sess) as *const i32;
                    return Err(GGRSError::InvalidRequest {
                        info: "Couldnt process events properly".to_string(),
                    });
                }
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::SyncTest => {
                return Err(GGRSError::InvalidRequest {
                    info: "SyncTest doesn't Have this functionality".to_string(),
                });
            }
        }
        Ok(())
    }
    //
    fn process_events(
        &mut self,
        callbacks: &UGGRSEvents,
        events: Drain<GGRSEvent<UGGRSConfig>>,
    ) -> Result<(), NulError> {
        for ev in events {
            match ev {
                GGRSEvent::Disconnected { addr } => {
                    let cstr = CString::new(addr.to_string())?;
                    callbacks
                        .disconnected_event
                        .call(AsciiPointer::from_cstr(&cstr))
                }
                GGRSEvent::NetworkInterrupted {
                    addr,
                    disconnect_timeout,
                } => {
                    let cstr = CString::new(addr.to_string())?;
                    callbacks
                        .network_interrupted_event
                        .call(AsciiPointer::from_cstr(&cstr), disconnect_timeout as u64);
                }
                GGRSEvent::NetworkResumed { addr } => {
                    let cstr = CString::new(addr.to_string())?;
                    callbacks
                        .network_resumed_event
                        .call(AsciiPointer::from_cstr(&cstr));
                }
                GGRSEvent::Synchronized { addr } => {
                    let cstr = CString::new(addr.to_string())?;
                    callbacks.synced_event.call(AsciiPointer::from_cstr(&cstr));
                }
                GGRSEvent::Synchronizing { addr, total, count } => {
                    let cstr = CString::new(addr.to_string())?;
                    callbacks
                        .syncing_event
                        .call(AsciiPointer::from_cstr(&cstr), total, count);
                }
                GGRSEvent::WaitRecommendation { skip_frames } => {
                    callbacks.wait_recommendation_event.call(skip_frames);
                }
            }
        }
        Ok(())
    }
    //
    pub fn current_state(&mut self) -> Result<UGGRSSesstionState, GGRSError> {
        if !self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "No session started".to_string(),
            });
        }
        let mut _state = UGGRSSesstionState::Error;
        match self.sess_type {
            UGGRSSessionType::P2P => {
                let sess = unsafe { Box::from_raw(self.session as *mut P2PSession<UGGRSConfig>) };
                if sess.current_state() == SessionState::Running {
                    _state = UGGRSSesstionState::Running;
                } else {
                    _state = UGGRSSesstionState::Synchronizing;
                }
                // and put it back
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::Spectator => {
                let sess =
                    unsafe { Box::from_raw(self.session as *mut SpectatorSession<UGGRSConfig>) };
                if sess.current_state() == SessionState::Running {
                    _state = UGGRSSesstionState::Running;
                } else {
                    _state = UGGRSSesstionState::Synchronizing;
                }
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::SyncTest => {
                return Err(GGRSError::InvalidRequest {
                    info: "SyncTest doesn't Have this functionality".to_string(),
                });
            }
        }
        Ok(_state)
    }
    //
    pub fn frames_ahead(&mut self) -> Result<i32, GGRSError> {
        if !self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "No session started".to_string(),
            });
        }
        if self.sess_type != UGGRSSessionType::P2P {
            return Err(GGRSError::InvalidRequest {
                info: "Session doesn't Have this functionality".to_string(),
            });
        }
        let sess = unsafe { Box::from_raw(self.session as *mut P2PSession<UGGRSConfig>) };
        let ahead = sess.frames_ahead();
        // and put it back
        self.session = Box::into_raw(sess) as *const i32;
        Ok(ahead)
    }
    //
    pub fn poll_remote_clients(&mut self) -> Result<(), GGRSError> {
        if !self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "No session started".to_string(),
            });
        }
        match self.sess_type {
            UGGRSSessionType::P2P => {
                let mut sess =
                    unsafe { Box::from_raw(self.session as *mut P2PSession<UGGRSConfig>) };
                sess.poll_remote_clients();
                // and put it back
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::Spectator => {
                let mut sess =
                    unsafe { Box::from_raw(self.session as *mut SpectatorSession<UGGRSConfig>) };
                sess.poll_remote_clients();
                self.session = Box::into_raw(sess) as *const i32;
            }
            UGGRSSessionType::SyncTest => {
                return Err(GGRSError::InvalidRequest {
                    info: "SyncTest doesn't Have this functionality".to_string(),
                });
            }
        }
        Ok(())
    }
    //
    pub fn setup_p2p(
        &mut self,
        np: u32,
        fps: u32,
        inpd: u32,
        max_pred: u32,
    ) -> Result<(), GGRSError> {
        if self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "Session is already started".to_string(),
            });
        }
        *self = Self::default();
        self.info.player_count = np;
        self.info.fps = fps;
        self.info.input_delay = inpd;
        self.info.max_pred_window = max_pred;
        self.sess_type = UGGRSSessionType::P2P;
        Ok(())
    }
    //
    pub fn setup_spectator(&mut self, np: u32, mfb: u32, cs: u32) -> Result<(), GGRSError> {
        if self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "Session is already started".to_string(),
            });
        }
        *self = Self::default();
        self.info.player_count = np;
        self.info.max_frames_behind = mfb;
        self.info.catchup_speed = cs;
        self.sess_type = UGGRSSessionType::Spectator;
        Ok(())
    }
    //
    pub fn setup_synctest(&mut self, np: u32, cd: u32, inpd: u32) -> Result<(), GGRSError> {
        if self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "Session is already started".to_string(),
            });
        }
        *self = Self::default();
        self.info.player_count = np;
        self.info.check_distance = cd;
        self.info.input_delay = inpd;
        self.sess_type = UGGRSSessionType::SyncTest;
        Ok(())
    }
    //
    pub fn create_session(&mut self) -> Result<(), GGRSError> {
        if self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "Session is already started".to_string(),
            });
        }
        let mut session = SessionBuilder::<UGGRSConfig>::new();
        match self.sess_type {
            UGGRSSessionType::P2P => {
                session = session
                    .with_num_players(self.info.player_count as usize)
                    .with_fps(self.info.fps as usize)?
                    .with_input_delay(self.info.input_delay as usize)
            }
            UGGRSSessionType::Spectator => {
                session = session
                    .with_num_players(self.info.player_count as usize)
                    .with_fps(self.info.fps as usize)?
                    .with_input_delay(self.info.input_delay as usize);
            }
            UGGRSSessionType::SyncTest => {
                session = session
                    .with_num_players(self.info.player_count as usize)
                    .with_check_distance(self.info.check_distance as usize)
                    .with_input_delay(self.info.input_delay as usize);
            }
        }
        // set session pointer
        let boxed_sess = Box::new(session);
        self.session = Box::into_raw(boxed_sess) as *const i32;
        Ok(())
    }
    //
    pub fn handle_player(
        &mut self,
        player_type: UGGRSPlayerType,
        player_handle: u32,
        addr: &str,
    ) -> Result<(), GGRSError> {
        match player_type {
            UGGRSPlayerType::Local => {
                self.add_player(PlayerType::Local, player_handle as usize)?;
            }
            UGGRSPlayerType::Remote => {
                let socket = addr.parse::<SocketAddr>();
                if socket.is_err() {
                    return Err(GGRSError::InvalidRequest {
                        info: "Cannot Add Player".to_string(),
                    });
                }
                let socket = socket.unwrap();
                self.add_player(PlayerType::Remote(socket), player_handle as usize)?;
            }
            UGGRSPlayerType::Spectator => {
                let socket = addr.parse::<SocketAddr>();
                if socket.is_err() {
                    return Err(GGRSError::InvalidRequest {
                        info: "Cannot Add Player".to_string(),
                    });
                }
                let socket = socket.unwrap();
                self.add_player(PlayerType::Spectator(socket), player_handle as usize)?;
            }
        }
        Ok(())
    }
    //
    fn add_player(
        &mut self,
        player_type: PlayerType<SocketAddr>,
        player_handle: PlayerHandle,
    ) -> Result<(), GGRSError> {
        // only allow adding players when the session hasnt started yet
        // but has been created
        if !(!self.session_started && self.session != 0 as *const i32) {
            return Err(GGRSError::InvalidRequest {
                info: "Cannot Add Player".to_string(),
            });
        }
        // try to retrieve the session builder from the session pointer
        // will be unsafe but idk a different way.. if you have an idea i'd like to hear it.
        let mut sess = unsafe { Box::from_raw(self.session as *mut SessionBuilder<UGGRSConfig>) };
        let res = sess.add_player(player_type, player_handle);
        if res.is_err() {
            sess = Box::new(res.unwrap());
            self.session = Box::into_raw(sess) as *const i32;
            return Err(GGRSError::InvalidRequest {
                info: "Cannot Add Player".to_string(),
            });
        } else {
            sess = Box::new(res.unwrap());
            // put session builder back where you found it
            self.session = Box::into_raw(sess) as *const i32;
        }
        Ok(())
    }
    //
    pub fn start_p2p_session(&mut self, local_port: u16) -> Result<(), GGRSError> {
        if self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "Session is already started".to_string(),
            });
        }
        let socket = UdpNonBlockingSocket::bind_to_port(local_port);
        if socket.is_err() {
            return Err(GGRSError::InvalidRequest {
                info: "Couldn't create socket".to_string(),
            });
        }
        let socket = socket.unwrap();

        let sess_build = unsafe { Box::from_raw(self.session as *mut SessionBuilder<UGGRSConfig>) };
        let res = sess_build.start_p2p_session(socket);
        if res.is_err() {
            let session = Box::new(res.unwrap());
            self.session = Box::into_raw(session) as *const i32;
            return Err(GGRSError::InvalidRequest {
                info: "Cannot start session".to_string(),
            });
        }
        let session = Box::new(res.unwrap());
        // session has been started
        self.session_started = true;
        // replace builder pointer with session pointer;
        self.session = Box::into_raw(session) as *const i32;
        Ok(())
    }
    //
    pub fn start_spectator_session(
        &mut self,
        host: SocketAddr,
        local_port: u16,
    ) -> Result<(), GGRSError> {
        if self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "Session is already started".to_string(),
            });
        }
        let socket = UdpNonBlockingSocket::bind_to_port(local_port);
        if socket.is_err() {
            return Err(GGRSError::InvalidRequest {
                info: "Couldn't create socket".to_string(),
            });
        }
        let socket = socket.unwrap();
        // try to retrieve the session builder from the session pointer
        // will be unsafe but idk a different way.. if you have an idea i'd like to hear it.
        let sess_build = unsafe { Box::from_raw(self.session as *mut SessionBuilder<UGGRSConfig>) };
        let session = Box::new(sess_build.start_spectator_session(host, socket));
        // session has been started
        self.session_started = true;
        // replace builder pointer with session pointer;
        self.session = Box::into_raw(session) as *const i32;
        Ok(())
    }
    //
    pub fn start_synctest_session(&mut self) -> Result<(), GGRSError> {
        if self.session_started {
            return Err(GGRSError::InvalidRequest {
                info: "Session is already started".to_string(),
            });
        }
        // try to retrieve the session builder from the session pointer
        // will be unsafe but idk a different way.. if you have an idea i'd like to hear it.
        let sess_build = unsafe { Box::from_raw(self.session as *mut SessionBuilder<UGGRSConfig>) };
        let res = sess_build.start_synctest_session();
        if res.is_err() {
            let session = Box::new(res.unwrap());
            self.session = Box::into_raw(session) as *const i32;
            return Err(GGRSError::InvalidRequest {
                info: "Cannot start session".to_string(),
            });
        }
        let session = Box::new(res.unwrap());
        // session has been started
        self.session_started = true;
        // replace builder pointer with session pointer;
        self.session = Box::into_raw(session) as *const i32;
        Ok(())
    }
    //
    pub fn cleanup_session(&mut self) -> Result<(), GGRSError> {
        if !self.session_started || self.session == 0 as *const i32 {
            return Err(GGRSError::InvalidRequest {
                info: "There's no session to be cleaned up!".to_string(),
            });
        }
        // essentially cleanup the session and reset the pointer
        match self.sess_type {
            UGGRSSessionType::P2P => {
                //Box will clean it at the end of the scope
                let _clean_sess =
                    unsafe { Box::from_raw(self.session as *mut P2PSession<UGGRSConfig>) };
            }
            UGGRSSessionType::Spectator => {
                let _clean_sess =
                    unsafe { Box::from_raw(self.session as *mut SpectatorSession<UGGRSConfig>) };
            }
            UGGRSSessionType::SyncTest => {
                let _clean_sess =
                    unsafe { Box::from_raw(self.session as *mut SyncTestSession<UGGRSConfig>) };
            }
        }
        // reset session pointer back to 0
        self.session = 0 as *const i32;
        self.session_started = false;
        // now the same session object could be reused by running setup_ -> create_ -> start_
        Ok(())
    }
}

impl Default for UGGRSBuilder {
    fn default() -> Self {
        Self {
            info: UGGRSSessionInfo::default(),
            sess_type: UGGRSSessionType::default(),
            session: 0 as *const i32,
            session_started: false,
        }
    }
}
