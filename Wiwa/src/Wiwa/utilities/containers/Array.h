#pragma once

namespace Wiwa {
	template<class _Ty, size_t _Size>
	class Array {
	private:
		_Ty arr[_Size];
	public:
		_Ty& at(size_t index) { return arr[index]; }
		const _Ty& at(size_t index) const { return arr[index]; }

		_Ty& operator[](size_t index) { return arr[index]; }
		const _Ty& operator[](size_t index) const { return arr[index]; }

		size_t Size() { return _Size; }
	};
}