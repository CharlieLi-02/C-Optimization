#include "Dictionary.h"
#include <iostream>
#include <cfloat>
#include <cmath>


#define LINEMAX 256

const float EPSINON = 0.00001f;


float CaculateWordScore(const std::vector<Point>& points, const std::string& strWord);

std::string GetWordsByPoint(const std::vector<Point>& points);

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
    //std::cout << "cutoff score is " << cutoff << std::endl;
    Heap heap(maxcount);
    if (mWords.size()== 0 || points.size()==0)
    {
        return heap;
    }

    std::string strCurWord = GetWordsByPoint(points);
    for (auto iter : mWords)
    {
        if(iter.length() == points.size()){
            float fScore = CaculateWordScore(points,iter);
            //std::cout << "words score for " << iter << " is " << fScore << std::endl;
            if (fScore > cutoff + EPSINON)
            {
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
            float powerD = (iter.x - QWERTY[i].x) *(iter.x - QWERTY[i].x) + (iter.y - QWERTY[i].y) *(iter.y - QWERTY[i].y);
            if (CompareFloat(fEuclideanDis, powerD))
            {
                nPos = i;
                fEuclideanDis = powerD;
            }
        }
        char c = nPos + 97;
        strWord += c;
    }
    return strWord;
}

float CaculateWordScore(const std::vector<Point>& points, const std ::string& strWord)
{
    if (points.size() != strWord.size())
    {
        return FLT_MIN;
    }

    float fSum = 0.0;
    for (size_t i = 0;i < points.size();i++)
    {
        Point pos = QWERTY[strWord[i] - 97];
        float powerD = (pos.x - points[i].x) * (pos.x - points[i].x) + (pos.y - points[i].y) * (pos.y - points[i].y);
        fSum += 1 / (10 * powerD + 1);
    }
    float fScore = fSum / float(points.size());
    return fScore;
}
int CompareFloat(float fValue1, float fValue2)
{
    return (std::abs(float(fValue1 - fValue2)) >= 0.00001f) ? 1 : 0;
}
