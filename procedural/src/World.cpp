/*
** Felix Schrynemaekers Project
** ProceduralX
** File description:
** World
*/

#include "World.hpp"

World::World(std::size_t width, std::size_t height, unsigned int seed, SDL_Renderer* renderer)
{
    _width = width;
    _height = height;
    _seed = seed;
    _rng = std::mt19937(seed);
    _tiles.resize(height, std::vector<Tile>(width));

    desertTexture = loadTexture("../src/assets/desert.png", renderer);
    plaineTexture = loadTexture("../src/assets/plaine.png", renderer);
    foretTexture = loadTexture("../src/assets/foret.png", renderer);
    riviereTexture = loadTexture("../src/assets/riviere.png", renderer);
    montagneTexture = loadTexture("../src/assets/montagne.png", renderer);
    oceanTexture = loadTexture("../src/assets/ocean.png", renderer);
}

World::~World()
{
    SDL_DestroyTexture(desertTexture);
    SDL_DestroyTexture(plaineTexture);
    SDL_DestroyTexture(foretTexture);
    SDL_DestroyTexture(riviereTexture);
}

float fbm(float x, float y, int octaves, float persistence) {
    float total = 0.0f;
    float frequency = 0.01f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;  

    for (int i = 0; i < octaves; i++) {
        total += PerlinNoise::perlin(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude; 
        amplitude *= persistence; 
        frequency *= 2.0f;
    }

    return total / maxValue; 
}

float World::getNoiseValue(std::size_t x, std::size_t y) {
    float elevation = fbm(x + _seed * 10, y + _seed * 10, 4, 0.5f);
    float moisture = fbm(x + _seed * 20, y + _seed * 20, 4, 0.5f);
    return elevation * 0.7f + moisture * 0.3f;
}

void World::generate()
{
    generateBiome();
    generateRivers();
}

void World::generateBiome()
{
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            float noiseValue = getNoiseValue(x, y);
            float moisture = fbm(x + _seed * 30, y + _seed * 30, 4, 0.5f); 

            std::cout << noiseValue << " ";
            if (noiseValue < -0.169386f)
                _tiles[y][x].biome = Biome::OCEAN;
            else if (noiseValue < 0.0f) 
                _tiles[y][x].biome = Biome::RIVIERE;
            else if (noiseValue < 0.4f && moisture < 0.5f)
                _tiles[y][x].biome = Biome::PLAINE;
            else if (noiseValue < 0.6f)
                _tiles[y][x].biome = Biome::FORET;
            else
                _tiles[y][x].biome = Biome::MONTAGNE;
        }
        std::cout << std::endl;
    }
}

void World::generateRivers() {
    int numRivers = 3 + (_seed % 3);
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int i = 0; i < numRivers; i++) {
        int x = rand() % _width;
        int y = rand() % _height;

        int riverLength = 50 + rand() % 50;

        for (int j = 0; j < riverLength; j++) {
            _tiles[y][x].biome = Biome::RIVIERE;

            float minNoise = getNoiseValue(x, y);
            int bestX = x, bestY = y;
            bool moved = false;

            for (auto& dir : directions) {
                int nx = x + dir.first;
                int ny = y + dir.second;

                if (nx >= 0 && nx < _width && ny >= 0 && ny < _height) {
                    float noise = getNoiseValue(nx, ny);
                    if (noise < minNoise || (rand() % 100 < 20)) {
                        minNoise = noise;
                        bestX = nx;
                        bestY = ny;
                        moved = true;
                    }
                }
            }

            if (!moved) { 
                auto dir = directions[rand() % directions.size()];
                int nx = x + dir.first;
                int ny = y + dir.second;
                if (nx >= 0 && nx < _width && ny >= 0 && ny < _height) {
                    bestX = nx;
                    bestY = ny;
                }
            }

            x = bestX;
            y = bestY;
        }
    }
}




SDL_Texture* World::loadTexture(const char* filepath, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filepath);
    if (!texture) {
        SDL_Log("Failed to load texture %s: %s", filepath, SDL_GetError());
    }
    return texture;
}

void World::render(SDL_Renderer* renderer, SDL_Point camera) {
    for (std::size_t y = 0; y < _height; y++) {
        for (std::size_t x = 0; x < _width; x++) {
            int isoX = (x - y) * (TILE_SIZE / 2) - camera.x;
            int isoY = (x + y) * (TILE_SIZE / 4) - camera.y;

            SDL_FRect tileRect = {
                static_cast<float>(isoX),
                static_cast<float>(isoY),
                static_cast<float>(TILE_SIZE),
                static_cast<float>(TILE_SIZE)
            };

            SDL_Texture* texture = nullptr;
            switch (_tiles[y][x].biome) {
                case Biome::DESERT: texture = desertTexture; break;
                case Biome::PLAINE: texture = plaineTexture; break;
                case Biome::FORET: texture = foretTexture; break;
                case Biome::RIVIERE: texture = riviereTexture; break;
                case Biome::OCEAN: texture = oceanTexture; break;
                case Biome::MONTAGNE: texture = montagneTexture; break;
            }

            if (texture) {
                SDL_RenderTexture(renderer, texture, nullptr, &tileRect);
            }
        }
    }
}

