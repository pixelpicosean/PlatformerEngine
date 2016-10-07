//
//  Character.hpp
//  PlatformerEngine
//
//  Created by Sean on 10/2/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#ifndef Character_hpp
#define Character_hpp

#include "../core/MovingEntity.hpp"

class Character : public MovingEntity {
  // Enums
  public:
    enum class State {
      Stand,
      Walk,
      Jump,
      GrabLedge,
    };

    enum class Input {
      Left,
      Right,
      Down,
      Jump,
    };

  // Configs
  public:
    float jumpSpeed = 410.0f;
    float walkSpeed = 160.0f;
    float minJumpSpeed = 200.0f;

  // Components & states
  public:
    sf::RectangleShape sprite;

    State currentState = State::Stand;

  // Methods
  public:
    Character(float x, float y, float w, float h, Map& map);

    void Update(sf::Time dt);
    void Draw(sf::RenderTarget& frame);

  private:
    sf::Vector2i ledgeTile;
};

#endif /* Character_hpp */
