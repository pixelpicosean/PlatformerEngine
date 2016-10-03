//
//  AABB.hpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#ifndef AABB_hpp
#define AABB_hpp

#include <SFML/Graphics.hpp>

using namespace sf;

struct AABB {
  Vector2f center;
  Vector2f halfSize;

  AABB(Vector2f center, Vector2f halfSize);

  bool Overlap(const AABB& other);
};

#endif /* AABB_hpp */
