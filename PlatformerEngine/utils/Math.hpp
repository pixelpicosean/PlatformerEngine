//
//  Math.hpp
//  PlatformerEngine
//
//  Created by Sean on 10/4/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#ifndef Math_hpp
#define Math_hpp

#include <SFML/Graphics.hpp>
#include <cmath>

sf::Vector2f LerpVector2f(const sf::Vector2f& a, const sf::Vector2f& b, float fct) {
  return {
    a.x + (b.x - a.x) * fct,
    a.y + (b.y - a.y) * fct,
  };
}

sf::Vector2f& RoundVector2f(sf::Vector2f& v) {
  v.x = std::round(v.x);
  v.y = std::round(v.y);
  return v;
}

#endif /* Math_hpp */
