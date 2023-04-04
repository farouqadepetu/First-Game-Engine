#pragma once

/** @file FADirectXExcepetion.h
*	@brief File has class DirectXExcepetion, ANsiToWString function and ThrowIfFailed macro function.
*/

#include <wrl.h>
#include <dxgidebug.h>
#include <comdef.h>
#include <string>
#include <sstream>
#include <vector>

/**@brief Converts a ansi string to a unicode string.
* 
* @param[in] str The string to convert.
*/
inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[1024];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 1024);
	return std::wstring(buffer);
}

/** @class DirectXException ""
*	@brief A class for handling Direct3D and DXGI errors from functions that return a HRESULT value.
*/
class DirectXException
{
public:

	/**@brief Constructs a DirectXException object.
	*
	* @param[in] hr The HRESULT value of a function.
	* @param[in] functionName The name of the function.
	* @param[in] fileName The name of the file where the function was called.
	* @param[in] lineNumber The line number of the function call.
	*/
	DirectXException(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber);

	/**@brief Returns a message describing the error.
	*/
	std::wstring ErrorMsg() const;

private:
	HRESULT errorCode;
	std::wstring functionName;
	std::wstring fileName;
	int lineNumber;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> mInfoQueue;
};

/**@brief Use when calling a Direct3D or DXGI function that returns a HRESULT value to check if the function failed or not.
*/
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)														\
{																				\
	HRESULT hr = (x);															\
	std::wstring filename(AnsiToWString(__FILE__));								\
	if (FAILED(hr)) { throw DirectXException(hr, L#x, filename, __LINE__); }	\
}
#endif