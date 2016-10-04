//
//  AABB.cpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#include "AABB.hpp"
#include <cmath>

AABB::AABB(Vector2f center, Vector2f halfSize):
  center(center), halfSize(halfSize)
{}

bool AABB::Overlap(const AABB& other) {
  if (std::abs(this->center.x - other.center.x) > (this->halfSize.x + other.halfSize.x)) return false;
  if (std::abs(this->center.y - other.center.y) > (this->halfSize.y + other.halfSize.y)) return false;
  return true;
}
