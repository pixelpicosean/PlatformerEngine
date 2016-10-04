//
//  Map.cpp
//  PlatformerEngine
//
//  Created by Sean on 10/3/16.
//  Copyright © 2016 Sean. All rights reserved.
//

#include "Map.hpp"

const sf::Color BLOCK_COLOR = { 200, 200, 200 };
const sf::Color EMPTY_COLOR = { 0, 0, 0 };
const sf::Color ONE_WAY_COLOR = { 20, 200, 20 };

Map::Map(float x, float y, const std::vector<std::vector<TileType> >& tiles):
  tiles(tiles),
  width(tiles[0].size()), height(tiles.size())
{
  for (int r = 0; r < this->height; r++) {
    this->tileShapes.push_back({});
    for (int q = 0; q < this->width; q++) {
      RectangleShape tile({ (float)this->tilesize, (float)this->tilesize });
      tile.setOrigin(this->tilesize / 2.0f, this->tilesize / 2.0f);
      if (tiles[r][q] == 0) {
        tile.setFillColor(EMPTY_COLOR);
      }
      else if (tiles[r][q] == 1) {
        tile.setFillColor(BLOCK_COLOR);
      }
      else if (tiles[r][q] == 2) {
        tile.setFillColor(ONE_WAY_COLOR);
      }

      this->tileShapes[r].push_back(tile);
    }
  }

  this->SetPosition(x, y);
}

void Map::SetPosition(float x, float y) {
  this->position.x = x;
  this->position.y = y;

  for (int r = 0; r < this->height; r++) {
    for (int q = 0; q < this->width; q++) {
      this->tileShapes[r][q].setPosition(q * this->tilesize + x, r * this->tilesize + y);
    }
  }
}

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

void Map::Draw(sf::RenderTarget &frame) {
  for (auto r: this->tileShapes) {
    for (auto tile: r) {
      frame.draw(tile);
    }
  }
}
