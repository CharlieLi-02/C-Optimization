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

	TreeNode* m_root;

public:
    // Constructor & Destructor
    StarMap(std::istream& stream);
    ~StarMap();

    // Required Member Function
    std::vector<Star> find(size_t n, float x, float y, float z);

};

#endif
