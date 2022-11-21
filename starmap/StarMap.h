#ifndef STARMAP_H
#define STARMAP_H

#include "Star.h"
#include "Helpers.h"
#include <istream>
#include <vector>
#include <cstddef>

class StarMap {
public:
    static StarMap* create(std::istream& stream);

private:
    std::vector<Star> m_vecStars;
    std::vector<Star> m_vecX;
    std::vector<Star> m_vecY;

public:
    // Constructor & Destructor
    StarMap(std::istream& stream);
    ~StarMap();

    // Required Member Function
    std::vector<Star> find(size_t n, float x, float y, float z);

    float closest(const Star& star, Heap& heap);

    float divideandConquer(const Star& star, const std::vector<Star>& vecX, const std::vector<Star>& vecY,
        const size_t size, Heap& heap);

    void insertRes(const Star& star, float dist, Heap& heap);

};

#endif
