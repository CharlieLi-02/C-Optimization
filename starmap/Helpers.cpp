#include "Helpers.h"
#include <cmath>
#include <stdexcept>
#include <cstddef>

// Space to implement helper class member functions.

bool CompareX(const Star& fst, const Star& snd)
{
	return fst.x < snd.x;
}

bool CompareY(const Star& fst, const Star& snd)
{
	return fst.y < snd.y;
}

float Calculate(const Star& fst, const Star& snd)
{
	return sqrt(std::pow(fst.x - snd.x, 2) + pow(fst.y - snd.y, 2) + pow(fst.z - snd.z, 2));
}


static void swap(Heap::Entry& x, Heap::Entry& y)
{
	auto temp = x;
	x = y;
	y = temp;
}

//计算父节点的索引
static size_t parent(size_t i)
{
	return (i - 1) / 2;
}

//计算左孩子的索引
static size_t left(size_t i)
{
	return (2 * i + 1);
}

//计算右孩子的索引
static size_t right(size_t i)
{
	return (2 * i + 2);
}


static void MaxHeapify(size_t i, Heap::Entry* data, size_t heapSize)
{
	/*递归最大堆化*/
	size_t l = left(i);
	size_t r = right(i);

	size_t biggest = i;
	if (l < heapSize && data[l].distance > data[i].distance)
		biggest = l;
	if (r < heapSize && data[r].distance > data[biggest].distance)
		biggest = r;

	//当前索引和smallest不一致时，进行交换及递归
	if (biggest != i)
	{
		swap(data[i], data[biggest]);
		MaxHeapify(biggest, data, heapSize);
	}
}

Heap::Heap(size_t capacity)
{
	mCount = 0;
	mCapacity = capacity;
	mData = new Entry[mCapacity];
}

Heap::Heap(const Heap& other)
{
	mCount = other.mCount;
	mCapacity = other.mCapacity;
	mData = new Entry[mCapacity];

	//数据深拷贝
	for (size_t i = 0; i < other.mCount; ++i)
	{
		mData[i] = other.mData[i];
	}
}

Heap::Heap(Heap&& other)
{
	mCount = other.mCount;
	mCapacity = other.mCapacity;
	mData = other.mData;
	other.mCount = 0;
	other.mCapacity = 0;
	other.mData = nullptr;
}

Heap::~Heap()
{
	if (mData != nullptr)
	{
		delete[] mData;
	}
}

size_t Heap::capacity() const
{
	return mCapacity;
}

size_t Heap::count() const
{
	return mCount;
}

const Heap::Entry& Heap::lookup(size_t index) const
{
	//如果不在堆的size内 抛出异常
	if (index >= mCount)
	{
		throw std::out_of_range("invalid index");
	}
	return mData[index];
}

Heap::Entry Heap::pop()
{
	//为0的时候 抛出异常
	if (mCount == 0)
	{
		throw std::underflow_error("This heap is empty");
	}

	//为1的时候 直接返回下标为0的，并且将mCount调整
	if (mCount == 1)
	{
		mCount--;
		return mData[0];
	}
	auto e = mData[0];
	mData[0] = mData[mCount - 1];
	mCount--;
	MaxHeapify(0, mData, mCount);
	return e;
}

Heap::Entry Heap::pushpop(const Star& star, float distance)
{
	//为0的时候 抛出异常
	if (mCount == 0 )
	{
		throw std::underflow_error("This heap is empty");
	}
	if (mSetId.count(star.id) > 0)
	{
		return mData[0];
	}
	mSetId.insert(star.id);
	//为1的时候 直接替换即可
	if (mCount == 1)
	{
		auto e = mData[0];
		mData[0] = Entry{ star, distance };
		return e;
	}
	auto e = mData[0];
	mData[0] = Entry{ star, distance };
	MaxHeapify(0, mData, mCount);
	return e;
}
void Heap::push(const Star& star, float distance)
{
	if (mCount == mCapacity)
	{
		throw std::overflow_error("no space to push");
	}
	if (mSetId.count(star.id) > 0)
	{
		return;
	}
	mSetId.insert(star.id);

	auto i = mCount;
	mCount++;
	mData[i] = Entry{ star, distance };

	//自下而上的进行交换数据
	while (i != 0 && mData[parent(i)].distance < mData[i].distance)
	{
		swap(mData[i], mData[parent(i)]);
		i = parent(i);
	}
}

const Heap::Entry& Heap::top() const
{
	if (mCount == 0)
	{
		throw std::underflow_error("This heap is empty");
	}

	return mData[0];

}
