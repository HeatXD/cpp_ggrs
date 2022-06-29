#include "../out/cpp_ggrs.h"
#include <iostream>
#include <stdint.h>
#include <string>

int main() {
  GGRS::GGRSSessionInfo info;
  GGRS::GGRSPlayer players[2] = {
      {
          .player_handle = 0,
          .player_type = GGRS::GGRSPlayerType::Local,
          .socket_addr = "local",
      },
      {
          .player_handle = 1,
          .player_type = GGRS::GGRSPlayerType::Remote,
          .socket_addr = "127.0.0.1:3211",
      },
  };
  std::cout << GGRS::test_lib(20) << std::endl;
  std::cout << info.local_port << std::endl;
  GGRS::setup_ggrs_info(info);
  GGRS::set_num_players(info, 2);
  GGRS::set_sparse_saving(info, true);
  GGRS::setup_p2p_session(info, 4545, 60, 2, 7);
  std::cout << info.local_port << std::endl;
  for (int i = 0; i < 2; i++) {
    std::string res = GGRS::add_player(info, players[i]) == true ? "Ok" : "Err";
    std::cout << res << std::endl;
  }
  return 0;
}