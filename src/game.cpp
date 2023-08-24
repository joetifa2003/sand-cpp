#include "game.hpp"
#include "cell.hpp"
#include "constants.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cstdlib>

SandGame::SandGame() {
  this->grid = std::vector<std::vector<Cell>>(ROWS, std::vector<Cell>(COLS));
  for (int y = ROWS - 2; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      this->grid[y][x] = {
          .type = CellType::STONE,
      };
    }
  }

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sand simulator");
  SetTargetFPS(60);
  this->render = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
}

SandGame::~SandGame() {
  UnloadRenderTexture(this->render);
  CloseWindow();
}

void SandGame::drawCell(int row, int col, Color color) {
  DrawRectangle(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}

void SandGame::drawGrid() {
  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      Cell cell = grid[y][x];
      if (!cell.dirty) {
        continue;
      }

      switch (cell.type) {
      case CellType::AIR:
        drawCell(y, x, RAYWHITE);
        break;

      default:
        drawCell(y, x, cell.getColor());
        break;
      }

      grid[y][x].dirty = false;
    }
  }
}

void SandGame::swapCells(int row1, int col1, int row2, int col2) {
  Cell temp = grid[row1][col1];
  grid[row1][col1] = grid[row2][col2];
  grid[row2][col2] = temp;

  grid[row1][col1].dirty = true;
  grid[row2][col2].dirty = true;
}

bool SandGame::canMove(int row, int col) {
  if (row >= 0 && col >= 0 && row < ROWS && col < COLS) {
    return grid[row][col].getPhysicalState() != CellPhysicalState::SOLID;
  }

  return false;
}

bool SandGame::isCell(int row, int col, CellType t) {
  if (row < 0 || col < 0 || row >= ROWS || col >= COLS) {
    return false;
  }

  return this->grid[row][col].type == t;
}

#define IS_EMPTY(row, col) isCell(row, col, CellType::AIR)

void SandGame::updateGrid() {
  this->alternateHorizontal = !this->alternateHorizontal;

  for (int y = ROWS - 1; y >= 0; y--) {
    for (int x = alternateHorizontal ? COLS - 1 : 0;
         alternateHorizontal ? x >= 0 : x < COLS;
         alternateHorizontal ? x-- : x++) {
      Cell cell = grid[y][x];

      switch (cell.type) {
      case CellType::SAND:
        if (canMove(y + 1, x)) {
          if (isCell(y + 1, x, CellType::WATER) && rand() % 2 == 0) {
            continue;
          }

          this->swapCells(y, x, y + 1, x);
        } else if (canMove(y + 1, x - 1)) {
          this->swapCells(y, x, y + 1, x - 1);
        } else if (canMove(y + 1, x + 1)) {
          this->swapCells(y, x, y + 1, x + 1);
        }
        break;

      case CellType::WATER:
        if (IS_EMPTY(y - 1, x)) {
          if (cell.variation == WaterVariation::BOTTOM) {
            grid[y][x].variation = WaterVariation::SURFACE;
            grid[y][x].dirty = true;
          }
        } else {
          if (cell.variation == WaterVariation::SURFACE) {
            grid[y][x].variation = WaterVariation::BOTTOM;
            grid[y][x].dirty = true;
          }
        }

        if (IS_EMPTY(y + 1, x)) {
          this->swapCells(y, x, y + 1, x);
        } else if (IS_EMPTY(y + 1, x - 1)) {
          this->swapCells(y, x, y + 1, x - 1);
        } else if (IS_EMPTY(y + 1, x + 1)) {
          this->swapCells(y, x, y + 1, x + 1);
        } else if (IS_EMPTY(y, x - 1) && IS_EMPTY(y, x + 1)) {
          if (rand() % 2 == 0) {
            this->swapCells(y, x, y, x - 1);
          } else {
            this->swapCells(y, x, y, x + 1);
          }
        } else if (IS_EMPTY(y, x - 1)) {
          this->swapCells(y, x, y, x - 1);
        } else if (IS_EMPTY(y, x + 1)) {
          this->swapCells(y, x, y, x + 1);
        }
        break;

      case CellType::STONE:
        break;
      case CellType::AIR:
        break;
      }
    }
  }
}

void SandGame::input() {
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && IsKeyDown(KEY_SPACE)) {
    camera.target = Vector2Subtract(camera.target, GetMouseDelta());
  } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
    for (int dy = -brushSize; dy <= brushSize; dy++) {
      for (int dx = -brushSize; dx <= brushSize; dx++) {
        int cellRow = this->mouseCellRow + dy;
        int cellCol = this->mouseCellCol + dx;

        if (cellRow < 0 || cellRow >= ROWS || cellCol < 0 || cellCol >= COLS) {
          continue;
        }

        if (this->selectedCell.type != CellType::WATER) {
          this->selectedCell.variation = rand();
        }
        this->grid[cellRow][cellCol] = this->selectedCell;
      }
    }
  }

  if (IsKeyPressed(KEY_ONE)) {
    this->selectedCell = {.type = CellType::SAND};
  } else if (IsKeyPressed(KEY_TWO)) {
    this->selectedCell = {.type = CellType::STONE};
  } else if (IsKeyPressed(KEY_THREE)) {
    this->selectedCell = {.type = CellType::WATER};
  } else if (IsKeyPressed(KEY_FOUR)) {
    this->selectedCell = {.type = CellType::AIR};
  }

  if (IsKeyPressed(KEY_W)) {
    this->brushSize++;
  } else if (IsKeyPressed(KEY_S)) {
    this->brushSize--;
  }

  if (IsKeyPressed(KEY_R)) {
    for (int y = 0; y < ROWS; y++) {
      for (int x = 0; x < COLS; x++) {
        this->grid[y][x] = {
            .type = CellType::AIR,
            .dirty = true,
        };
      }
    }
  }
}

void SandGame::draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  BeginTextureMode(this->render);
  this->drawGrid();
  EndTextureMode();

  BeginMode2D(this->camera);
  DrawTextureRec(this->render.texture,
                 Rectangle{.x = 0,
                           .y = 0,
                           .width = (float)this->render.texture.width,
                           .height = -(float)this->render.texture.height},
                 Vector2{.x = 0, .y = 0}, WHITE);

  for (int dy = -brushSize; dy <= brushSize; dy++) {
    for (int dx = -brushSize; dx <= brushSize; dx++) {
      int cellRow = this->mouseCellRow + dy;
      int cellCol = this->mouseCellCol + dx;

      if (cellRow < 0 || cellRow >= ROWS || cellCol < 0 || cellCol >= COLS) {
        continue;
      }

      this->drawCell(cellRow, cellCol,
                     ColorAlpha(this->selectedCell.getColor(), 0.5));
    }
  }
  EndMode2D();

  this->drawUI();

  EndDrawing();
}

void SandGame::drawUI() {
  DrawRectangle(10, 50, 50, 50, this->selectedCell.getColor());
  DrawFPS(10, 10);
}

void SandGame::update() {
  Vector2 mPos = GetMousePosition();
  mPos = GetScreenToWorld2D(mPos, this->camera);
  this->mouseCellRow = mPos.y / CELL_SIZE;
  this->mouseCellCol = mPos.x / CELL_SIZE;
  this->camera.zoom += GetMouseWheelMove() * 0.05;

  this->updateGrid();
}
