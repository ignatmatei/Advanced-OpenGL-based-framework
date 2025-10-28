#pragma once

#include <string>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace object2D {
    Mesh* CreateTerrain2(const std::string& name, unsigned int m, unsigned int n);
    Mesh* CreateBalloonBase(const std::string& name, glm::vec3 color);
}