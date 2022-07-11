#include "terrain.h"
#include <queue>
#include <random>
#include <cassert>
//Terrain();

Terrain::Terrain(int gameBoardWidth, int gameBoardHeight, terrain map): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mTerrain(map)
{
}

Terrain::~Terrain()
{
}

// Initialize terrain
void Terrain::initializeTerrain(int difficulty)
{
    if (this->mTerrain == Plain)
    {
        this->Terrains = {};
    }
    else if (this->mTerrain == Mountain)
    {
        this->initializeMountain(difficulty);
    }
    else if (this->mTerrain != Maze)
    {
        this->initialize(difficulty);
    }
    else
    {
        this->initializeMaze(difficulty);
    }
}

void Terrain::initialize(int difficulty)
{
    int pools = difficulty + 1;
    int area = (this->mGameBoardWidth * this->mGameBoardHeight) / (60 * 20) * 60;
    for (int i = 0; i < pools; i++)
    {
        int width, height;
        do{
            width = rand() % (this->mGameBoardWidth - 2) + 1;
            height = rand() % (this->mGameBoardHeight - 2) + 1;
        }while(inVector(this->Terrains, std::make_pair(width, height)));
        Block init = std::make_pair(width, height);
        std::queue<Block> pool;
        pool.push(init);

        while(this->Terrains.size() < (i + 1) * area)
        {
            int l = pool.size();
            Block block = pool.front();
            pool.pop();
            for(int i = 0; i < l; i++)
            {
                int probability = this->randomInteger(0, 8);
                if (probability < 2)
                {
                    auto left = std::make_pair(block.first - 1, block.second);
                    if (inBounds(left) && !inVector(this->Terrains, left))
                    {
                        this->Terrains.push_back(left);
                        pool.push(left);
                    }
                }
                if (probability > 6)
                {
                    auto right = std::make_pair(block.first + 1, block.second);
                    if (inBounds(right) && !inVector(this->Terrains, right))
                    {
                        this->Terrains.push_back(right);
                        pool.push(right);
                    }
                }
                if (probability > 4)
                {
                    auto up = std::make_pair(block.first, block.second - 1);
                    if (inBounds(up) && !inVector(this->Terrains, up))
                    {
                        this->Terrains.push_back(up);
                        pool.push(up);
                    }
                }
                if (probability)
                {
                    auto down = std::make_pair(block.first, block.second + 1);
                    if (inBounds(down) && !inVector(this->Terrains, down))
                    {
                        this->Terrains.push_back(down);
                        pool.push(down);
                    }
                }
            }
        }
    }
}

void Terrain::initializeMountain(int difficulty)
{
    //srand(time(nullptr));
    int pools = difficulty + 1;
    int area = (this->mGameBoardWidth * this->mGameBoardHeight) / (60 * 20) * 60;
    for (int i = 0; i < pools; i++)
    {
        int width, height;

        do{
            width = rand() % (this->mGameBoardWidth - 2) + 1;
            height = rand() % (this->mGameBoardHeight - 2) + 1;
        }while(inVector(this->Terrains, std::make_pair(width, height))
            || (width - this->mGameBoardWidth / 2) * (width - this->mGameBoardWidth / 2) +
               (height - this->mGameBoardHeight / 2) * (height - this->mGameBoardHeight / 2) < area);

        Block init = std::make_pair(width, height);
        std::queue<Block> pool;
        pool.push(init);

        while(this->Terrains.size() < (i + 1) * area)
        {
            int l = pool.size();
            Block block = pool.front();
            pool.pop();
            for(int i = 0; i < l; i++)
            {
                int probability = this->randomInteger(0, 8);
                if (probability < 3)
                {
                    auto left = std::make_pair(block.first - 1, block.second);
                    if (inBounds(left) && !inVector(this->Terrains, left))
                    {
                        this->Terrains.push_back(left);
                        pool.push(left);
                    }
                }
                if (probability > 6)
                {
                    auto right = std::make_pair(block.first + 1, block.second);
                    if (inBounds(right) && !inVector(this->Terrains, right))
                    {
                        this->Terrains.push_back(right);
                        pool.push(right);
                    }
                }
                if (probability < 2)
                {
                    auto up = std::make_pair(block.first, block.second - 1);
                    if (inBounds(up) && !inVector(this->Terrains, up))
                    {
                        this->Terrains.push_back(up);
                        pool.push(up);
                    }
                }
                if (probability)
                {
                    auto down = std::make_pair(block.first, block.second + 1);
                    if (inBounds(down) && !inVector(this->Terrains, down))
                    {
                        this->Terrains.push_back(down);
                        pool.push(down);
                    }
                }
            }
        }
    }
}
void Terrain::initializeMaze(int difficulty)
{
    return;
}
// Terrain Map
void Terrain::changeTerrain(terrain type)
{
    this->mTerrain = type;
}
std::vector<Block>& Terrain::getTerrains()
{
    return this->Terrains;
}
int Terrain::getSize() const
{
    return this->Terrains.size();
}

terrain Terrain::getattr() const
{
    return this->mTerrain;
}

bool Terrain::inBounds(Block block) const
{
    return (block.first > 0 && block.first < this->mGameBoardWidth - 1
            && block.second > 0 && block.second < this->mGameBoardHeight - 1);
}

bool Terrain::inVector(std::vector<Block> Terrains, Block block) const
{
    int l = Terrains.size();
    for (int i = 0; i < l; i++)
    {
        if (Terrains[i].first == block.first && Terrains[i].second == block.second) return true;
    }
    return false;
}
//auxiliary function
int Terrain::randomInteger(int low, int high) const
{
    return low + rand() % (high - low);
}
