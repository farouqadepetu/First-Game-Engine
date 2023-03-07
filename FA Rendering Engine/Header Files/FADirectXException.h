#pragma once

#include <wrl.h>
#include <dxgidebug.h>
#include <comdef.h>
#include <string>
#include <sstream>
#include <vector>

inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[1024];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 1024);
	return std::wstring(buffer);
}

class DirectXException
{
public:
	DirectXException(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber);

	std::wstring errorMsg() const;

private:
	HRESULT errorCode;
	std::wstring functionName;
	std::wstring fileName;
	int lineNumber;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> mInfoQueue;
};

//use when calling Direct3D or DXGI function to check if the function failed or not.
#ifndef ThrowIfFailed
#define ThrowIfFailed(x)														\
{																				\
	HRESULT hr = (x);															\
	std::wstring filename(AnsiToWString(__FILE__));								\
	if (FAILED(hr)) { throw DirectXException(hr, L#x, filename, __LINE__); }	\
}
#endif