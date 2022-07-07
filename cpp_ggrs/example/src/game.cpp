#include "../include/game.hpp"
#include <iostream>

void MovePlayer(ex::game::Game &game, std::uint32_t input, int player) {
  int speed = 2;
  if (ex::game::IsBitSet(input, 0))
    game.players[player].Y -= speed;
  if (ex::game::IsBitSet(input, 1))
    game.players[player].Y += speed;
  if (ex::game::IsBitSet(input, 2))
    game.players[player].X -= speed;
  if (ex::game::IsBitSet(input, 3))
    game.players[player].X += speed;
}

void ex::game::AdvanceGame(Game &game, rust::Vec<GGRS::GGRSInput> inputs) {
  game.frame++;
  int idx = 0;
  for (auto input : inputs) {
    MovePlayer(game, input.input, idx);
    idx++;
  }
}

void ex::game::SaveGame(Game game, SaveState &save) {
  std::cout << "Saved Frame: " << game.frame << std::endl;
  save.save = game;
}

void ex::game::LoadGame(Game &game, SaveState &save) {
  std::cout << "Loaded Frame: " << save.save.frame << std::endl;
  auto tmp = save.save;
  game = tmp;
}

bool ex::game::IsBitSet(std::uint32_t bf, int pos) {
  return ((bf >> pos) & 1) ? true : false;
}