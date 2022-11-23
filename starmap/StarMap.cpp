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

#define MAXDIS 10.0 //坐标范围是[-1,1]，因此最大距离不会超过3

//计算两点间距离
float Distance(Star a, Star b)
{
	float tmp = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	return sqrt(tmp);
}
void ChooseSplit(std::vector<Star>& vecStars, DIM& dim, Star& dimChoose)
{
	/*
	1. 计算每个维度(x,y,z)的方差,从具有最大方差的维度开始切分，如x方向；
	2. 计算x方向的参数得中值，作为起始节点；
	*/

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
		tmp1 += 1.0 / (float)vecStars.size() * vecStars[i].z * vecStars[i].z;
		tmp2 += 1.0 / (float)vecStars.size() * vecStars[i].z;
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
	// set the split point value:中值
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
		//起始切分方向为x方向
		for (size_t i = 0; i < vecStars.size(); ++i)
		{
			//小于等于节点star.x的属于左空间
			if (!equal(star, vecStars[i]) && vecStars[i].x <= star.x)
			{
				leftStar.push_back(vecStars[i]);
			}  //大于节点star.x的属于右空间
			else if (!equal(star, vecStars[i]) && vecStars[i].x > star.x)
			{
				rightStar.push_back(vecStars[i]);
			}
		}
	}
	else if (split == DIM::Y)
	{
		//起始切分方向为y方向
		for (size_t i = 0; i < vecStars.size(); ++i)
		{
			//小于等于节点star.y的属于左空间
			if (!equal(star, vecStars[i]) && vecStars[i].y <= star.y)
			{
				leftStar.push_back(vecStars[i]);

			}  //大于节点star.y的属于右空间
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
			//小于等于节点star.z的属于左空间
			if (!equal(star, vecStars[i]) && vecStars[i].z <= star.z)
			{
				leftStar.push_back(vecStars[i]);

			}  //大于节点star.z的属于右空间
			else if (!equal(star, vecStars[i]) && vecStars[i].z > star.z)
			{
				rightStar.push_back(vecStars[i]);
			}
		}
	}
	return true;
}

//递归创建kdtree
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
		ChooseSplit(vecStar, split, curStar);   //返回起始切分方向和起始节点
		std::vector<Star> rightStar;  //存储位于右子空间的点
		std::vector<Star> leftStar;   //存储位于左子空间的点
		splitPointsToDiffSpace(vecStar, rightStar, leftStar, split, curStar);

		T = new TreeNode;
		T->star.x = curStar.x;
		T->star.y = curStar.y;
		T->star.z = curStar.z;
		T->star.id = curStar.id;
		T->dim = split;
		T->left = build_kdtree(leftStar, T->left);    //递归
		T->right = build_kdtree(rightStar, T->right);  //递归
	}
	return T;
}
void updateVecWithStar(vector<StarNode>& vec,const unsigned int& id, const size_t& maxCount,double dis, double& maxDis)
{
	//升序排列数据 最大值为最后一个，超过容量时删除，模拟最大堆使用方式
	StarNode cur;
	cur.dis = dis;
	cur.id = id;

	//去除重复数据
	for (auto iter : vec)
	{
		if (iter.id == id)
		{
			return;
		}
	}
	vec.push_back(cur);
	sort(vec.begin(), vec.end(), [](StarNode a, StarNode b) { return a.dis < b.dis; });
	if (vec.size() > maxCount)
	{
		vec.pop_back();
	}
	maxDis = vec.at(vec.size()-1).dis;
}

//搜索最邻近点
vector<unsigned int> searchNearest(TreeNode* root, Star target,const size_t& count)
{
	vector<StarNode> result;

	// 1. 如果m_root是空的，则设dist为无穷大返回
	// 2. 向下搜索直到叶子结点
	stack<TreeNode*> search_path;
	TreeNode*        pSearch = root;
	Star			 nearest;
	float           dist;
	float			 curMaxDis;
	while (pSearch != nullptr)
	{
		// pSearch加入到search_path中;
		search_path.push(pSearch);

		if (pSearch->dim == DIM::X)
		{
			if (target.x <= pSearch->star.x) /* 如果小于就进入左子树 */
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
			if (target.y <= pSearch->star.y) /* 如果小于就进入左子树 */
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
			if (target.z <= pSearch->star.z) /* 如果小于就进入左子树 */
			{
				pSearch = pSearch->left;
			}
			else
			{
				pSearch = pSearch->right;
			}
		}

	}
	updateVecWithStar(result, 0, count, 10, curMaxDis);

	//取出search_path最后一个赋给nearest
	nearest.x = search_path.top()->star.x;
	nearest.y = search_path.top()->star.y;
	nearest.z = search_path.top()->star.z;
	nearest.id = search_path.top()->star.id;
	search_path.pop();

	dist = Distance(nearest, target);

	updateVecWithStar(result, nearest.id, count, dist, curMaxDis);

	// 3. 回溯搜索路径。关键点:往前回溯时，为了快速找到回溯到节点的父节点，可以用栈保存走过的路径。
	TreeNode* pBack;
	while (!search_path.empty())
	{
		//取出search_path最后一个结点赋给pBack
		pBack = search_path.top();
		search_path.pop();

		dist = Distance(pBack->star, target);
		updateVecWithStar(result, pBack->star.id, count, dist, curMaxDis);

		//到父节点的子空间中搜索
		unsigned int s = pBack->dim;
		if (s == 1)
		{
			/* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交，那么就要跳到另一边的子空间去进行递归搜索。循环就是不断遍历插入子节点 */
			if (fabs(pBack->star.x - target.x) < curMaxDis)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{

					if (target.x <= pSearch->star.x) /*如果target位于pBack的左子空间，那么就要跳到右子空间去搜索*/
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索*/
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
			/* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交，那么就要跳到另一边的子空间去递归搜索。循环就是不断遍历插入子节点 */
			if (fabs(pBack->star.y - target.y) < curMaxDis)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{

					if (target.x <= pSearch->star.x) /*如果target位于pBack的左子空间，那么就要跳到右子空间去搜索*/
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
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
			/* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交，那么就要跳到另一边的子空间去递归搜索。循环就是不断遍历插入子节点 */
			if (fabs(pBack->star.z - target.z) < curMaxDis)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{
					if (target.z <= pSearch->star.z) /*如果target位于pBack的左子空间，那么就要跳到右子空间去搜索*/
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索*/
					}
					if (pSearch != nullptr)
					{
						search_path.push(pSearch);
					}
				}
			}
		}
	}

	//返回坐标对应的id
	vector<unsigned int> vec;
	for (auto iter : result)
	{
		if (iter.id != 0)
		{
			vec.push_back(iter.id);
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
			if (f->left != NULL) q.push(f->left);//如果子女不为空就加入子女，一会儿删除
			if (f->right != NULL) q.push(f->right);
			delete f;//删除当前节点
			q.pop();//把没用的指针弹出
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
	vector<Star> ret;
	vector<unsigned int> vec = searchNearest(m_root, pointStar, n);
	for (auto iter : vec)
	{
		ret.push_back(m_vecStars[iter-1]);
	}
	return ret;
}
