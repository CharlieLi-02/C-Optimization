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

#define MAXDIS 10.0 //���귶Χ��[-1,1]����������벻�ᳬ��3

//������������
double Distance(Star a, Star b)
{
	double tmp = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
	return sqrt(tmp);
}
void ChooseSplit(std::vector<Star>& vecStars, DIM& dim, Star& dimChoose)
{
	/*
	1. ����ÿ��ά��(x,y,z)�ķ���,�Ӿ�����󷽲��ά�ȿ�ʼ�з֣���x����
	2. ����x����Ĳ�������ֵ����Ϊ��ʼ�ڵ㣻
	*/

	double tmp1, tmp2;
	tmp1 = tmp2 = 0;
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += 1.0 / (double)vecStars.size() * vecStars[i].x * vecStars[i].x;
		tmp2 += 1.0 / (double)vecStars.size() * vecStars[i].x;
	}
	double v1 = tmp1 - tmp2 * tmp2;  // compute variance on the x dimension

	tmp1 = tmp2 = 0;
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += 1.0 / (double)vecStars.size() * vecStars[i].y * vecStars[i].y;
		tmp2 += 1.0 / (double)vecStars.size() * vecStars[i].y;
	}
	double v2 = tmp1 - tmp2 * tmp2;  // compute variance on the y dimension

	tmp1 = tmp2 = 0;
	for (int i = 0; i < vecStars.size(); ++i)
	{
		tmp1 += 1.0 / (double)vecStars.size() * vecStars[i].z * vecStars[i].z;
		tmp2 += 1.0 / (double)vecStars.size() * vecStars[i].z;
	}
	double v3 = tmp1 - tmp2 * tmp2;

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
		DIM  split;
		Star curStar;
		ChooseSplit(vecStar, split, curStar);   //������ʼ�зַ������ʼ�ڵ�
		std::vector<Star> rightStar;  //�洢λ�����ӿռ�ĵ�
		std::vector<Star> leftStar;   //�洢λ�����ӿռ�ĵ�
		splitPointsToDiffSpace(vecStar, rightStar, leftStar, split, curStar);

		T = new TreeNode;
		T->star.x = curStar.x;
		T->star.y = curStar.y;
		T->star.z = curStar.z;
		T->star.id = curStar.id;
		T->dim = split;
		T->left = build_kdtree(leftStar, T->left);    //�ݹ�
		T->right = build_kdtree(rightStar, T->right);  //�ݹ�
	}
	return T;
}
void updateVecWithStar(vector<StarNode>& vec,const unsigned int& id, const int& maxCount,double dis, double& maxDis)
{
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
vector<unsigned int> searchNearest(TreeNode* root, Star target,const size_t& count)
{
	vector<StarNode> result;

	// 1. ���m_root�ǿյģ�����distΪ����󷵻�
	// 2. ��������ֱ��Ҷ�ӽ��
	stack<TreeNode*> search_path;
	TreeNode*        pSearch = root;
	Star			 nearest;
	double           dist;
	double			 curMaxDis;
	while (pSearch != nullptr)
	{
		// pSearch���뵽search_path��;
		search_path.push(pSearch);

		if (pSearch->dim == DIM::X)
		{
			if (target.x <= pSearch->star.x) /* ���С�ھͽ��������� */
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
			if (target.y <= pSearch->star.y) /* ���С�ھͽ��������� */
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
			if (target.z <= pSearch->star.z) /* ���С�ھͽ��������� */
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

	//ȡ��search_path���һ������nearest
	nearest.x = search_path.top()->star.x;
	nearest.y = search_path.top()->star.y;
	nearest.z = search_path.top()->star.z;
	nearest.id = search_path.top()->star.id;
	search_path.pop();

	dist = Distance(nearest, target);

	updateVecWithStar(result, nearest.id, count, dist, curMaxDis);

	// 3. ��������·�����ؼ���:��ǰ����ʱ��Ϊ�˿����ҵ����ݵ��ڵ�ĸ��ڵ㣬������ջ�����߹���·����
	TreeNode* pBack;
	while (!search_path.empty())
	{
		//ȡ��search_path���һ����㸳��pBack
		pBack = search_path.top();
		search_path.pop();

		dist = Distance(pBack->star, target);
		updateVecWithStar(result, pBack->star.id, count, dist, curMaxDis);

		//�����ڵ���ӿռ�������
		unsigned int s = pBack->dim;
		if (s == 1)
		{
			/* �����targetΪ���ĵ�Բ������򣩣��뾶Ϊdist��Բ��ָƽ���ཻ����ô��Ҫ������һ�ߵ��ӿռ�ȥ���еݹ�������ѭ�����ǲ��ϱ��������ӽڵ� */
			if (fabs(pBack->star.x - target.x) < curMaxDis)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{

					if (target.x <= pSearch->star.x) /*���targetλ��pBack�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����*/
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left; /* ���targetλ��pBack�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����*/
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
			/* �����targetΪ���ĵ�Բ������򣩣��뾶Ϊdist��Բ��ָƽ���ཻ����ô��Ҫ������һ�ߵ��ӿռ�ȥ�ݹ�������ѭ�����ǲ��ϱ��������ӽڵ� */
			if (fabs(pBack->star.y - target.y) < curMaxDis)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{

					if (target.x <= pSearch->star.x) /*���targetλ��pBack�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����*/
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left; /* ���targetλ��pBack�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ���� */
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
			/* �����targetΪ���ĵ�Բ������򣩣��뾶Ϊdist��Բ��ָƽ���ཻ����ô��Ҫ������һ�ߵ��ӿռ�ȥ�ݹ�������ѭ�����ǲ��ϱ��������ӽڵ� */
			if (fabs(pBack->star.z - target.z) < curMaxDis)
			{
				pSearch = pBack;

				while (pSearch != nullptr)
				{
					if (target.z <= pSearch->star.z) /*���targetλ��pBack�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����*/
					{
						pSearch = pSearch->right;
					}
					else
					{
						pSearch = pSearch->left; /* ���targetλ��pBack�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����*/
					}
					if (pSearch != nullptr)
					{
						search_path.push(pSearch);
					}
				}
			}
		}
	}

	//���������Ӧ��id
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

std::vector<Star> StarMap::find(size_t n, float x, float y, float z)
{
	Star pointStar(x, y, z);
	vector<Star> ret;
	vector<unsigned int> vec = searchNearest(m_root, pointStar, n);
	for (auto iter : vec)
	{
		ret.push_back(m_vecStars[iter-1]);
	}
	return ret;
}
