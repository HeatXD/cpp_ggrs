#include "../../out/cpp_ggrs.h"
#include "../include/raylib.h"
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>

void UpdateGameState();
void DrawGameState();

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << argc << std::endl;
    return 0;
  }
  // get command line args
  // 1 > local player number
  std::uint32_t local_player = 0;
  // 2 > local port
  std::uint16_t local_port = 0;
  // 3 > remote address
  rust::string remote_addr = "";
  // setup ggrs
  GGRS::GGRSSessionInfo info;
  GGRS::GGRSPlayer players[2];
  players[0].socket_addr = remote_addr;

  GGRS::setup_ggrs_info(info);
  GGRS::set_num_players(info, 2);
  GGRS::set_sparse_saving(info, false);
  GGRS::setup_p2p_session(info, 4545, 60, 2, 7);
  // add players
  for (int i = 0; i < 2; i++) {
    std::string res = GGRS::add_player(info, players[i]) == true ? "Ok" : "Err";
    std::cout << res << std::endl;
  }
  // add spectators (optional)
  // creating the session pointer
  auto sess = GGRS::create_session(info);
  // setup raylib
  InitWindow(600, 600, "GGRS C++ GAME");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    DrawGameState();
  }
  return 0;
}

void DrawGameState() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawFPS(50, 50);
  EndDrawing();
}