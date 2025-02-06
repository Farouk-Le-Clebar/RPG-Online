/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** World
*/

#include "World.hpp"
#include <noise/module/voronoi.h>

World::World(std::size_t width, std::size_t height, unsigned int seed, SDL_Renderer* renderer)
    : _width(width), _height(height), _seed(seed), _rng(seed), _perlin()
{
    _perlin.SetSeed(seed);
    _perlin.SetFrequency(0.1);
    _perlin.SetPersistence(0.5);
    _perlin.SetLacunarity(2.0);
    _perlin.SetOctaveCount(6);

    _noiseMap.resize(width);
    for (std::size_t i = 0; i < width; ++i) {
        _noiseMap[i].resize(height);
    }

    _tiles.resize(_width, std::vector<Tile>(_height));

    _grassTexture = loadTexture("../src/assets/grass.png", renderer);
    _sandTexture = loadTexture("../src/assets/sand.png", renderer);
    _waterTexture = loadTexture("../src/assets/watter.png", renderer);
    _grassForestTexture = loadTexture("../src/assets/grass_forest.png", renderer);

    _treeTexture = loadTexture("../src/assets/tree.png", renderer);
    _smalltreeTexture = loadTexture("../src/assets/smalltree.png", renderer);
    _foresttreeTexture = loadTexture("../src/assets/forest_tree.png", renderer);
    _forestsmalltreeTexture = loadTexture("../src/assets/forest_smalltree.png", renderer);

    // _bushTexture = loadTexture("../src/assets/bush.png", renderer);
    _smallrockTexture = loadTexture("../src/assets/grass_rocks/grass_rock1.png", renderer);
    _rockTexture = loadTexture("../src/assets/grass_rocks/grass_rock2.png", renderer);
    _bigRockTexture = loadTexture("../src/assets/grass_rocks/grass_rock3.png", renderer);

    // loadGrassRocksTextures(renderer);
    // loadGrassLeafsTextures(renderer);
    // loadSandRocksTextures(renderer);
}

World::~World()
{
    std::cout << "DESTROY WORLD" << std::endl;
}

void World::loadGrassRocksTextures(SDL_Renderer* renderer) {
    _grassRocksTextures.push_back(loadTexture("../src/assets/grass_rocks/grass_rock1.png", renderer));
    _grassRocksTextures.push_back(loadTexture("../src/assets/grass_rocks/grass_rock2.png", renderer));
    _grassRocksTextures.push_back(loadTexture("../src/assets/grass_rocks/grass_rock3.png", renderer));
}

void World::loadGrassLeafsTextures(SDL_Renderer* renderer) {
    _grassLeafsTextures.push_back(loadTexture("../src/assets/grass_leafs/grass_leaf1.png", renderer));
    _grassLeafsTextures.push_back(loadTexture("../src/assets/grass_leafs/grass_leaf2.png", renderer));
    _grassLeafsTextures.push_back(loadTexture("../src/assets/grass_leafs/grass_leaf3.png", renderer));
}

void World::loadSandRocksTextures(SDL_Renderer* renderer) {
    _sandRocksTextures.push_back(loadTexture("../src/assets/sand_rocks/sand_rock1.png", renderer));
    _sandRocksTextures.push_back(loadTexture("../src/assets/sand_rocks/sand_rock2.png", renderer));
    _sandRocksTextures.push_back(loadTexture("../src/assets/sand_rocks/sand_rock3.png", renderer));
}

SDL_Texture* World::loadTexture(const char* filepath, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filepath);
    if (!texture) {
        SDL_Log("Failed to load texture %s: %s", filepath, SDL_GetError());
    }
    return texture;
}

void World::generate() {
    generateBiome();
    generateFeatures();
}

void World::generateBiome() {
    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            double noiseValue = _perlin.GetValue(x / 100.0, y / 100.0, 0.0);
            _noiseMap[x][y] = noiseValue;

            if (noiseValue < -0.2) {
                _tiles[x][y].biome = Biome::OCEAN;
            }
            else if (noiseValue < 0.0) {
                _tiles[x][y].biome = Biome::PLAGE;
            }
            else if (noiseValue < 0.3) {
                _tiles[x][y].biome = Biome::PLAINE;
            }
            else {
                _tiles[x][y].biome = Biome::FORET;
            }
        }
    }
}

void World::generateFeatures() {
    noise::module::Voronoi voronoi;
    voronoi.SetSeed(_seed);
    voronoi.EnableDistance(true);
    noise::module::Voronoi voronoiRock;
    voronoiRock.SetSeed(_seed + 1);
    voronoi.EnableDistance(true);


    for (int x = 0; x < _tiles.size(); ++x) {
        for (int y = 0; y < _tiles[x].size(); ++y) {
            _tiles[x][y].tree = Tree::NOTREE;

            double noiseValue = _perlin.GetValue(x / 100.0, y / 100.0, 0.0);
            double voronoiValue = voronoi.GetValue(x / 10.0, y / 10.0, 0.0);
            double voronoiRockValue = voronoiRock.GetValue(x / 10.0, y / 10.0, 0.0);

            if (noiseValue < 0.3 && noiseValue >= 0.0) {
                if (voronoiValue < 0.1) {
                    _tiles[x][y].tree = (voronoiValue < 0.05) ? Tree::TREE : Tree::SMALLTREE;
                }
            } else if (noiseValue < 0.5 && noiseValue >= 0.3) {
                if (voronoiValue < 0.15) {
                    _tiles[x][y].tree = (voronoiValue < 0.1) ? Tree::FORESTTREE : Tree::SMALLFORESTTREE;
                }
            }
        }
    }
}

SDL_Texture* World::getTileTexture(Biome biome) {
    switch (biome) {
        case Biome::PLAINE:
            return _grassTexture;
        case Biome::PLAGE:
            return _sandTexture;
        case Biome::OCEAN:
            return _waterTexture;
        case Biome::FORET:
            return _grassForestTexture;
        default:
            return _grassTexture;
    }
}

SDL_Texture* World::getTreeTexture(Tree tree) {
    if (tree == Tree::NOTREE)
        return nullptr;

    switch (tree) {
        case Tree::TREE:
            return _treeTexture;
        case Tree::SMALLTREE:
            return _smalltreeTexture;
        case Tree::FORESTTREE:
            return _foresttreeTexture;
        case Tree::SMALLFORESTTREE:
            return _forestsmalltreeTexture;
        default:
            return nullptr;
    }
}

SDL_Texture* World::getBushTexture(Bush bush) {
    if (bush == Bush::NOBUSH)
        return nullptr;

    switch (bush) {
        case Bush::BUSH:
            return nullptr;
            // return _bushTexture;
        default:
            return nullptr;
    }
}

SDL_Texture* World::getRockTexture(Rock rock) {
    if (rock == Rock::NOROCK)
        return nullptr;

    switch (rock) {
        case Rock::SMALLROCK:
            return _smallrockTexture;
        case Rock::ROCK:
            return _rockTexture;
        case Rock::BIGROCK:
            return _bigRockTexture;
        default:
            return nullptr;
    }
}

void World::render(SDL_Renderer* renderer, SDL_Point camera) {    
    int startX = std::max(0, camera.x / TILE_SIZE);
    int startY = std::max(0, camera.y / TILE_SIZE);
    int endX = std::min(_width, (camera.x + _width) / TILE_SIZE + 1);
    int endY = std::min(_height, (camera.y + _height) / TILE_SIZE + 1);

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            SDL_Texture* texture;

            texture = getTileTexture(_tiles[x][y].biome);

            SDL_FRect rect = {
                static_cast<float>(x * TILE_SIZE - camera.x), 
                static_cast<float>(y * TILE_SIZE - camera.y), 
                static_cast<float>(TILE_SIZE), 
                static_cast<float>(TILE_SIZE)
            };

            SDL_RenderTexture(renderer, texture, nullptr, &rect);
        }
    }
}

void World::renderFeatures(SDL_Renderer* renderer, SDL_Point camera) {   
    int startX = std::max(0, camera.x / TILE_SIZE);
    int startY = std::max(0, camera.y / TILE_SIZE);
    int endX = std::min(_width, (camera.x + _width) / TILE_SIZE + 1);
    int endY = std::min(_height, (camera.y + _height) / TILE_SIZE + 1);

    for (int x = startX; x < endX; ++x) {
        for (int y = startY; y < endY; ++y) {
            SDL_Texture* texture;

            texture = getTreeTexture(_tiles[x][y].tree);

            if (texture != nullptr) {
                SDL_FRect rectTree = {
                    static_cast<float>(x * TILE_SIZE - camera.x), 
                    static_cast<float>(y * TILE_SIZE - camera.y), 
                    static_cast<float>(TREE_SIZE), 
                    static_cast<float>(TREE_SIZE)
                };
                SDL_RenderTexture(renderer, texture, nullptr, &rectTree);
            }
            // texture = nullptr;
            // texture = getBushTexture(_tiles[x][y].bush);

            // if (texture != nullptr) {
            //     SDL_FRect rectBush = {
            //         static_cast<float>(x * TILE_SIZE - camera.x), 
            //         static_cast<float>(y * TILE_SIZE - camera.y), 
            //         static_cast<float>(TILE_SIZE), 
            //         static_cast<float>(TILE_SIZE)
            //     };
            //     SDL_RenderTexture(renderer, texture, nullptr, &rectBush);
            // }

            texture = nullptr;
            texture = getRockTexture(_tiles[x][y].rock);

            if (texture != nullptr) {
                SDL_FRect rectRock = {
                    static_cast<float>(x * TILE_SIZE - camera.x), 
                    static_cast<float>(y * TILE_SIZE - camera.y), 
                    static_cast<float>(TILE_SIZE), 
                    static_cast<float>(TILE_SIZE)
                };
                SDL_RenderTexture(renderer, texture, nullptr, &rectRock);
            }
        }
    }
} 
