#pragma once

namespace Wiwa {
	template<class _Ty, size_t _Size>
	class Array {
	private:
		_Ty arr[_Size];
	public:
		Array() = default;
		inline Array(const Array<_Ty, _Size>& other) { for (int i = 0; i < _Size; i++) arr[i] = other[i]; }
		
		inline constexpr Array(const _Ty other[_Size]){ for (int i = 0; i < _Size; i++) arr[i] = other[i]; }
		inline constexpr Array(_Ty other[_Size]){ for (int i = 0; i < _Size; i++) arr[i] = other[i]; }

		inline _Ty& at(size_t index) { return arr[index]; }
		inline const _Ty& at(size_t index) const { return arr[index]; }

		inline _Ty& operator[](size_t index) { return arr[index]; }
		inline const _Ty& operator[](size_t index) const { return arr[index]; }

		inline constexpr size_t Size() const { return _Size; }
	};
}