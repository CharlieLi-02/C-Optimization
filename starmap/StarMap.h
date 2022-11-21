#ifndef STARMAP_H
#define STARMAP_H

#include "Star.h"
#include "Helpers.h"
#include <istream>
#include <vector>

class StarMap {
public:
    // 构造函数
    static StarMap* create(std::istream& stream);

private:
	
	//输入星星的坐标
    std::vector<Star> m_vecStars;
	//根据X值排序的数据
    std::vector<Star> m_vecX;
	//根据Y值排序的数据
    std::vector<Star> m_vecY;

public:
    // Constructor & Destructor
    StarMap(std::istream& stream);
    ~StarMap();

    // Required Member Function
    std::vector<Star> find(size_t n, float x, float y, float z);

    //递归结束处理，集合数量小于3的时候处理
    float closest(const Star& star, Heap& heap);

	//递归查找距离最近的坐标
    float divideandConquer(const Star& star, const std::vector<Star>& vecX, const std::vector<Star>& vecY,
        const int size, Heap& heap);

	//满足条件的数据尝试插入堆中
    void insertRes(const Star& star, float dist, Heap& heap);

};

#endif
