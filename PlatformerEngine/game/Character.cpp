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

Character::Character(float x, float y, float w, float h, Map& map): MovingEntity(x, y, w, h, map) {
  this->sprite.setSize(sf::Vector2f(w, h));
  this->sprite.setOrigin(w / 2, h / 2);
}

void Character::Update(sf::Time dt) {
  // Update states
  switch (this->currentState) {
    case STAND:
      this->sprite.setFillColor(sf::Color(255, 255, 255));

      this->speed.x = this->speed.y = 0.0f;

      if (!this->isOnGround) {
        this->currentState = JUMP;
        break;
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) != sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        this->currentState = WALK;
        break;
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        this->speed.y = -this->jumpSpeed;
        this->currentState = JUMP;
        break;
      }

      break;

    case WALK:
      this->sprite.setFillColor(sf::Color(125, 255, 125));

      // Horizontal movement
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        this->currentState = STAND;
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
        this->currentState = JUMP;
        break;
      }
      else if (!this->isOnGround) {
        this->currentState = JUMP;
        break;
      }
      break;

    case JUMP:
      this->sprite.setFillColor(sf::Color(150, 50, 250));

      // Vertical movement
      // - Gravity
      this->speed.y += GRAVITY * dt.asSeconds();
      // - Fall
      if (this->speed.y > 0.0f) {
        this->speed.y = std::fmax(this->speed.y, MAX_FALL_SPEED);
      }
      // - Jump
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->speed.y < 0.0f) {
        this->speed.y = -std::fmin(std::fabs(this->speed.y), this->minJumpSpeed);
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
          this->currentState = STAND;
          this->speed.x = this->speed.y = 0.0f;
        }
        else {
          this->currentState = WALK;
          this->speed.y = 0.0f;
        }
      }

      break;

    case GRAB_EDGE:
      break;

    default:
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
