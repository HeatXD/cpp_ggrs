#include "../../out/cpp_ggrs.h"
#include "../include/game.hpp"
#include "../include/raylib.h"
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>

void UpdateGameState();
void DrawGameState();
void PrintEvent(GGRS::GGRSEvent &ev);

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << argc << std::endl;
    return 0;
  }
  // get command line args
  // 1 > local player number
  std::uint32_t local_player = atoi(argv[1]);
  // 2 > local port
  std::uint16_t local_port = atoi(argv[2]);
  // 3 > remote address
  rust::string remote_addr = argv[3];
  std::cout << local_player << " " << local_port << " " << remote_addr
            << std::endl;
  // setup ggrs
  GGRS::GGRSSessionInfo info;
  GGRS::GGRSPlayer players[2];
  players[0].socket_addr = remote_addr;

  GGRS::setup_ggrs_info(info);
  GGRS::set_num_players(info, 2);
  GGRS::set_sparse_saving(info, false);
  GGRS::setup_p2p_session(info, local_port, 60, 2, 7);
  // add players
  for (int i = 0; i < 2; i++) {
    players[i].player_handle = i;
    if (i == local_player) {
      players[i].player_type = GGRS::GGRSPlayerType::Local;
      players[i].socket_addr = "localhost";
    } else {
      players[i].player_type = GGRS::GGRSPlayerType::Remote;
      players[i].socket_addr = remote_addr;
    }
    GGRS::add_player(info, players[i]);
  }
  // add spectators (optional)
  // creating the session pointer
  auto sess = GGRS::create_session(info);
  // setup raylib
  InitWindow(600, 600, "GGRS C++ GAME");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    GGRS::poll_remote_clients(sess);
    auto events = GGRS::get_events(sess);
    for (int i = 0; i < events.size(); i++) {
      PrintEvent(events[i]);
    }
    if (GGRS::get_current_state(sess) == GGRS::GGRSSessionState::Running) {
      DrawText("Running!", 150, 150, 25, GREEN);
    } else {
      DrawText("Syncing!", 150, 150, 25, RED);
    }
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

std::string EventTypeToString(GGRS::GGRSEventType type) {
  switch (type) {
  case GGRS::GGRSEventType::Empty:
    return "Empty";
  case GGRS::GGRSEventType::Synchronizing:
    return "Synchronizing";
  case GGRS::GGRSEventType::Synchronized:
    return "Synchronized";
  case GGRS::GGRSEventType::Disconnected:
    return "Disconnected";
  case GGRS::GGRSEventType::NetworkInterrupted:
    return "NetworkInterrupted";
  case GGRS::GGRSEventType::NetworkResumed:
    return "NetworkResumed";
  case GGRS::GGRSEventType::WaitRecommendation:
    return "WaitRecommendation";
  default:
    return "Invalid Session Type";
  }
}

void PrintEvent(GGRS::GGRSEvent &ev) {
  std::cout << "Event: " << EventTypeToString(ev.event_type)
            << "\nAddress: " << ev.event_info.addr
            << "\nCount: " << ev.event_info.count
            << "\nTotal: " << ev.event_info.total
            << "\nTimeout: " << ev.event_info.disconnect_timeout
            << "\nFrames: " << ev.event_info.skip_frames << std::endl;
}