#ifndef _EXAMPLE_GAME_H
#define _EXAMPLE_GAME_H

namespace ex {
namespace game {

struct Player {
  int X, Y;
};

struct Game {
  int frame;
  Player players[2];
};

void AdvanceGame(Game &game);
} // namespace game
} // namespace ex

#endif