
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

int main(int, char const**) {
  // Setup window
  sf::RenderWindow window(sf::VideoMode(640, 400), "PlatformerEngine");

  sf::Image icon;
  if (!icon.loadFromFile(resourcePath() + "icon.png")) {
    return EXIT_FAILURE;
  }
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

  // Initialize
//  sf::Texture texture;
//  if (!texture.loadFromFile(resourcePath() + "cute_image.jpg")) {
//    return EXIT_FAILURE;
//  }
//  sf::Sprite sprite(texture);
//
//  sf::Font font;
//  if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
//    return EXIT_FAILURE;
//  }
//  sf::Text text("Hello SFML", font, 50);
//  text.setFillColor(sf::Color::Black);

  Character player(320, 60, 12, 40);

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

    // - Draw player
    window.draw(player.sprite);

    // Flush and render to screen
    window.display();
  }

  return EXIT_SUCCESS;
}
