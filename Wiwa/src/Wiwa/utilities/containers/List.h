#pragma once

#include "../../Core.h"

#include "../../simple_types.h"

namespace EM {
	template<class T>
	class EM_API List {
	private:
		T* mData;

		uint32 mSize;
		uint32 mCapacity;

		void ReAlloc(uint32 new_capacity);
	public:
		void PushBack(const T& value);


		uint32 Size() const;

		T& operator[](uint32 index);
		const T& operator[](uint32 index) const;
	};

	template<class T>
	inline void List<T>::ReAlloc(uint32 new_capacity)
	{
		T* new_block = new T[new_capacity];

		if (new_capacity < mSize) {
			mSize = new_capacity;
		}

		for (uint32 i = 0; i < mSize; i++) {
			new_block[i] = mData[i];
		}

		delete[] mData;

		mData = new_block;
	}

	template<class T>
	inline void List<T>::PushBack(const T & value)
	{
		if (mSize >= mCapacity) {
			ReAlloc(mCapacity + mCapacity / 2)
		}

		mData[mSize] = value;
		mSize++;
	}

	template<class T>
	inline uint32 List<T>::Size() const
	{
		return mSize;
	}

	template<class T>
	inline T & List<T>::operator[](uint32 index)
	{
		return mData[index];
	}

	template<class T>
	inline const T & List<T>::operator[](uint32 index) const
	{
		return mData[index];
	}
}