#include "cell.hpp"
#include "raylib.h"

Color Cell::getColor() {
  switch (this->type) {
  case CellType::AIR:
    return Color{};
  case CellType::SAND:
    switch (this->variation % 2) {
    case SandVariation::SAND1:
      return YELLOW;
    case SandVariation::SAND2:
      return ColorBrightness(YELLOW, -0.04);
    }
  case CellType::STONE:
    return GRAY;
  case CellType::WATER:
    switch (this->variation % 2) {
    case WaterVariation::SURFACE:
      return SKYBLUE;
    case WaterVariation::BOTTOM:
      return BLUE;
    }
  default:
    return BLACK;
  }
}

CellPhysicalState Cell::getPhysicalState() {
  switch (this->type) {
  case CellType::AIR:
    return CellPhysicalState::GAS;

  case CellType::SAND:
  case CellType::STONE:
    return CellPhysicalState::SOLID;

  case CellType::WATER:
    return CellPhysicalState::LIQUID;

  default:
    return CellPhysicalState::SOLID;
  }
}
