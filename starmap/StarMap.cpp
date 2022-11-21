#include "StarMap.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>

using std::vector;
using std::string;
using std::cout;
using std::endl;

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

    m_vecX =std::vector<Star>(m_vecStars.begin(), m_vecStars.end());
    m_vecY = std::vector<Star>(m_vecStars.begin(), m_vecStars.end());
    
    // Sort vx in terms of x coordinates. O(nlogn) sorting complexity.
    std::sort(m_vecX.begin(), m_vecX.end(), CompareX);
    // Sort vy in terms of y coordinates. O(nlogn) sorting complexity.
    std::sort(m_vecY.begin(), m_vecY.end(), CompareY);
}		   

StarMap::~StarMap()
{

}

std::vector<Star> StarMap::find(size_t n, float x, float y, float z)
{
    std::vector<Star> vecStars;
    Star star;
    star.x = x;
    star.y = y;
    star.z = z;
    Heap heap(n);
    divideandConquer(star, m_vecX, m_vecY, m_vecStars.size(), heap);
    while (heap.count() > 0)
    {
        vecStars.insert(vecStars.begin(), heap.pop().star);
    }
    
    return vecStars;
}

float StarMap::closest(const Star& star, Heap& heap)
{
    float closestDist = INFINITY; // Give a very big number to find minimum distances.

    for (size_t x = 0; x < m_vecStars.size(); x++) {
        auto dist = Calculate(m_vecStars[x], star);
        insertRes(m_vecStars[x], dist, heap);
        if (dist < closestDist)
        {
            closestDist = dist;
        }  
    }
    return closestDist;
}

// 
float StarMap::divideandConquer(const Star& star, const std::vector<Star>& vecX, const std::vector<Star>& vecY, const size_t size, Heap& heap)
{
    if (size <= 3)
        return closest(star, heap);

    const size_t intermediate = size/ 2;
    Star midPoint = m_vecX[intermediate];

    std::vector<Star> vecLeftPart;
    vecLeftPart.resize(intermediate + 1) ;

    std::vector<Star> vecRightPart;
    vecRightPart.resize(size - intermediate - 1);
 
    int left = 0, right = 0;

    for (size_t i = 0; i < size; i++)
    {
        if (m_vecY[i].x <= midPoint.x)
        {
            if (left < (intermediate + 1))
            {
                vecLeftPart[left++] = m_vecY[i];
            }
        }
        else
        {
            if (right < (size - intermediate - 1))
            {
                vecRightPart[right++] = m_vecY[i];
            }
        }  
    }

    float lengthl = divideandConquer(star, m_vecX, vecLeftPart, intermediate, heap);
    auto vecTempX = std::vector<Star>(m_vecX.begin() + intermediate, m_vecX.end());

    float lengthr = divideandConquer(star, vecTempX, vecRightPart, size - intermediate, heap);

    float smallest = std::min(lengthl, lengthr);

    vector<Star> vecMidlane;
    size_t midlength = 0;
    for (size_t a = 0; a < size; a++)
    {
        if (abs(m_vecY[a].x - midPoint.x) < smallest)
        {
            vecMidlane.push_back(m_vecY[a]);
        }
    }

    for (size_t i = 0; i < vecMidlane.size() && ((star.y - vecMidlane[i].y) < smallest); ++i)
    {
        auto distance = Calculate(vecMidlane[i], star);
        if (distance < smallest)
        {
            smallest = distance;
            insertRes(vecMidlane[i], distance, heap);
        }      
    }

    return smallest;
}

void StarMap::insertRes(const Star& star, float dist, Heap& heap)
{
    if (heap.capacity() == heap.count())
    {
        if (heap.top().distance > dist )
        {
            heap.pushpop(star, dist);
        }
    }
    else
    {
        heap.push(star, dist);
    }
}
