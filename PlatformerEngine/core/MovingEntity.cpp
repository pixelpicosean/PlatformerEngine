//
//  MovingEntity.cpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#include "MovingEntity.hpp"

#include <cmath>

const sf::Vector2f UP =     {  0.0f, -1.0f };
const sf::Vector2f RIGHT =  { +1.0f,  0.0f };

MovingEntity::MovingEntity(float x, float y, float w, float h, Map& map):
  lastPosition(x, y), position(x, y),
  aabb({ x, y }, { w / 2, h / 2 }),
  aabbOffset({ 0.0f, 0.0f }),
  map(map)
{}

void MovingEntity::Update(Time dt) {
  // Save current touching info
  this->lastPosition = this->position;
  this->lastSpeed = this->speed;

  this->pushedLeftWall = this->pushesLeftWall;
  this->pushedRightWall = this->pushesRightWall;
  
  this->wasOnGround = this->isOnGround;
  this->wasAtCeiling = this->isAtCeiling;

  // Update velocity
  this->position += (this->speed * dt.asSeconds());

  //printf("(%.2f, %.2f)\n", this->position.x, this->position.y);

  // Ground detection
  float groundY = 0.0f;
  //auto hasGround = this->HasGround(this->lastPosition, this->position, this->speed, groundY);
  //printf("Ground: %s\n", hasGround ? "true" : "false");
  if (this->speed.y >= 0.0f && this->HasGround(this->lastPosition, this->position, this->speed, groundY)) {
    //printf("groundY: %.2f\n", groundY);
    this->position.y = groundY - this->aabb.halfSize.y + this->aabbOffset.y;
    this->speed.y = 0.0f;
    this->isOnGround = true;
  }
  else {
    this->isOnGround = false;
  }

  // Update AABB
  this->aabb.center = this->position + this->aabbOffset;
}

bool MovingEntity::HasGround(const sf::Vector2f &oldPos, const sf::Vector2f &position, const sf::Vector2f &speed, float &groundY) {
  auto lastCenter = oldPos + this->aabbOffset;
  auto center = position + this->aabbOffset;

  auto lastBottomLeft = Vector2f(lastCenter.x - this->aabb.halfSize.x, lastCenter.y + this->aabb.halfSize.y) - UP - RIGHT;
  auto bottomLeft = Vector2f(center.x - this->aabb.halfSize.x, center.y + this->aabb.halfSize.y) - UP - RIGHT;
  auto bottomRight = Vector2f(bottomLeft.x + this->aabb.halfSize.x * 2.0f + 2.0f, bottomLeft.y);

  //printf("(%.2f, %.2f)\n", position.x, position.y);
  //printf("(%.2f, %.2f)\n", bottomLeft.x, bottomLeft.y);

  int tileX, tileY;
  for (auto checkedTile = bottomLeft;; checkedTile.x += this->map.tilesize) {
    checkedTile.x = std::fmin(checkedTile.x, bottomRight.x);

    tileX = this->map.GetMapTileXAtPoint(checkedTile.x);
    tileY = this->map.GetMapTileYAtPoint(checkedTile.y);
    //printf("(%d, %d)\n", tileX, tileY);
    //printf("(%.2f, %.2f)\n", checkedTile.x, checkedTile.y);

    groundY = tileY * this->map.tilesize /*+ this->map.tilesize / 2.0f*/ + this->map.position.y;

    if (this->map.IsObstacle(tileX, tileY)) {
      return true;
    }

    if (checkedTile.x >= bottomRight.x) {
      break;
    }
  }
  
  return false;
}
