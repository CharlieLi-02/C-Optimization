#include "StarMap.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <stack>
#include <queue>

using std::stack;
using std::vector;
using std::string;
using std::cout;
using std::endl;

#define MAXDIS 10.0 //

float Distance_Square(Star a, Star b)
{
	float tmp = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	return tmp;
}

void ChooseSplit(std::vector<Star>& vecStars, DIM& dim, Star& dimChoose)
{
    float tmp1, tmp2;
	tmp1 = tmp2 = 0;
	for (size_t i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += 1.0 / (float)vecStars.size() * vecStars[i].x * vecStars[i].x;
		tmp2 += 1.0 / (float)vecStars.size() * vecStars[i].x;
	}
    float v1 = tmp1 - tmp2 * tmp2;  // compute variance on the x dimension

	tmp1 = tmp2 = 0;
	for (size_t i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += 1.0 / (float)vecStars.size() * vecStars[i].y * vecStars[i].y;
		tmp2 += 1.0 / (float)vecStars.size() * vecStars[i].y;
	}
    float v2 = tmp1 - tmp2 * tmp2;  // compute variance on the y dimension

	tmp1 = tmp2 = 0;
	for (size_t i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += 1.0 / (double)vecStars.size() * vecStars[i].z * vecStars[i].z;
		tmp2 += 1.0 / (double)vecStars.size() * vecStars[i].z;
	}
    float v3 = tmp1 - tmp2 * tmp2;

	float max_variance;
	max_variance = v1 > v2 ? v1 : v2;
	max_variance = max_variance > v3 ? max_variance : v3;

	if (max_variance == v1)
	{
		dim = X;
		sort(vecStars.begin(), vecStars.end(), [](Star a, Star b) { return a.x < b.x; });
	}
	else if (max_variance == v2)
	{
		dim = Y;
		sort(vecStars.begin(), vecStars.end(), [](Star a, Star b) { return a.y < b.y; });
	}
	else
	{
		dim = Z;
		sort(vecStars.begin(), vecStars.end(), [](Star a, Star b) { return a.z < b.z; });
	}
	// set the split point value:
	dimChoose.x = vecStars[vecStars.size() / 2].x;
	dimChoose.y = vecStars[vecStars.size() / 2].y;
	dimChoose.z = vecStars[vecStars.size() / 2].z;
	dimChoose.id = vecStars[vecStars.size() / 2].id;
}

bool equal(const Star& a, const Star& b)
{
	return (a.id == b.id);
}

bool splitPointsToDiffSpace(std::vector<Star> const& vecStars, std::vector<Star>& rightStar,
	std::vector<Star>& leftStar, DIM const split, Star const& star)
{
	if (vecStars.empty())
	{
		return false;
	}

	if (split == DIM::X)
	{
		for (size_t i = 0; i < vecStars.size(); ++i)
		{
			if (!equal(star, vecStars[i]) && vecStars[i].x <= star.x)
			{
				leftStar.push_back(vecStars[i]);
			}
			else if (!equal(star, vecStars[i]) && vecStars[i].x > star.x)
			{
				rightStar.push_back(vecStars[i]);
			}
		}
	}
	else if (split == DIM::Y)
	{
		for (size_t i = 0; i < vecStars.size(); ++i)
		{
			if (!equal(star, vecStars[i]) && vecStars[i].y <= star.y)
			{
				leftStar.push_back(vecStars[i]);

			}
			else if (!equal(star, vecStars[i]) && vecStars[i].y > star.y)
			{
				rightStar.push_back(vecStars[i]);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < vecStars.size(); ++i)
		{
			if (!equal(star, vecStars[i]) && vecStars[i].z <= star.z)
			{
				leftStar.push_back(vecStars[i]);
            }
			else if (!equal(star, vecStars[i]) && vecStars[i].z > star.z)
			{
				rightStar.push_back(vecStars[i]);
			}
		}
	}
	return true;
}

TreeNode* build_kdtree(std::vector<Star> vecStar, TreeNode* T)
{
	// call function ChooseSplit to choose the split dimension and split point
	if (vecStar.size() == 0)
	{
		return nullptr;
	}
	else
	{
		DIM  split;
		Star curStar;
		ChooseSplit(vecStar, split, curStar);
		std::vector<Star> rightStar;
		std::vector<Star> leftStar;
		splitPointsToDiffSpace(vecStar, rightStar, leftStar, split, curStar);

		T = new TreeNode;
		T->star.x = curStar.x;
		T->star.y = curStar.y;
		T->star.z = curStar.z;
		T->star.id = curStar.id;
		T->dim = split;
		T->left = build_kdtree(leftStar, T->left);
		T->right = build_kdtree(rightStar, T->right);
	}
	return T;
}

void updateVecWithStar(vector<StarNode>& vec, StarNode& target, const size_t& maxCount, float& maxDis)
{
	for (auto iter : vec)
	{
		if (iter.star.id == target.star.id)
		{
			return;
		}
	}
	vec.push_back(target);
	sort(vec.begin(), vec.end(), [](StarNode a, StarNode b) {return a.dis < b.dis; });
	if (vec.size() > maxCount)
	{
		vec.pop_back();
	}
	maxDis = vec.at(vec.size()-1).dis;
}

vector<Star> searchNearest(TreeNode* root, Star target, const size_t& count)
{
	vector<StarNode> result;
	stack<TreeNode*> search_path;
	TreeNode*        pSearch = root;
	StarNode		 nearest;
	float            dist_square;
    float			 curMaxDis_square;
	while (pSearch != nullptr)
	{
		search_path.push(pSearch);

		if (pSearch->dim == DIM::X)
		{
			if (target.x <= pSearch->star.x)
			{
				pSearch = pSearch->left;
			}
			else
			{
				pSearch = pSearch->right;
			}
		}
		else if (pSearch->dim == DIM::Y)
		{
			if (target.y <= pSearch->star.y)
			{
				pSearch = pSearch->left;
			}
			else
			{
				pSearch = pSearch->right;
			}
		}
		else
		{
			if (target.z <= pSearch->star.z)
			{
				pSearch = pSearch->left;
			}
			else
			{
				pSearch = pSearch->right;
			}
		}
    }

    nearest.star = search_path.top()->star;
    nearest.dis = Distance_Square(nearest.star, target);
    search_path.pop();

	updateVecWithStar(result, nearest, count, curMaxDis_square);

	TreeNode* pBack;
    
	while (!search_path.empty())
	{
		pBack = search_path.top();
        StarNode current;
        current.star = pBack->star;
        current.dis = Distance_Square(current.star, target);
        search_path.pop();
		updateVecWithStar(result, current, count, curMaxDis_square);

		unsigned int s = pBack->dim;
		if (s == 1)
		{
			if ((pBack->star.x - target.x)*(pBack->star.x - target.x) < curMaxDis_square)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{

					if (target.x <= pSearch->star.x)
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left;
					}

					if (pSearch != nullptr)
					{
						search_path.push(pSearch);
					}
				}
			}
		}
		else if (s == 2)
		{
			if ((pBack->star.y - target.y)*(pBack->star.y - target.y) < curMaxDis_square)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{

					if (target.x <= pSearch->star.x)
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left;
					}
					if (pSearch != nullptr)
					{
						search_path.push(pSearch);
					}
				}
			}
		}
		else
		{
			if ((pBack->star.z - target.z)*(pBack->star.z - target.z) < curMaxDis_square)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{
					if (target.z <= pSearch->star.z)
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left;
					}
					if (pSearch != nullptr)
					{
						search_path.push(pSearch);
					}
				}
			}
		}
	}

	vector<Star> vec;
	for (auto iter : result)
	{
		if (iter.star.id != 0)
		{
            iter.dis = sqrt(iter.dis);
			vec.push_back(iter.star);
		}
	}
	return vec;
}

void DeleteTree(TreeNode* root)
{
	if (root != nullptr)
	{
		std::queue<TreeNode*> q;
		q.push(root);
		while (!q.empty())
		{
			TreeNode* f = q.front();
			if (f->left != NULL) q.push(f->left);
			if (f->right != NULL) q.push(f->right);
			delete f;
			q.pop();
		}
		root = NULL;
	}
}

StarMap* StarMap::create(std::istream& stream) {
  // This default implementation will probably do what you want.
  // If you use a different constructor, you'll need to change it.
  return new StarMap(stream);
}

StarMap::StarMap(std::istream& stream)
{
    Star* ptr;
    ptr = new Star;

    int nID = 1;
    std::string strTemp;
    while (!(stream.eof())) 
    {
        ptr->id = nID++;
        stream >> ptr->x;
        stream >> ptr->y;
        stream >> ptr->z;
		m_vecStars.emplace_back(*ptr);
    }

	m_vecStars.pop_back(); // To get rid of the blank lines at the end of the text files.
    delete ptr;

	m_root = build_kdtree(m_vecStars, m_root);
}		   


StarMap::~StarMap()
{
	if (m_root != nullptr)
	{
		DeleteTree(m_root);
	}
}

std::vector<Star> StarMap::find(size_t n, float xCor, float yCor, float zCor)
{
    Star pointStar;
    pointStar.x = xCor;
    pointStar.y = yCor;
    pointStar.z = zCor;
	vector<Star> vec = searchNearest(m_root, pointStar, n);
	return vec;
}
