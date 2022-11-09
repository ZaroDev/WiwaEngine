#pragma once
#include <string>

namespace Wiwa
{
	class WI_API FileDialogs
	{
	public:
		/// <summary>
		/// Opens a file dialog in open mode
		/// </summary>
		/// <param name="file">filter</param>
		/// <returns>returns the file path that's opened, returns empty when cancelled </returns>
		static std::string OpenFile(const char* filter);
		/// <summary>
		/// Opens a file dialog in save as mode
		/// </summary>
		/// <param name="file">filter</param>
		/// <returns>returns the file path that's opened, returns empty when cancelled </returns>
		static std::string SaveFile(const char* filter);
	};
}