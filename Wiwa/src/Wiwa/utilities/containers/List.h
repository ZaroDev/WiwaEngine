#pragma once

typedef unsigned char byte;

namespace Wiwa {
	template<class T>
	class List {
	private:
		byte* m_Data;

		size_t m_Size;
		size_t m_Capacity;

		void ReAlloc(size_t new_capacity);
	public:
		List();
		~List();

		void push_back(const T& value);

		void insert(size_t index, const T& value);

		void erase(size_t index);

		template<class... TArgs>
		void emplace_back(TArgs&&... args);

		void reserve(size_t amount);
		void resize(size_t size);

		T* data() const;

		size_t capacity() const;
		size_t size() const;

		T& at(size_t index);

		T& operator[](size_t index);
		const T& operator[](size_t index) const;
	};

	template<class T>
	inline void List<T>::ReAlloc(size_t new_capacity)
	{
		byte* new_block = new byte[new_capacity * sizeof(T)];

		if (new_capacity < m_Size) {
			m_Size = new_capacity;
		}

		for (size_t i = 0; i < m_Size; i++) {
			//new (new_block + step) T(*(T*)&m_Data[step]);

			// Assignment 100ms faster
			((T*)new_block)[i] = ((T*)m_Data)[i];
		}

		delete[] m_Data;

		m_Data = new_block;

		m_Capacity = new_capacity;
	}

	template<class T>
	inline List<T>::List()
	{
		m_Size = 0;
		m_Capacity = 0;

		m_Data = NULL;
	}

	template<class T>
	inline List<T>::~List()
	{
	}

	template<class T>
	inline void List<T>::push_back(const T& value)
	{
		if (m_Size >= m_Capacity) {
			ReAlloc(m_Capacity + m_Capacity / 2 + 1);
		}

		new (m_Data + m_Size * sizeof(T)) T(value);
		m_Size++;
	}

	template<class T>
	inline void List<T>::insert(size_t index, const T& value)
	{
		if (m_Size >= m_Capacity) {
			ReAlloc(m_Capacity + m_Capacity / 2);
		}

		size_t c_size = m_Size - index;

		for (size_t i = 0; i < c_size; i++) {
			((T*)m_Data)[m_Size - i] = ((T*)m_Data)[m_Size - i - 1];
		}

		((T*)m_Data)[index] = value;
		m_Size++;
	}

	template<class T>
	inline void List<T>::erase(size_t index)
	{
		size_t msize = m_Size - index;

		for (size_t i = 0; i < msize; i++) {
			((T*)m_Data)[index + i] = ((T*)m_Data)[index + i + 1];
		}

		m_Size--;
	}

	template<class T>
	inline void List<T>::reserve(size_t amount)
	{
		if (amount <= m_Capacity) return;

		ReAlloc(amount);
	}

	template<class T>
	inline void List<T>::resize(size_t size)
	{
		ReAlloc(size);
	}

	template<class T>
	inline T* List<T>::data() const
	{
		return (T*)m_Data;
	}

	template<class T>
	inline size_t List<T>::capacity() const
	{
		return m_Capacity;
	}

	template<class T>
	inline size_t List<T>::size() const
	{
		return m_Size;
	}

	template<class T>
	inline T& List<T>::at(size_t index)
	{
		return ((T*)m_Data)[index];
	}

	template<class T>
	inline T& List<T>::operator[](size_t index)
	{
		return ((T*)m_Data)[index];
	}

	template<class T>
	inline const T& List<T>::operator[](size_t index) const
	{
		return ((T*)m_Data)[index];
	}

	template<class T>
	template<class ...TArgs>
	inline void List<T>::emplace_back(TArgs&&...args)
	{
		if (m_Size >= m_Capacity) {
			ReAlloc(m_Capacity + m_Capacity / 2 + 1);
		}

		new (m_Data + m_Size * sizeof(T)) (T)(args...);
		m_Size++;
	}
}