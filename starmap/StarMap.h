#ifndef STARMAP_H
#define STARMAP_H

#include "Star.h"
#include "Helpers.h"
#include <istream>
#include <vector>

class StarMap {
public:
    // ���캯��
    static StarMap* create(std::istream& stream);

private:
	
	//�������ǵ�����
    std::vector<Star> m_vecStars;
	//����Xֵ���������
    std::vector<Star> m_vecX;
	//����Yֵ���������
    std::vector<Star> m_vecY;

public:
    // Constructor & Destructor
    StarMap(std::istream& stream);
    ~StarMap();

    // Required Member Function
    std::vector<Star> find(size_t n, float x, float y, float z);

    //�ݹ����������������С��3��ʱ����
    float closest(const Star& star, Heap& heap);

	//�ݹ���Ҿ������������
    float divideandConquer(const Star& star, const std::vector<Star>& vecX, const std::vector<Star>& vecY,
        const int size, Heap& heap);

	//�������������ݳ��Բ������
    void insertRes(const Star& star, float dist, Heap& heap);

};

#endif
