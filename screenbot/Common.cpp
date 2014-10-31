#include "Common.h"

tostream& operator<<(tostream& out, const Pixel& pix) {
    out << "(" << (int)pix.r << ", " << (int)pix.g << ", " << (int)pix.b << ", " << (int)pix.a << ")";
    return out;
}

u64 operator+(const Pixel& pix, u64 a) {
    return ((pix.r << 16) | (pix.g << 8) | (pix.b)) + a;
}

bool operator==(const Pixel& first, const Pixel& second) {
    return first.r == second.r && first.g == second.g &&
        first.b == second.b;
}

bool operator!=(const Pixel& first, const Pixel& second) {
    return !(first == second);
}

tostream& operator<<(tostream& out, const Coord& coord) {
    out << "(" << coord.x << ", " << coord.y << ")";
    return out;
}

bool operator==(const Coord& first, const Coord& second) {
    return first.x == second.x && first.y == second.y;
}

bool operator!=(const Coord& first, const Coord& second) {
    return !(first == second);
}

bool operator<(const Coord& first, const Coord& second) {
    if (first.y < second.y) return true;
    if (first.y == second.y) return first.x < second.x;
    return false;
}