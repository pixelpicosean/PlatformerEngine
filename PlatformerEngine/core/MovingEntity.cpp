//
//  MovingEntity.cpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#include "MovingEntity.hpp"

MovingEntity::MovingEntity(float x, float y, float w, float h):
  lastPosition(x, y), position(x, y),
  aabb({ x, y }, { w / 2, h / 2 }),
  aabbOffset({ 0.0f, h / 2 })
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

  // Ground detection
  if (this->position.y >= 300) {
    this->position.y = 300;
    this->isOnGround = true;
  }
  else {
    this->isOnGround = false;
  }

  // Update AABB
  this->aabb.center = this->position + this->aabbOffset;
}
