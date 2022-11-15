#include "Dictionary.h"
#include <iostream>
#include <cfloat>
#include <cmath>

//文件一行的长度
#define LINEMAX 256
//float误差
const float EPSINON = 0.00001f;	  

//计算单词的分数
float CaculateWordScore(const std::vector<Point>& points, const std::string& strWord); 
//根据坐标点获取每个单词
std::string GetWordsByPoint(const std::vector<Point>& points);
//float比较 1 第一个大于第二个数 0 第二个大于第一个数
int CompareFloat(float fValue1,float fValue2);

Dictionary::Dictionary(std::istream& stream)
{
	char cBuf[LINEMAX] = { 0 };
	while (stream.getline(cBuf, LINEMAX))
	{
		if (cBuf[0] != 0 && cBuf[0] != '#')
		{
			mWords.push_back(cBuf);
		}
	}
}

Heap Dictionary::correct(const std::vector<Point>& points, size_t maxcount, float cutoff) const
{
	Heap heap(maxcount);
	if (mWords.size()== 0 || points.size()==0)
	{
		return heap;
	}

	std::string strCurWord = GetWordsByPoint(points);  //获取当前点的单词
	for (auto iter : mWords)
	{
		//计算每个点的分数
		float fScore = CaculateWordScore(points,iter);
		if (CompareFloat(fScore, cutoff))
		{
			//堆满了就开始替换最小的数据
			if (heap.count() == heap.capacity())
			{
				if (CompareFloat(fScore, heap.top().score))
				{
					heap.pushpop(iter, fScore);
				}
			}
			else
			{
				heap.push(iter, fScore);
			}
		}
	}
	return heap;
}
std::string GetWordsByPoint(const std::vector<Point>& points)
{
	std::string strWord;
	for (auto iter : points)
	{
		int nPos = 0;
		float fEuclideanDis = FLT_MAX;
		for (int i= 0;i<26;i++)
		{
			//计算每个字母和当前坐标的欧式距离，使用欧式距离最近的字母为当前坐标的字母，如果距离一样，使用QWERTY中首先出现的字母
			float powerD = (iter.x - QWERTY[i].x) *(iter.x - QWERTY[i].x) + (iter.y - QWERTY[i].y) *(iter.y - QWERTY[i].y);
			if (CompareFloat(fEuclideanDis, powerD))
			{
				nPos = i;
				fEuclideanDis = powerD;
			}
		}
		//字母转化
		char c = nPos + 97;
		strWord += c;
	}
	return strWord;
}

float CaculateWordScore(const std::vector<Point>& points, const std ::string& strWord) 
{
	//只计算相同长度单词
	if (points.size() != strWord.size())
	{
		return FLT_MIN;
	}

	float fSum = 0.0; //单词每个字母得分的和
	for (size_t i = 0;i < points.size();i++)
	{
		// 计算s的值 s = 1 / (10*d*d + 1)
		Point pos = QWERTY[strWord[i] - 97];
		float powerD = (pos.x - points[i].x) * (pos.x - points[i].x) + (pos.y - points[i].y) * (pos.y - points[i].y);  //计算 d 的二次方，取3位精度值
		fSum += 1 / (10 * powerD + 1);
	}
	float fScore = fSum / float(points.size());
	return fScore;
}
int CompareFloat(float fValue1, float fValue2)
{
	return (std::abs(float(fValue1 - fValue2)) >= 0.00001f) ? 1 : 0;
}
