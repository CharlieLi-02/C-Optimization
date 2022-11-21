#ifndef HELPERS_H
#define HELPERS_H

#include "Star.h"
#include <set>

bool CompareX(const Star&, const Star&);
bool CompareY(const Star&, const Star&);
float Calculate(const Star&, const Star&);

class Heap {
public:
    struct Entry {
        Star star;
        float distance;
    };

private:
    Entry* mData;
    size_t mCapacity;
    size_t mCount;
	
	//将写入堆中的数据ID记录，下次不再写入
	std::set<int> mSetId;
public:
    Heap(size_t capacity);
    Heap(const Heap& other);
    Heap(Heap&& other);
    ~Heap();

    size_t       capacity() const;
    size_t       count() const;
    const Entry& lookup(size_t index) const;
    Entry        pop();
    Entry        pushpop(const Star& star, float distance);
    void         push(const Star& star, float distance);
    const Entry& top() const;
};


#endif
