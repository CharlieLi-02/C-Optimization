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

//计算两点间距离
double Distance(Star a, Star b)
{
	double tmp = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	return sqrt(tmp);
}
void ChooseDim(std::vector<Star>& vecStars, DIM& dim, Star& dimChoose)
{
	/*
	1. 计算每个维度(x,y,z)的方差,从具有最大方差的维度开始切分，如x方向；
	2. 计算x方向的参数得中值，作为起始节点；
	*/

	double tmp1, tmp2;
	tmp1 = tmp2 = 0;
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp2 += 1.0 / (double)vecStars.size() * vecStars[i].x;
	}
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += (tmp2 - vecStars[i].x)*(tmp2 - vecStars[i].x) / (double)vecStars.size();
	}
	double v1 = tmp1;

	tmp1 = tmp2 = 0;
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp2 += 1.0 / (double)vecStars.size() * vecStars[i].y;
	}
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += (tmp2 - vecStars[i].y)*(tmp2 - vecStars[i].y) / (double)vecStars.size();
	}

	double v2 = tmp1;  // compute variance on the y dimension

	tmp1 = tmp2 = 0;
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp2 += 1.0 / (double)vecStars.size() * vecStars[i].z;
	}
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += (tmp2 - vecStars[i].z)*(tmp2 - vecStars[i].z) / (double)vecStars.size();
	}

	double v3 = tmp1;

	double max_variance;
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
//	cout << "v1" << v1 << " v2" << v2 << " v3" << v3 << endl;

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
		for (unsigned int i = 0; i < vecStars.size(); ++i)
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
		for (unsigned int i = 0; i < vecStars.size(); ++i)
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
		for (unsigned int i = 0; i < vecStars.size(); ++i)
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
		DIM  dim;
		Star curStar;
		ChooseDim(vecStar, dim, curStar);   //返回起始切分方向和起始节点
		std::vector<Star> rightStar;  //存储位于右子空间的点
		std::vector<Star> leftStar;   //存储位于左子空间的点
		splitPointsToDiffSpace(vecStar, rightStar, leftStar, dim, curStar);

		T = new TreeNode;
		T->star.x = curStar.x;
		T->star.y = curStar.y;
		T->star.z = curStar.z;
		T->star.id = curStar.id;
		T->dim = dim;
		T->left = build_kdtree(leftStar, T->left);    //递归
		T->right = build_kdtree(rightStar, T->right);  //递归
		return T;
	}
}

void updateVecWithStar(vector<StarNode>& vec,const unsigned int& id, const int& maxCount,double dis, double& maxDis)
{
//	cout << "id:" << id << " dis:" << dis << " maxdis:" << maxDis << " vec size:" << vec.size()<<" maxcount:"<<maxCount << endl;
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
void searchNearest(TreeNode* root, Star target, const size_t& count, vector<StarNode>& vec)
{
	if (root == nullptr)
	{
		return;
	}

	double dist = Distance(root->star, target);
	double curMaxDis;
	updateVecWithStar(vec, root->star.id, count, dist, curMaxDis);  //插入坐标点，并获取最大堆的top

	unsigned int dim = root->dim;
	if (dim == X)
	{
		//1.当前点root在左子树，则遍历左子树
		bool left = false;
		if (target.x < root->star.x)
		{
			searchNearest(root->left,target,count,vec);
			left = true;
		}
		else
		{
			searchNearest(root->right, target, count, vec);
		}

		//2.如果最大堆队列未满，则将另一另外一个子树也遍历;
		//3.以当前点画球体，如果其距离小于当前缓存距离的最大值，且当前点在左子树，则遍历右子树
		if (vec.size() < count || fabs(target.x - root->star.x) < curMaxDis)
		{
			if (left)
			{
				searchNearest(root->right, target, count, vec);
			}
			else
			{
				searchNearest(root->left, target, count, vec);
			}
		}
	}
	else if (dim == Y)
	{
		//1.当前点root在左子树，则遍历左子树
		bool left = false;
		if (target.y < root->star.y)
		{
			searchNearest(root->left, target, count, vec);
			left = true;
		}
		else
		{
			searchNearest(root->right, target, count, vec);
		}

		//2.如果最大堆队列未满，则将另一另外一个子树也遍历;
		//3.以当前点画球体，如果其距离小于当前缓存距离的最大值，且当前点在左子树，则遍历右子树
		if (vec.size() < count || fabs(target.y - root->star.y) < curMaxDis)
		{
			if (left)
			{
				searchNearest(root->right, target, count, vec);
			}
			else
			{
				searchNearest(root->left, target, count, vec);
			}
		}
	}
	else if (dim == Z)
	{
		//1.当前点root在左子树，则遍历左子树
		bool left = false;
		if (target.z < root->star.z)
		{
			searchNearest(root->left, target, count, vec);
			left = true;
		}
		else
		{
			searchNearest(root->right, target, count, vec);
		}

		//2.如果最大堆队列未满，则将另一另外一个子树也遍历;
		//3.以当前点画球体，如果其距离小于当前缓存距离的最大值，且当前点在左子树，则遍历右子树
		if (vec.size() < count || fabs(target.z - root->star.z) < curMaxDis)
		{
			if (left)
			{
				searchNearest(root->right, target, count, vec);
			}
			else
			{
				searchNearest(root->left, target, count, vec);
			}
		}
	}
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
		stream >> strTemp;
		ptr->x = stof(strTemp);
        stream >> strTemp;
		ptr->y = stof(strTemp);
        stream >> strTemp;
		ptr->z = stof(strTemp);
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

std::vector<Star> StarMap::find(size_t n, float x, float y, float z)
{
	Star pointStar(x, y, z);
	vector<Star> ret;
	vector<StarNode> vec;
	searchNearest(m_root, pointStar, n, vec);

	for (auto iter : vec)
	{
		ret.push_back(m_vecStars[iter.id - 1]);
	}
	return ret;
}
