//
//  Map.hpp
//  PlatformerEngine
//
//  Created by Sean on 10/3/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#ifndef Map_hpp
#define Map_hpp

#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;

typedef int TileType;
const TileType EMPTY =   0;
const TileType BLOCK =   1;
const TileType ONE_WAY = 2;

class Map {
  public:
    int width = 80;
    int height = 60;
    const int tilesize = 16;

    Vector2f position;

  public:
    Map(const std::vector<std::vector<TileType> >& tiles);

    // Tile <-> Position
    Vector2i GetMapTileAtPoint(float x, float y);
    int GetMapTileXAtPoint(float x);
    int GetMapTileYAtPoint(float y);

    Vector2f GetMapTilePosition(int x, int y);

    // Tile info
    TileType GetTile(int x, int y);

    bool IsObstacle(int x, int y);
    bool IsGround(int x, int y);
    bool IsOneWayPlatform(int x, int y);
    bool IsEmpty(int x, int y);

  private:
    std::vector<std::vector<TileType> > tiles;
};

#endif /* Map_hpp */
