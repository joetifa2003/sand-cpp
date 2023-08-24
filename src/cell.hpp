#pragma once

#include "raylib.h"

enum class CellType { AIR, SAND, STONE, WATER };
enum class CellPhysicalState { SOLID, LIQUID, GAS };
enum WaterVariation { BOTTOM, SURFACE };
enum SandVariation { SAND1, SAND2 };

struct Cell {
  CellType type;
  bool dirty = true;
  int variation = 0;

  Color getColor();
  CellPhysicalState getPhysicalState();
};
