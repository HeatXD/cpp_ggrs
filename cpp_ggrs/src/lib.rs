use std::net::SocketAddr;
use ggrs::Config;
use wrapper::{GGRSPlayer, GGRSSessionType};

#[cxx::bridge(namespace = "GGRS")]
mod wrapper {

    struct GGRSPlayer{
        player_handle: u32,
        player_type: GGRSPlayerType,
        socket_addr: String,
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
        type GGRSSessionInfo;

        fn setup_ggrs_info(info: &mut GGRSSessionInfo) -> bool;
        fn add_player(info: &mut GGRSSessionInfo, player: GGRSPlayer) -> bool;
        fn set_session_type(info: &mut GGRSSessionInfo, session_type: GGRSSessionType) -> bool;
        fn set_num_players(info: &mut GGRSSessionInfo, players: u32) -> bool;
        fn set_fps(info: &mut GGRSSessionInfo, fps: u32) -> bool;
        fn set_input_delay(info: &mut GGRSSessionInfo, delay: u32) -> bool;
        fn set_max_frames_behind(info: &mut GGRSSessionInfo, max_frames_behind: u32) -> bool;
        fn set_catchup_speed(info: &mut GGRSSessionInfo, catchup_speed: u32) -> bool;
        fn set_check_distance(info: &mut GGRSSessionInfo, check_distance: u32) -> bool;
        fn set_local_port(info: &mut GGRSSessionInfo, local_port: u16) -> bool;
    }
}

pub struct GGRSConfig;
impl Config for GGRSConfig {
    type Input = u32; // good for now         
    type State = u8; // good for now not going to use this anyways just gonna supply the user the frame number of action      
    type Address = SocketAddr; // maybe add a way that people can change their socket type. we'll see later.       
}

pub struct GGRSSessionInfo{
    session_type: GGRSSessionType,
    num_players: u32,
    fps: u32,
    input_delay: u32,
    max_frames_behind: u32,
    catchup_speed: u32,
    check_distance: u32,
    local_port: u16,
    players: Vec<GGRSPlayer>,
}

impl Default for GGRSSessionInfo{
    fn default() -> Self {
        Self { 
            session_type: GGRSSessionType::NotSet,
            num_players: 2,
            fps: 60,
            input_delay: 0,
            max_frames_behind: 8,
            catchup_speed: 1,
            check_distance: 2,
            local_port: 1234,
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
        self.max_frames_behind = tmp.max_frames_behind;
        self.catchup_speed = tmp.catchup_speed;
        self.check_distance = tmp.check_distance;
        self.local_port = tmp.local_port;
        self.players = Vec::new();
    }

    fn set_session_type(&mut self, session_type: GGRSSessionType){
        self.session_type = session_type;
    }

    fn set_num_players(&mut self, players: u32){
        self.num_players = players;
    }

    fn set_fps(&mut self, fps: u32){
        self.fps = fps;
    }

    fn set_input_delay(&mut self, delay: u32){
        self.input_delay = delay;
    }

    fn set_max_frames_behind(&mut self, max_frames_behind: u32){
        self.max_frames_behind = max_frames_behind;
    }

    fn set_catchup_speed(&mut self, catchup_speed: u32){
        self.catchup_speed = catchup_speed;
    }

    fn set_check_distance(&mut self, check_distance: u32){
        self.check_distance = check_distance;
    }

    fn set_local_port(&mut self, local_port: u16){
        self.local_port = local_port;
    }

    fn add_player(&mut self, player: GGRSPlayer){
        self.players.push(player);
    }
}

// outward facing functions

fn setup_ggrs_info(info: &mut GGRSSessionInfo) -> bool{
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

fn set_session_type(info: &mut GGRSSessionInfo, session_type: GGRSSessionType) -> bool{
    if info.session_type == GGRSSessionType::NotSet{
        info.set_session_type(session_type);
        return true;
    }
    return false;
}

fn set_num_players(info: &mut GGRSSessionInfo, players: u32) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.set_num_players(players);
        return true;
    }
    return false;
}

fn set_fps(info: &mut GGRSSessionInfo, fps: u32) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.set_fps(fps);
        return true;
    }
    return false;
}

fn set_input_delay(info: &mut GGRSSessionInfo, delay: u32) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.set_input_delay(delay);
        return true;
    }
    return false;
}

fn set_max_frames_behind(info: &mut GGRSSessionInfo, max_frames_behind: u32) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.set_max_frames_behind(max_frames_behind);
        return true;
    }
    return false;
}

fn set_catchup_speed(info: &mut GGRSSessionInfo, catchup_speed: u32) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.set_catchup_speed(catchup_speed);
        return true;
    }
    return false;
}

fn set_check_distance(info: &mut GGRSSessionInfo, check_distance: u32) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.set_check_distance(check_distance);
        return true;
    }
    return false;
}

fn set_local_port(info: &mut GGRSSessionInfo, local_port: u16) -> bool{
    if info.session_type != GGRSSessionType::NotSet{
        info.set_local_port(local_port);
        return true;
    }
    return false;
}