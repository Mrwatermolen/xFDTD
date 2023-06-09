#include "simulation/yee_cell.h"

#include <utility>

namespace xfdtd {
YeeCell::YeeCell(PointVector point, PointVector size,
                 int material_index, SpatialIndex x, SpatialIndex y,
                 SpatialIndex z)
    : _shape{std::make_unique<Cube>(std::move(point), std::move(size))},
      _material_index{material_index},
      _x{x},
      _y{y},
      _z{z} {}
}  // namespace xfdtd
