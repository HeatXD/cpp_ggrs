#ifndef _EXAMPLE_GAME_H
#define _EXAMPLE_GAME_H

#include "../../out/cpp_ggrs.h"
#include <stdint.h>

namespace ex {
namespace game {

struct Player {
  int X, Y;
};

struct Game {
  int frame;
  Player players[2];
};

void AdvanceGame(Game &game, rust::Vec<uint32_t> inputs);
} // namespace game
} // namespace ex

#endif