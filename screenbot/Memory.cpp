#include "Memory.h"

namespace Memory {

std::vector<WritableArea> GetWritableAreas(HANDLE handle) {
    MEMORY_BASIC_INFORMATION meminfo;
    DWORD pos = 0;
    const int Writable = (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
    std::vector<WritableArea> areas;

    while (VirtualQueryEx(handle, (LPVOID)pos, &meminfo, sizeof(meminfo))) {
        if ((meminfo.Protect & Writable))
            areas.push_back(WritableArea(reinterpret_cast<unsigned int>(meminfo.BaseAddress), static_cast<unsigned int>(meminfo.RegionSize)));
        pos += meminfo.RegionSize;
    }

    return areas;
}

std::vector<unsigned int> FindRange(HANDLE handle, const unsigned int start, const unsigned int end) {
    const unsigned int upper = 0x7FFFFFFF;
    std::vector<unsigned int> found;

    std::vector<WritableArea> areas = GetWritableAreas(handle);

    for (WritableArea& area : areas) {
        if (area.size == 0) continue;

        char *buffer = new char[area.size];
        SIZE_T num_read;

        if (ReadProcessMemory(handle, (LPVOID)area.base, buffer, area.size, &num_read)) {
            for (unsigned int i = 0; i < num_read - 4; i += 4) {
                unsigned int check = *reinterpret_cast<unsigned int *>(buffer + i);

                if (check >= start && check <= end)
                    found.push_back(area.base + i);
            }
        }

        delete[] buffer;
    }

    return found;
}


unsigned int GetU32(HANDLE handle, const unsigned int address) {
    unsigned int value;
    SIZE_T num_read;

    if (ReadProcessMemory(handle, (LPVOID)address, &value, sizeof(unsigned int), &num_read))
        return value;

    return 0;
}

}