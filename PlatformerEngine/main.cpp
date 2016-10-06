
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"

// Engine headers
#include "game/Character.hpp"
#include "core/Map.hpp"

int main(int, char const**) {
  // Setup window
  sf::RenderWindow window(sf::VideoMode(320, 320), "PlatformerEngine");

  // Initialize
  Map map(8, 8, {
    { 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },

    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,2,2,2,2, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },

    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,1,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,1, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },

    { 1,1,1,1,1, 1,1,0,0,0, 2,2,2,2,2, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1 },
    { 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1 },
  });

  Character player(100, 100, 30, 40, map);

  // Game loop
  sf::Clock clock;
  while (window.isOpen()) {
    // Handle events
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        window.close();
      }
    }


    // Update
    sf::Time dt = clock.restart();

    // - Update player
    player.Update(dt);


    // Draw
    window.clear();

    // - Draw map
    map.Draw(window);
    // - Draw player
    player.Draw(window);

    // Flush and render to screen
    window.display();
  }

  return EXIT_SUCCESS;
}
