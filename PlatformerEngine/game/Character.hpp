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

    struct Input {
      static const int Left = 0;
      static const int Right = 1;
      static const int Down = 2;
      static const int Jump = 3;
    };

  // Configs
  public:
    float jumpSpeed = 410.0f;
    float walkSpeed = 160.0f;
    float minJumpSpeed = 200.0f;

    sf::Keyboard::Key leftKey = sf::Keyboard::A;
    sf::Keyboard::Key rightKey = sf::Keyboard::D;
    sf::Keyboard::Key downKey = sf::Keyboard::S;
    sf::Keyboard::Key jumpKey = sf::Keyboard::W;

  // Components & states
  public:
    sf::RectangleShape sprite;

    State currentState = State::Stand;

  // Methods
  public:
    Character(float x, float y, float w, float h, Map& map);

    void Update(sf::Time dt);
    void Draw(sf::RenderTarget& frame);

  protected:
    bool inputs[4] = { false, false, false, false };
    bool lastInputs[4] = { false, false, false, false };

  private:
    sf::Vector2i ledgeTile;
};

#endif /* Character_hpp */
