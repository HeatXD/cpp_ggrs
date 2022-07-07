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
void HandleRequests(rust::Vec<GGRS::GGRSFrameAction> requests);
std::uint32_t FetchLocalInput();
bool IsBitSet(std::uint32_t bf, int pos);

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
  std::cout << local_player << " " << local_port << " " << remote_addr << std::endl;
  // setup ggrs
  GGRS::GGRSSessionInfo info;
  GGRS::GGRSPlayer players[2];
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
    // poll other peers
    GGRS::poll_remote_clients(sess);
    // handle events
    auto events = GGRS::get_events(sess);
    for (int i = 0; i < events.size(); i++) {
      PrintEvent(events[i]);
    }
    int input = FetchLocalInput();
    // if (GGRS::get_current_state(sess) == GGRS::GGRSSessionState::Running)
    // {
    //   // add local input
    //   GGRS::add_local_input(sess,local_player, FetchLocalInput());
    //   // advance frame
    //   auto result = GGRS::advance_frame(sess);
    //   // handle update
    //   if (!result.skip_frame) {
    //     HandleRequests(result.actions);
    //   }
    // }
    // render game
    DrawGameState();
  }
  return 0;
}

void DrawGameState() {
  BeginDrawing();
  ClearBackground(BLACK);
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
    return "Invalid Event Type";
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

std::uint32_t FetchLocalInput() {
  std::uint32_t input = 0;
  
  if (IsKeyDown(KEY_W))
    input |= (1 << 0);
  if (IsKeyDown(KEY_S))
    input |= (1 << 1);
  if (IsKeyDown(KEY_A))
    input |= (1 << 2);
  if (IsKeyDown(KEY_D))
    input |= (1 << 3);
  
  return input;
}

bool IsBitSet(std::uint32_t bf, int pos) {
  return ((bf >> pos) & 1) ? true : false;
}