#ifndef HELPERS_H
#define HELPERS_H

#include "Star.h"

enum DIM : unsigned int
{
	X = 0x1,
	Y = 0x2,
	Z = 0x3
};

struct TreeNode
{
	struct Star       star;
	unsigned int      dim = 0;
	struct TreeNode*  left = nullptr;
	struct TreeNode*  right = nullptr;
};

struct StarNode
{
	unsigned int id = 0;
	float dis = 0.0;
};

#endif
