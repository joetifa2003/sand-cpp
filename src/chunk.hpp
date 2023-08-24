#pragma once

#include "./cell.hpp"
#include "raylib.h"
#include <vector>

class Chunk {
  std::vector<std::vector<Cell>> grid;
  RenderTexture2D render;
  bool dirty = true;

public:
  Chunk() = delete;
  Chunk(int rows, int cols);
};
