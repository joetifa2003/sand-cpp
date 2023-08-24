#define RAYGUI_IMPLEMENTATION

#include "./vendor/raygui/raygui.h"
#include "game.hpp"
#include "raylib.h"
#include "raymath.h"
#include <ctime>
#include <iostream>

int main() {
  srand(time(nullptr));
  SandGame g;

  while (!WindowShouldClose()) {
    g.input();
    g.update();
    g.draw();
  }

  return 0;
}
