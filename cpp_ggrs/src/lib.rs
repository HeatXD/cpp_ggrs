use core::num;
use std::net::SocketAddr;
use ggrs::Config;
use wrapper::{GGRSPlayer, GGRSSessionType, GGRSSessionInfo};

#[cxx::bridge(namespace = "GGRS")]
mod wrapper {

    struct GGRSSessionInfo{
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
        players: Vec<GGRSPlayer>,
    }

    struct GGRSPlayer{
        player_handle: u32,
        player_type: GGRSPlayerType,
        socket_addr: String
    }

    enum GGRSPlayerType {
        Local,
        Remote,
        Spectator
    }

    enum GGRSSessionType {
        NotSet,
        Peer2Peer,
        Spectator,
        Synctest,
    }

    extern "Rust" {
        // i like when my outwards facing functions return something. help with testing
        fn setup_ggrs_info(info: &mut GGRSSessionInfo) -> bool;
        fn add_player(info: &mut GGRSSessionInfo, player: GGRSPlayer) -> bool;
        fn setup_p2p_session(info: &mut GGRSSessionInfo, local_port: u16, fps: u32, input_delay: u32, max_prediction_frames: u32) -> bool;
        fn setup_spectator_session(info: &mut GGRSSessionInfo, local_port: u16, host: String, max_frames_behind: u32, catchup_speed: u32) -> bool;
        fn setup_synctest_session(info: &mut GGRSSessionInfo, check_distance: u32, input_delay: u32) -> bool;
        fn set_num_players(info: &mut GGRSSessionInfo, num: u32) -> bool;
        fn set_sparse_saving(info: &mut GGRSSessionInfo, enable: bool) -> bool;
        fn test_lib(num: i32) -> i32;
    }
}

fn test_lib(num: i32) -> i32{
    num * 5
}

pub struct GGRSConfig;
impl Config for GGRSConfig {
    type Input = u32; // good for now         
    type State = u8; // good for now not going to use this anyways just gonna supply the user the frame number of action      
    type Address = SocketAddr; // maybe add a way that people can change their socket type. we'll see later.       
}


impl Default for GGRSSessionInfo{
    fn default() -> Self {
        Self { 
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
            players: Vec::new()
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