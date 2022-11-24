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

//������������
double Distance(Star a, Star b)
{
	double tmp = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	return sqrt(tmp);
}
void ChooseDim(std::vector<Star>& vecStars, DIM& dim, Star& dimChoose)
{
	/*
	1. ����ÿ��ά��(x,y,z)�ķ���,�Ӿ�����󷽲��ά�ȿ�ʼ�з֣���x����
	2. ����x����Ĳ�������ֵ����Ϊ��ʼ�ڵ㣻
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

	// set the split point value:��ֵ
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
		//��ʼ�зַ���Ϊx����
		for (unsigned int i = 0; i < vecStars.size(); ++i)
		{
			//С�ڵ��ڽڵ�star.x��������ռ�
			if (!equal(star, vecStars[i]) && vecStars[i].x <= star.x)
			{
				leftStar.push_back(vecStars[i]);
			}  //���ڽڵ�star.x�������ҿռ�
			else if (!equal(star, vecStars[i]) && vecStars[i].x > star.x)
			{
				rightStar.push_back(vecStars[i]);
			}
		}
	}
	else if (split == DIM::Y)
	{
		//��ʼ�зַ���Ϊy����
		for (unsigned int i = 0; i < vecStars.size(); ++i)
		{
			//С�ڵ��ڽڵ�star.y��������ռ�
			if (!equal(star, vecStars[i]) && vecStars[i].y <= star.y)
			{
				leftStar.push_back(vecStars[i]);

			}  //���ڽڵ�star.y�������ҿռ�
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
			//С�ڵ��ڽڵ�star.z��������ռ�
			if (!equal(star, vecStars[i]) && vecStars[i].z <= star.z)
			{
				leftStar.push_back(vecStars[i]);

			}  //���ڽڵ�star.z�������ҿռ�
			else if (!equal(star, vecStars[i]) && vecStars[i].z > star.z)
			{
				rightStar.push_back(vecStars[i]);
			}
		}
	}
	return true;
}

//�ݹ鴴��kdtree
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
		ChooseDim(vecStar, dim, curStar);   //������ʼ�зַ������ʼ�ڵ�
		std::vector<Star> rightStar;  //�洢λ�����ӿռ�ĵ�
		std::vector<Star> leftStar;   //�洢λ�����ӿռ�ĵ�
		splitPointsToDiffSpace(vecStar, rightStar, leftStar, dim, curStar);

		T = new TreeNode;
		T->star.x = curStar.x;
		T->star.y = curStar.y;
		T->star.z = curStar.z;
		T->star.id = curStar.id;
		T->dim = dim;
		T->left = build_kdtree(leftStar, T->left);    //�ݹ�
		T->right = build_kdtree(rightStar, T->right);  //�ݹ�
		return T;
	}
}

void updateVecWithStar(vector<StarNode>& vec,const unsigned int& id, const int& maxCount,double dis, double& maxDis)
{
//	cout << "id:" << id << " dis:" << dis << " maxdis:" << maxDis << " vec size:" << vec.size()<<" maxcount:"<<maxCount << endl;
	//������������ ���ֵΪ���һ������������ʱɾ����ģ������ʹ�÷�ʽ
	StarNode cur;
	cur.dis = dis;
	cur.id = id;
	

	//ȥ���ظ�����
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

//�������ڽ���
void searchNearest(TreeNode* root, Star target, const size_t& count, vector<StarNode>& vec)
{
	if (root == nullptr)
	{
		return;
	}

	double dist = Distance(root->star, target);
	double curMaxDis;
	updateVecWithStar(vec, root->star.id, count, dist, curMaxDis);  //��������㣬����ȡ���ѵ�top

	unsigned int dim = root->dim;
	if (dim == X)
	{
		//1.��ǰ��root���������������������
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

		//2.������Ѷ���δ��������һ����һ������Ҳ����;
		//3.�Ե�ǰ�㻭���壬��������С�ڵ�ǰ�����������ֵ���ҵ�ǰ�����������������������
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
		//1.��ǰ��root���������������������
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

		//2.������Ѷ���δ��������һ����һ������Ҳ����;
		//3.�Ե�ǰ�㻭���壬��������С�ڵ�ǰ�����������ֵ���ҵ�ǰ�����������������������
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
		//1.��ǰ��root���������������������
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

		//2.������Ѷ���δ��������һ����һ������Ҳ����;
		//3.�Ե�ǰ�㻭���壬��������С�ڵ�ǰ�����������ֵ���ҵ�ǰ�����������������������
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
			if (f->left != NULL) q.push(f->left);//�����Ů��Ϊ�վͼ�����Ů��һ���ɾ��
			if (f->right != NULL) q.push(f->right);
			delete f;//ɾ����ǰ�ڵ�
			q.pop();//��û�õ�ָ�뵯��
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
