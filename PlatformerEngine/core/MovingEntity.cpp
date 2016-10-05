//
//  MovingEntity.cpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#include "MovingEntity.hpp"

#include "../utils/Math.hpp"

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

  // Map collision detection
  // - Ground
  float groundY = 0.0f;
  if (this->speed.y >= 0.0f && this->HasGround(this->lastPosition, this->position, this->speed, groundY, this->onOneWayPlatform)) {
    this->position.y = groundY - this->aabb.halfSize.y - this->aabbOffset.y;
    this->speed.y = 0.0f;
    this->isOnGround = true;
  }
  else {
    this->isOnGround = false;
  }

  // - Ceiling
  float ceilingY = 0.0f; static int c = 0;
  if (this->speed.y <= 0.0f && this->HasCeiling(this->lastPosition, this->position, ceilingY)) {
    //printf("[%d] Touch ceiling at Y: %.2f | Ceiling: %.2f\n", c++, this->position.y, ceilingY);
    //this->position.y = ceilingY + this->aabb.halfSize.y - this->aabbOffset.y;
    this->speed.y = 0.0f;
    this->isAtCeiling = true;
  }
  else {
    this->isAtCeiling = false;
  }

  // Update AABB
  this->aabb.center = this->position + this->aabbOffset;
}

bool MovingEntity::HasGround(const sf::Vector2f &oldPos, const sf::Vector2f &position, const sf::Vector2f &speed, float &groundY, bool& onOneWayPlatform) {
  auto lastCenter = oldPos + this->aabbOffset;
  auto center = position + this->aabbOffset;

  auto lastBottomLeft = Vector2f(lastCenter.x - this->aabb.halfSize.x, lastCenter.y + this->aabb.halfSize.y) - UP - RIGHT;
  auto bottomLeft = Vector2f(center.x - this->aabb.halfSize.x, center.y + this->aabb.halfSize.y) - UP - RIGHT;
  auto bottomRight = Vector2f(bottomLeft.x + this->aabb.halfSize.x * 2.0f + 2.0f, bottomLeft.y);

  int tileX, tileY;
  for (auto checkedTile = bottomLeft;; checkedTile.x += this->map.tilesize) {
    checkedTile.x = std::min(checkedTile.x, bottomRight.x);

    tileX = this->map.GetMapTileXAtPoint(checkedTile.x);
    tileY = this->map.GetMapTileYAtPoint(checkedTile.y);

    groundY = tileY * this->map.tilesize - this->map.tilesize / 2.0f + this->map.position.y;

    if (this->map.IsObstacle(tileX, tileY)) {
      onOneWayPlatform = false;
      return true;
    }
    else if (this->map.IsOneWayPlatform(tileX, tileY) && (std::abs(checkedTile.y - groundY) <= ONE_WAY_PLATFORM_THRESHOD + (position.y - oldPos.y))) {
      onOneWayPlatform = true;
      return true;
    }

    if (checkedTile.x >= bottomRight.x) {
      break;
    }
  }
  
  return false;
}

bool MovingEntity::HasCeiling(const sf::Vector2f &oldPos, const sf::Vector2f &position, float &ceilingY) {
  auto center = position + this->aabbOffset;
  auto lastCenter = oldPos + this->aabbOffset;

  ceilingY = 0.0f;

  auto lastTopRight = lastCenter - this->aabb.halfSize - UP + RIGHT;

  auto currTopRight = center - this->aabb.halfSize - UP + RIGHT;
  auto currTopLeft = Vector2f(currTopRight.x - this->aabb.halfSize.x * 2.0f - 2.0f, currTopRight.y);

  int endY = this->map.GetMapTileYAtPoint(currTopRight.y);
  int begY = std::min(this->map.GetMapTileYAtPoint(lastTopRight.y) + 1, endY);
  int dist = std::max(std::abs(endY - begY), 1);

  int tileX;
  for (int tileY = begY; tileY >= endY; --tileY) {
    auto topRight = LerpVector2f(currTopRight, lastTopRight, std::abs(endY - tileY) / (float)dist);
    auto topLeft = Vector2f(topRight.x - this->aabb.halfSize.x * 2.0f - 2.0f, topRight.y);

    for (auto checkedTile = topLeft;; checkedTile.x += this->map.tilesize) {
      checkedTile.x = std::min(checkedTile.x, topRight.x);

      tileX = this->map.GetMapTileXAtPoint(checkedTile.x);

      if (this->map.IsObstacle(tileX, tileY)) {
        ceilingY = tileY * this->map.tilesize + this->map.tilesize / 2.0f + this->map.position.y;
        return true;
      }

      if (checkedTile.x >= topRight.x) {
        break;
      }
    }
  }

  return false;
}
