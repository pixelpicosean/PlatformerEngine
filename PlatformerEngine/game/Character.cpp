//
//  Character.cpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#include "Character.hpp"
#include <cmath>

const float GRAVITY = 980.0f;
const float MAX_FALL_SPEED = 300.0f;

const float GRAB_LEDGE_START_Y = 0.0f;
const float GRAB_LEDGE_END_Y = 2.0f;
const float GRAB_LEDGE_TILE_OFFSET_Y = -4.0f;

Character::Character(float x, float y, float w, float h, Map& map): MovingEntity(x, y, w, h, map) {
  this->sprite.setSize(sf::Vector2f(w, h));
  this->sprite.setOrigin(w / 2, h / 2);
}

void Character::Update(sf::Time dt) {
  // Update states
  switch (this->currentState) {
    case State::Stand:
      this->sprite.setFillColor(sf::Color(255, 255, 255));

      this->speed.x = this->speed.y = 0.0f;

      if (!this->isOnGround) {
        this->currentState = State::Jump;
        break;
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) != sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        this->currentState = State::Walk;
        break;
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        this->speed.y = -this->jumpSpeed;
        this->currentState = State::Jump;
        break;
      }

      // Drop from one-way platform
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (this->onOneWayPlatform) {
          this->position.y += ONE_WAY_PLATFORM_THRESHOD;
        }
      }

      break;

    case State::Walk:
      this->sprite.setFillColor(sf::Color(125, 255, 125));

      // Horizontal movement
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        this->currentState = State::Stand;
        this->speed.x = this->speed.y = 0.0f;
        break;
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (this->pushesLeftWall) {
          this->speed.x = 0.0f;
        }
        else {
          this->speed.x = -this->walkSpeed;
        }
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (this->pushesRightWall) {
          this->speed.x = 0.0f;
        }
        else {
          this->speed.x = this->walkSpeed;
        }
      }

      // Jump
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        this->speed.y = -this->jumpSpeed;
        this->currentState = State::Jump;
        break;
      }
      else if (!this->isOnGround) {
        this->currentState = State::Jump;
        break;
      }

      // Drop from one-way platform
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (this->onOneWayPlatform) {
          this->position.y += ONE_WAY_PLATFORM_THRESHOD;
        }
      }

      break;

    case State::Jump:
      this->sprite.setFillColor(sf::Color(150, 50, 250));

      // Vertical movement
      // - Gravity
      this->speed.y += GRAVITY * dt.asSeconds();
      // - Fall
      if (this->speed.y > 0.0f) {
        this->speed.y = std::max(this->speed.y, MAX_FALL_SPEED);
      }
      // - Jump
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->speed.y < 0.0f) {
        this->speed.y = -std::min(std::abs(this->speed.y), this->minJumpSpeed);
      }

      // Horizontal movement
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        this->speed.x = 0.0f;
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (this->pushesLeftWall) {
          this->speed.x = 0.0f;
        }
        else {
          this->speed.x = -this->walkSpeed;
        }
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (this->pushesRightWall) {
          this->speed.x = 0.0f;
        }
        else {
          this->speed.x = this->walkSpeed;
        }
      }

      if (this->isOnGround) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          this->currentState = State::Stand;
          this->speed.x = this->speed.y = 0.0f;
        }
        else {
          this->currentState = State::Walk;
          this->speed.y = 0.0f;
        }
      }
      // Ledge grab
      // - falling && not at ceiling && collide with wall and move towards it
      else if (this->speed.y >= 0.0f && !this->isAtCeiling && ((this->pushesRightWall && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (this->pushesLeftWall && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)))) {
        Vector2f aabbCornerOffset;

        // Calculate corner offset based moving direction
        if (this->pushesRightWall && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          aabbCornerOffset = { this->aabb.halfSize.x, -this->aabb.halfSize.y };
        }
        else {
          aabbCornerOffset = { -this->aabb.halfSize.x - 1.0f, -this->aabb.halfSize.y };
        }

        int tileX, topY, bottomY;
        tileX = this->map.GetMapTileXAtPoint(this->aabb.center.x + aabbCornerOffset.x);

        // Need to look for a ledge from last frame since it already touches a wall and x won't change
        if ((this->pushesLeftWall && this->pushedLeftWall) || (this->pushesRightWall && this->pushedRightWall)) {
          topY = this->map.GetMapTileYAtPoint(this->lastPosition.y + this->aabbOffset.y + aabbCornerOffset.y + GRAB_LEDGE_START_Y);
          bottomY = this->map.GetMapTileYAtPoint(this->aabb.center.y + aabbCornerOffset.y + GRAB_LEDGE_END_Y);
        }
        else {
          topY = this->map.GetMapTileYAtPoint(this->aabb.center.y + aabbCornerOffset.y + GRAB_LEDGE_START_Y);
          bottomY = this->map.GetMapTileYAtPoint(this->aabb.center.y + aabbCornerOffset.y + GRAB_LEDGE_END_Y);
        }

        for (int y = topY; y <= bottomY; ++y) {
          // The tile is empty and the tile below it is solid
          if (!this->map.IsObstacle(tileX, y) && this->map.IsObstacle(tileX, y + 1)) {
            auto tileCorner = this->map.GetMapTilePosition(tileX, y + 1);
            tileCorner.x -= (aabbCornerOffset.x > 0.0f ? 1.0f : -1.0f) * (this->map.tilesize / 2.0f);
            tileCorner.y -= this->map.tilesize / 2.0f;

            if (y > bottomY || ((this->aabb.center.y + aabbCornerOffset.y) - tileCorner.y <= GRAB_LEDGE_END_Y && tileCorner.y - (this->aabb.center.y + aabbCornerOffset.y) >= GRAB_LEDGE_START_Y)) {
              // Ledge tile is finally found
              this->ledgeTile.x = tileX;
              this->ledgeTile.y = y + 1;

              // Align to the tile
              this->position.y = tileCorner.y - aabbCornerOffset.y - this->aabbOffset.y - GRAB_LEDGE_START_Y + GRAB_LEDGE_TILE_OFFSET_Y;

              // Zero the speed
              this->speed.x = this->speed.y = 0.0f;

              // Change state
              this->currentState = State::GrabLedge;
              break;
            }
          }
        }
      }

      break;

    case State::GrabLedge:
      bool ledgeOnLeft = this->ledgeTile.x * this->map.tilesize < this->position.x;
      bool ledgeOnRight = !ledgeOnLeft;

      // Drop by either press the opposite direction or the down key
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || (ledgeOnLeft && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (ledgeOnRight && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {
        this->currentState = State::Jump;
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        this->speed.y = -this->jumpSpeed;
        this->currentState = State::Jump;
      }

      break;
  }

  // Update movement
  MovingEntity::Update(dt);

  // Update components
  this->sprite.setPosition(this->position);
}

void Character::Draw(sf::RenderTarget& frame) {
  frame.draw(this->sprite);
}
