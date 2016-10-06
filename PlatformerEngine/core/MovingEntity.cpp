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
  float groundY = 0.0f, ceilingY = 0.0f, rightWallX = 0.0f, leftWallX = 0.0f;
  // - Ground
  if (this->speed.y >= 0.0f && this->HasGround(this->lastPosition, this->position, this->speed, groundY, this->onOneWayPlatform)) {
    this->position.y = groundY - this->aabb.halfSize.y - this->aabbOffset.y;
    this->speed.y = 0.0f;
    this->isOnGround = true;
  }
  else {
    this->isOnGround = false;
  }
  // - Ceiling
  if (this->speed.y <= 0.0f && this->HasCeiling(this->lastPosition, this->position, ceilingY)) {
    this->position.y = ceilingY + this->aabb.halfSize.y - this->aabbOffset.y + 1.0f;
    this->speed.y = 0.0f;
    this->isAtCeiling = true;
  }
  else {
    this->isAtCeiling = false;
  }
  // - Left wall
  if (this->speed.x <= 0.0f && this->CollideWithLeftWall(this->lastPosition, this->position, leftWallX)){
    if (this->lastPosition.x - this->aabb.halfSize.x + this->aabbOffset.x >= leftWallX) {
      this->position.x = leftWallX + this->aabb.halfSize.x - this->aabbOffset.x;
      this->pushesLeftWall = true;
    }
    this->speed.x = std::max(this->speed.x, 0.0f);
  }
  else {
    this->pushesLeftWall = false;
  }
  // - Right wall
  if (this->speed.x >= 0.0f && this->CollideWithRightWall(this->lastPosition, this->position, rightWallX)){
    if (this->lastPosition.x + this->aabb.halfSize.x + this->aabbOffset.x <= rightWallX) {
      this->position.x = rightWallX - this->aabb.halfSize.x - this->aabbOffset.x;
      this->pushesRightWall = true;
    }
    this->speed.x = std::min(this->speed.x, 0.0f);
  }
  else {
    this->pushesRightWall = false;
  }

  // Update AABB
  this->aabb.center = this->position + this->aabbOffset;
}

bool MovingEntity::HasGround(const sf::Vector2f &oldPos, const sf::Vector2f &position, const sf::Vector2f &speed, float &groundY, bool& onOneWayPlatform) {
  auto lastCenter = oldPos + this->aabbOffset;
  auto center = position + this->aabbOffset;

  auto lastBottomLeft = Vector2f(lastCenter.x - this->aabb.halfSize.x, lastCenter.y + this->aabb.halfSize.y) - UP + RIGHT;
  lastBottomLeft = RoundVector2f(lastBottomLeft);

  auto currBottomLeft = Vector2f(center.x - this->aabb.halfSize.x, center.y + this->aabb.halfSize.y) - UP + RIGHT;
  currBottomLeft = RoundVector2f(currBottomLeft);
  auto currBottomRight = Vector2f(currBottomLeft.x + this->aabb.halfSize.x * 2.0f - 2.0f, currBottomLeft.y);
  currBottomRight = RoundVector2f(currBottomRight);

  int endY = this->map.GetMapTileYAtPoint(currBottomLeft.y);
  int begY = std::max(this->map.GetMapTileYAtPoint(lastBottomLeft.y) + 1, endY);
  int dist = std::max(std::abs(endY - begY), 1);

  int tileX;
  for (int tileY = begY; tileY >= endY; --tileY) {
    auto bottomLeft = LerpVector2f(currBottomLeft, lastBottomLeft, std::abs(tileY - endY) / (float)dist);
    auto bottomRight = Vector2f(bottomLeft.x + this->aabb.halfSize.x * 2.0f - 2.0f, bottomLeft.y);

    for (auto checkedTile = currBottomLeft;; checkedTile.x += this->map.tilesize) {
      checkedTile.x = std::min(checkedTile.x, currBottomRight.x);

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

      if (checkedTile.x >= currBottomRight.x) {
        break;
      }
    }
  }

  return false;
}

bool MovingEntity::HasCeiling(const sf::Vector2f &oldPos, const sf::Vector2f &position, float &ceilingY) {
  auto center = position + this->aabbOffset;
  auto lastCenter = oldPos + this->aabbOffset;

  ceilingY = 0.0f;

  auto lastTopRight = Vector2f(lastCenter.x + this->aabb.halfSize.x, lastCenter.y - this->aabb.halfSize.y) + UP - RIGHT;
  lastTopRight = RoundVector2f(lastTopRight);

  auto currTopRight = Vector2f(center.x + this->aabb.halfSize.x, center.y - this->aabb.halfSize.y) + UP - RIGHT;
  currTopRight = RoundVector2f(currTopRight);
  auto currTopLeft = Vector2f(currTopRight.x - this->aabb.halfSize.x * 2.0f + 2.0f, currTopRight.y);
  currTopLeft = RoundVector2f(currTopLeft);

  int endY = this->map.GetMapTileYAtPoint(currTopRight.y);
  int begY = std::max(this->map.GetMapTileYAtPoint(lastTopRight.y) - 1, endY);
  int dist = std::max(std::abs(endY - begY), 1);

  int tileX;
  for (int tileY = begY; tileY >= endY; --tileY) {
    auto topRight = LerpVector2f(currTopRight, lastTopRight, std::abs(endY - tileY) / (float)dist);
    auto topLeft = Vector2f(topRight.x - this->aabb.halfSize.x * 2.0f + 2.0f, topRight.y);

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

bool MovingEntity::CollideWithLeftWall(const sf::Vector2f &oldPos, const sf::Vector2f &position, float &wallX) {
  auto center = position + this->aabbOffset;
  auto lastCenter = oldPos + this->aabbOffset;

  wallX = 0.0f;

  auto lastBottomLeft = Vector2f(lastCenter.x - this->aabb.halfSize.x, lastCenter.y + this->aabb.halfSize.y) - RIGHT + UP;
  lastBottomLeft = RoundVector2f(lastBottomLeft);

  auto currBottomLeft = Vector2f(center.x - this->aabb.halfSize.x, center.y + this->aabb.halfSize.y) - RIGHT + UP;
  currBottomLeft = RoundVector2f(currBottomLeft);
  auto currTopLeft = Vector2f(currBottomLeft.x, currBottomLeft.y - this->aabb.halfSize.y * 2.0f + 2.0f);
  currTopLeft = RoundVector2f(currTopLeft);

  int endX = this->map.GetMapTileXAtPoint(currBottomLeft.x);
  int begX = std::max(this->map.GetMapTileXAtPoint(lastBottomLeft.x) + 1, endX);
  int dist = std::max(std::abs(endX - begX), 1);

  int tileY;
  for (int tileX = begX; tileX >= endX; --tileX) {
    auto bottomLeft = LerpVector2f(currBottomLeft, lastBottomLeft, std::abs(endX - tileX) / (float)dist);
    auto topLeft = Vector2f(bottomLeft.x, bottomLeft.y - this->aabb.halfSize.x * 2.0f + 2.0f);

    for (auto checkedTile = bottomLeft;; checkedTile.y -= this->map.tilesize) {
      checkedTile.y = std::max(checkedTile.y, topLeft.y);

      tileY = this->map.GetMapTileYAtPoint(checkedTile.y);

      if (this->map.IsObstacle(tileX, tileY)) {
        wallX = tileX * this->map.tilesize + this->map.tilesize / 2.0f + this->map.position.x;
        return true;
      }

      if (checkedTile.y <= topLeft.y) {
        break;
      }
    }
  }
  
  return false;
}

bool MovingEntity::CollideWithRightWall(const sf::Vector2f &oldPos, const sf::Vector2f &position, float &wallX) {
  auto center = position + this->aabbOffset;
  auto lastCenter = oldPos + this->aabbOffset;

  wallX = 0.0f;

  auto lastBottomRight = lastCenter + this->aabb.halfSize + RIGHT + UP;
  lastBottomRight = RoundVector2f(lastBottomRight);

  auto currBottomRight = center + this->aabb.halfSize + RIGHT + UP;
  currBottomRight = RoundVector2f(currBottomRight);
  auto currTopRight = Vector2f(currBottomRight.x, currBottomRight.y - this->aabb.halfSize.y * 2.0f + 2.0f);
  currTopRight = RoundVector2f(currTopRight);

  int endX = this->map.GetMapTileXAtPoint(currBottomRight.x);
  int begX = std::min(this->map.GetMapTileXAtPoint(lastBottomRight.x) + 1, endX);
  int dist = std::max(std::abs(endX - begX), 1);

  int tileY;
  for (int tileX = begX; tileX <= endX; ++tileX) {
    auto bottomRight = LerpVector2f(currBottomRight, lastBottomRight, std::abs(endX - tileX) / (float)dist);
    auto topRight = Vector2f(bottomRight.x, bottomRight.y - this->aabb.halfSize.x * 2.0f + 2.0f);

    for (auto checkedTile = bottomRight;; checkedTile.y -= this->map.tilesize) {
      checkedTile.y = std::max(checkedTile.y, topRight.y);

      tileY = this->map.GetMapTileYAtPoint(checkedTile.y);

      if (this->map.IsObstacle(tileX, tileY)) {
        wallX = tileX * this->map.tilesize - this->map.tilesize / 2.0f + this->map.position.x;
        return true;
      }

      if (checkedTile.y <= topRight.y) {
        break;
      }
    }
  }
  
  return false;
}
