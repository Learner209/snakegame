#ifndef TERRAIN_H
#define TERRAIN_H
#include <vector>

enum terrain
{
    Plain = 0,
    Water = 1,
    Mountain = 2,
    Forest = 3,
    Maze = 4,
};

typedef std::pair<int, int> Block;
// Terrain class should have no dependency on the GUI library
class Terrain
{
public:
    //Terrain();
    Terrain(int gameBoardWidth, int gameBoardHeight, terrain map);
    ~Terrain();

    // Initialize terrain
    void initializeTerrain(int difficulty);
    void initialize(int difficulty);
    void initializeMountain(int difficulty);
    void initializeMaze(int difficulty);

    // Terrain Map
    std::vector<Block>& getTerrains();
    int getSize() const;
    bool inBounds(Block) const;
    bool inVector(std::vector<Block>, Block) const;
    terrain getattr() const;

protected:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    terrain mTerrain;
    std::vector<Block> Terrains = {};


    //auxiliary function
    int randomInteger(int low, int high) const;

};
#endif //TERRAIN_H
