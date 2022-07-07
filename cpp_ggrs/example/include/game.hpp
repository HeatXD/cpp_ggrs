#ifndef _EXAMPLE_GAME_H
#define _EXAMPLE_GAME_H

#include "../../out/cpp_ggrs.h"

namespace ex {
namespace game {

struct Player {
  int X, Y;
};

struct Game {
  int frame;
  Player players[2];
};

struct SaveState {
  Game save;
};

void AdvanceGame(Game &game, rust::Vec<GGRS::GGRSInput> inputs);
void SaveGame(Game game, SaveState &save);
void LoadGame(Game &game, SaveState &save);

bool IsBitSet(std::uint32_t bf, int pos);

} // namespace game
} // namespace ex

#endif