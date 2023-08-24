#include "chunk.hpp"
#include "constants.hpp"
#include "raylib.h"

Chunk::Chunk(int rows, int cols) {
  this->grid = std::vector<std::vector<Cell>>(rows, std::vector<Cell>(cols));
  this->render = LoadRenderTexture(cols * CELL_SIZE, rows * CELL_SIZE);
}
