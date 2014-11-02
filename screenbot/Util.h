#ifndef UTIL_H_
#define UTIL_H_

#include "Common.h"
#include <vector>

class Level;

namespace Util {
    int GetEnergy(ScreenAreaPtr* energyarea);
    int GetEnergyDigit(int digit, ScreenAreaPtr* energyarea);
    int GetRotation(const ScreenAreaPtr& area);
    std::vector<Coord> GetEnemies(ScreenAreaPtr& radar);
    bool InSafe(const ScreenAreaPtr& area, Coord coord);
    Coord GetClosestEnemy(const std::vector<Coord>& enemies, ScreenAreaPtr& radar, int* dx, int* dy, double* dist);
    void GetDistance(Coord from, Coord to, int *dx, int *dy, double* dist);
    int GetTargetRotation(int dx, int dy);
    bool PlayerInSafe(const ScreenAreaPtr& player);
    bool XRadarOn(const ScreenGrabberPtr& grabber);
    int GetShip(const ScreenAreaPtr& ship);
    int GetShipRadius(int n);
    bool InShip(const ScreenGrabberPtr& grabber);
    bool IsClearPath(Coord from, Coord target, int radius, const Level& level);
}

#endif
