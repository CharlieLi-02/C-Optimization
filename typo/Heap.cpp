#include "Heap.h"
#include <stdexcept>

static void swap(Heap::Entry &x, Heap::Entry &y)
{
	auto temp = x;
	x = y;
	y = temp;
}

static size_t parent(size_t i)
{
	return (i-1)/2; 
}

static size_t left(size_t i)
{
	return (2 * i + 1);
}

static size_t right(size_t i)
{
	return (2 * i + 2);
}

static void MinHeapify(size_t i, Heap::Entry *data, size_t heapSize)
{
	size_t l = left(i);
	size_t r = right(i);

	size_t smallest = i;
	if (l < heapSize && data[l].score < data[i].score)
		smallest = l;
	if (r < heapSize && data[r].score < data[smallest].score)
		smallest = r;

	if (smallest != i)
	{
		swap(data[i], data[smallest]);
		MinHeapify(smallest, data, heapSize);
	}
}

Heap::Heap(size_t capacity)
{
	mCount = 0;
	mCapacity = capacity;
	mData = new Entry[mCapacity];
}

Heap::Heap(const Heap &other)
{
	mCount = other.mCount;
	mCapacity = other.mCapacity;
	mData = new Entry[mCapacity];

	for (size_t i = 0; i < other.mCount; ++i)
	{
		mData[i] = other.mData[i];
	}
}

Heap::Heap(Heap &&other)
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

const Heap::Entry &Heap::lookup(size_t index) const
{
	if (index >= mCount)
	{
		throw std::out_of_range("invalid index");
	}
	return mData[index];
}

Heap::Entry Heap::pop()
{
	if (mCount == 0)
	{
		throw std::underflow_error("This heap is empty");
	}

	if (mCount == 1)
	{
		mCount--;
		return mData[0];
	}
	auto e = mData[0];
	mData[0] = mData[mCount - 1];
	mCount--;
	MinHeapify(0, mData, mCount);
	return e;
}

Heap::Entry Heap::pushpop(const std::string &value, float score)
{
	if (mCount == 0)
	{
		throw std::underflow_error("This heap is empty");
	}

	if (mCount == 1)
	{
		auto e = mData[0];
		mData[0] = Entry{ value, score };
		return e;
	}
	auto e = mData[0];
	mData[0] = Entry{ value, score };
	MinHeapify(0, mData, mCount);
	return e;
}
void Heap::push(const std::string &value, float score)
{
	if (mCount == mCapacity)
	{
		throw std::overflow_error("no space to push");
	}

	auto i = mCount;
	mCount++;
	mData[i] = Entry{ value, score };
	
	while (i != 0 && mData[parent(i)].score > mData[i].score)
	{
		swap(mData[i], mData[parent(i)]);
		i = parent(i);
	}
}

const Heap::Entry &Heap::top() const
{
	if (mCount == 0)
	{
		throw std::underflow_error("This heap is empty");
	}

	return mData[0];

}
