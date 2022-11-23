#ifndef HELPERS_H
#define HELPERS_H

#include "Star.h"

//Œ¨∂»√∂æŸ÷µ
enum DIM : unsigned int
{
	X = 0x1,
	Y = 0x2,
	Z = 0x3
};

//k-d tree 
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
	double dis = 0.0;
};

#endif
