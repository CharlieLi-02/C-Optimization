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

void updateVecWithStar(vector<TreeNode*>& vec, TreeNode* target, const size_t& maxCount, float& maxDis)
{
	if(vec.size() < 2){
    		vec.push_back(target);
		return;
	}
	vec.push_back(target);
	sort(vec.begin(), vec.end(), [](TreeNode* a, TreeNode* b) {return a->dis < b->dis; });
	if (vec.size() > maxCount)
	{
		vec.pop_back();
	}
	maxDis = vec.at(vec.size()-1)->dis;
}

vector<TreeNode*> searchPath(TreeNode* root, Star target){
    vector<TreeNode*> search_path;
    TreeNode*        pSearch = root;
    while (pSearch != nullptr)
    {
        search_path.push_back(pSearch);

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
    return search_path;
}

void searchBranch(TreeNode* pSearch, vector<TreeNode*>& vec, Star target, const size_t& count, float& curMaxDis_square){
    if(pSearch == nullptr) {
        return;
    }
    
    updateVecWithStar(vec, pSearch, count, curMaxDis_square);
    unsigned int s = pSearch->dim;
    if (s == 1)
    {
        if ((pSearch->star.x - target.x)*(pSearch->star.x - target.x) < curMaxDis_square)
        {
            while (pSearch != nullptr)
            {
                if (pSearch->left != nullptr)
                {
                    searchBranch(pSearch->left, vec, target, count, curMaxDis_square);
                }
                if (pSearch->right != nullptr)
                {
                    searchBranch(pSearch->right, vec, target, count, curMaxDis_square);
                }
            }
        }
    }
    else if (s == 2)
    {
        if ((pSearch->star.y - target.y)*(pSearch->star.y - target.y) < curMaxDis_square)
        {
            while (pSearch != nullptr)
            {
                if (pSearch->left != nullptr)
                {
                    searchBranch(pSearch->left, vec, target, count, curMaxDis_square);
                }
                if (pSearch->right != nullptr)
                {
                    searchBranch(pSearch->right, vec, target, count, curMaxDis_square);
                }
            }
        }
    }
    else if (s == 3)
    {
        if ((pSearch->star.z - target.z)*(pSearch->star.z - target.z) < curMaxDis_square)
        {
            while (pSearch != nullptr)
            {
                if (pSearch->left != nullptr)
                {
                    searchBranch(pSearch->left, vec, target, count, curMaxDis_square);
                }
                if (pSearch->right != nullptr)
                {
                    searchBranch(pSearch->right, vec, target, count, curMaxDis_square);
                }
            }
        }
    }
    else {
        std::cout << "dimension error" << std::endl;
    }
}

vector<Star> searchNearest(TreeNode* root, Star target, const size_t& count)
{
    vector<TreeNode*> search_path = searchPath(root, target);
    float             curMaxDis_square;
	vector<TreeNode*> result;
    curMaxDis_square = Distance_Square(search_path.back()->star, target);
    
    while(result.size() <= count) {
        searchBranch(search_path.back(), result, target, count, curMaxDis_square);
        search_path.pop_back();
    }

	vector<Star> vec;
	for (auto iter : result)
	{
		if (iter->star.id != 0)
		{
            std::cout << "the star with index " << iter->star.id << " distance is " << iter->dis << std::endl;
            iter->dis = sqrt(iter->dis);
			vec.push_back(iter->star);
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
