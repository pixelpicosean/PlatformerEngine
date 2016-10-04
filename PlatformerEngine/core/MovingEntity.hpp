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
#include "Map.hpp"

using namespace sf;

/* Won't check collision if character is 2 pixels below the top surface */
const float ONE_WAY_PLATFORM_THRESHOD = 2.0f;

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

    bool onOneWayPlatform = false;

    // Components
    Map& map;

  public:
    MovingEntity(float x, float y, float w, float h, Map& map);

    void Update(Time dt);

    bool HasGround(const sf::Vector2f& oldPos, const sf::Vector2f& position, const sf::Vector2f& speed, float& groundY, bool& onOneWayPlatform);
};

#endif /* MovingEntity_hpp */
