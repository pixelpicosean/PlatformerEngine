//
//  MovingEntity.hpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#ifndef MovingEntity_hpp
#define MovingEntity_hpp

#include <SFML/Graphics.hpp>

#include "../physics/AABB.hpp"

using namespace sf;

class MovingEntity {
  public:
    Vector2f lastPosition;
    Vector2f position;

    Vector2f lastSpeed;
    Vector2f speed;

    Vector2f scale = { 1.0f, 1.0f };

    // AABB
    AABB aabb;
    Vector2f aabbOffset;

    // Touching info
    bool pushedRightWall, pushesRightWall;
    bool pushedLeftWall, pushesLeftWall;
    bool wasOnGround, isOnGround;
    bool wasAtCeiling, isAtCeiling;

  public:
    MovingEntity(float x, float y, float w, float h);

    void Update(Time dt);
};

#endif /* MovingEntity_hpp */
