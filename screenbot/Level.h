#ifndef LEVEL_H_
#define LEVEL_H_

#include <string>
#include "Common.h"

#pragma pack(push, 1)
struct Tile {
    unsigned long x : 12;
    unsigned long y : 12;
    unsigned long tile : 8;
};
#pragma pack(pop)

class Level {
private:
    tstring m_Filename;
    int *m_Tiles;

public:
    Level();
    bool Load(const tstring& filename);

    static bool IsSolid(int id);
    int GetTileID(unsigned short x, unsigned short y) const;
};

#endif
