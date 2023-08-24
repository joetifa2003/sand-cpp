#pragma once

#include "cell.hpp"
#include "constants.hpp"
#include "raylib.h"
#include <vector>

class SandGame {
  Camera2D camera{.zoom = 1.0f};
  RenderTexture2D render;

  std::vector<std::vector<Cell>> grid;
  Cell selectedCell{.type = CellType::SAND};
  int mouseCellRow = 0;
  int mouseCellCol = 0;
  bool alternateHorizontal = false;
  int brushSize = 2;

public:
  SandGame();
  ~SandGame();

  void input();
  void update();
  void draw();
  bool isCell(int row, int col, CellType t);

private:
  void drawCell(int row, int col, Color color);
  void drawGrid();
  void swapCells(int row1, int col1, int row2, int col2);
  void updateGrid();
  void drawUI();
  bool canMove(int row, int col);
};
