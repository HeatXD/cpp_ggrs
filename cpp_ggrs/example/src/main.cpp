#include "../../out/cpp_ggrs.h"
#include "../include/game.hpp"
#include "../include/raylib.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <string>


void DrawGameState(ex::game::Game &game);
void PrintEvent(GGRS::GGRSEvent &ev, int &frames_to_skip);
void HandleRequests(ex::game::Game &game, ex::game::SaveState &save,
                    rust::Vec<GGRS::GGRSFrameAction> requests);
std::uint32_t FetchLocalInput();

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
  // setup game
  ex::game::SaveState save;
  ex::game::Game game;
  // setup ggrs
  GGRS::GGRSSessionInfo info;
  GGRS::GGRSPlayer players[2];
  GGRS::setup_ggrs_info(info);
  GGRS::set_num_players(info, 2);
  GGRS::set_sparse_saving(info, true);
  GGRS::setup_p2p_session(info, local_port, 60, 1, 7);
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
  auto err = GGRS::GGRSErrorType::GGRSOk;
  std::cout << "Err: " << (uint16_t)err << std::endl;
  auto sess = GGRS::create_session(info, err);
  std::cout << "Err: " << (uint16_t)err << std::endl;
  // setup raylib
  InitWindow(600, 600, "GGRS C++ GAME");
  SetTargetFPS(60);
  // timesync
  int frames_to_skip = 0;
  //
  while (!WindowShouldClose()) {
    // poll other peers
    err = GGRS::poll_remote_clients(sess);
    std::cout << "Err: " << (uint16_t)err << std::endl;
    // handle events
    auto events = GGRS::get_events(sess, err);
    std::cout << "Err: " << (uint16_t)err << std::endl;
    for (int i = 0; i < events.size(); i++) {
      PrintEvent(events[i], frames_to_skip);
    }
    // keep the clients synced
    if( frames_to_skip > 0) {
      frames_to_skip -= 1;
      std::cout << "Frame: " << game.frame << " skipped: WaitRecommendation" << std::endl;
    }
    else if (GGRS::get_current_state(sess, err) == GGRS::GGRSSessionState::Running) {
      std::cout << "Err: " << (uint16_t)err << std::endl;
      // add local input
      err = GGRS::add_local_input(sess, local_player, FetchLocalInput());
      std::cout << "Err: " << (uint16_t)err << std::endl;
      // advance frame
      auto result = GGRS::advance_frame(sess, err);
      std::cout << "Err: " << (uint16_t)err << std::endl;
      // handle update
      if (!result.skip_frame) {
        HandleRequests(game, save, result.actions);
      } else {
        std::cout << "Skipping Frame: " << game.frame << std::endl;
      }
    }
    // render game
    DrawGameState(game);
  }
  GGRS::clean_session(sess);
  return 0;
}

void DrawGameState(ex::game::Game &game) {
  BeginDrawing();
  ClearBackground(BLACK);
  int idx = 0;
  for (auto player : game.players) {
    DrawRectangle(player.X, player.Y, 100, 100, idx == 0 ? RED : BLUE);
    idx++;
  }
  std::stringstream ss;
  ss << "P(0) X:" << game.players[0].X << " Y:" << game.players[0].Y
     << std::endl
     << "P(1) X:" << game.players[1].X << " Y:" << game.players[1].Y
     << std::endl;
  DrawText(ss.str().c_str(), 10, 500, 20, WHITE);
  DrawFPS(20, 20);
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

void PrintEvent(GGRS::GGRSEvent &ev, int &frames_to_skip) {
  
  std::cout << "Event: " << EventTypeToString(ev.event_type)
            << "\nAddress: " << ev.event_info.addr
            << "\nCount: " << ev.event_info.count
            << "\nTotal: " << ev.event_info.total
            << "\nTimeout: " << ev.event_info.disconnect_timeout
            << "\nFrames: " << ev.event_info.skip_frames << std::endl;
  
  if (ev.event_type == GGRS::GGRSEventType::WaitRecommendation) 
    frames_to_skip += ev.event_info.skip_frames;
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

void HandleRequests(ex::game::Game &game, ex::game::SaveState &save,
                    rust::Vec<GGRS::GGRSFrameAction> requests) {
  for (auto action : requests) {
    switch (action.action_type) {
    case GGRS::GGRSFrameActionType::SaveGameState:
      ex::game::SaveGame(game,  save);
      break;
    case GGRS::GGRSFrameActionType::LoadGameState:
      ex::game::LoadGame(game, save);
      break;
    case GGRS::GGRSFrameActionType::AdvanceFrame:
      ex::game::AdvanceGame(game, action.action_info.inputs);
      break;
    }
  }
}