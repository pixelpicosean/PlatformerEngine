//
//  Map.cpp
//  PlatformerEngine
//
//  Created by Sean on 10/3/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#include "Map.hpp"

Map::Map(const std::vector<std::vector<TileType> >& tiles):
  tiles(tiles),
  width(tiles[0].size()), height(tiles.size())
{}

Vector2i Map::GetMapTileAtPoint(float x, float y) {
  return {
    (int)((x - this->position.x + this->tilesize / 2.0f) / this->tilesize),
    (int)((y - this->position.y + this->tilesize / 2.0f) / this->tilesize),
  };
}

int Map::GetMapTileXAtPoint(float x) {
  return (int)((x - this->position.x + this->tilesize / 2.0f) / this->tilesize);
}

int Map::GetMapTileYAtPoint(float y) {
  return (int)((y - this->position.y + this->tilesize / 2.0f) / this->tilesize);
}

Vector2f Map::GetMapTilePosition(int x, int y) {
  return {
    x * this->tilesize + this->position.x,
    y * this->tilesize + this->position.y,
  };
}

TileType Map::GetTile(int x, int y) {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
    return BLOCK;
  }

  return this->tiles[y][x];
}

bool Map::IsObstacle(int x, int y) {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
    return true;
  }

  return tiles[y][x] == BLOCK;
}

bool Map::IsGround(int x, int y) {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height) {
    return false;
  }

  return (tiles[y][x] == BLOCK) || (tiles[y][x] == ONE_WAY);
}

bool Map::IsOneWayPlatform(int x, int y)
{
  if (x < 0 || x >= this->width || y < 0 || y >= this->height)
    return false;

  return (this->tiles[y][x] == ONE_WAY);
}

bool Map::IsEmpty(int x, int y) {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height)
    return false;

  return (this->tiles[y][x] == EMPTY);
}
