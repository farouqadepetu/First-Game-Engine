#pragma once

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
	WCHAR buffer[2048];
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


/**@brief Creates an IDXGIInfoQueue object.
*/
inline void CreateInfoQueue(Microsoft::WRL::ComPtr<IDXGIInfoQueue>& infoQueue)
{
#if defined(_DEBUG) || defined(DEBUG)										
	//define function signature
	typedef HRESULT(WINAPI* dxgiDebugInterface)(REFIID, void**);

	//Get a handle to the dll file
	HMODULE dxgiDebugHandle;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, L"Dxgidebug.dll", &dxgiDebugHandle);

	//get the address of the function DXGIGetDebugInterface in the dll file
	dxgiDebugInterface DXGIGetDebugInterface = (dxgiDebugInterface)GetProcAddress(dxgiDebugHandle, "DXGIGetDebugInterface");
	if (DXGIGetDebugInterface == nullptr)
	{
		exit(-1);
	}

	//create a DXGIInfoQueue object.
	DXGIGetDebugInterface(IID_PPV_ARGS(&infoQueue));
#endif
}



/**@brief Returns the error message associated with the error code.
*/
inline std::wstring ErrorMessage(HRESULT errorCode, const std::wstring& functionName, const std::wstring& filename, int lineNumber,
	const Microsoft::WRL::ComPtr<IDXGIInfoQueue>& infoQueue)
{
	//the _com_error class lets us retrieve the error message associated with the HRESULT error code
	_com_error error(errorCode);
	std::wstring msg = error.ErrorMessage();

	//Get the hex value of the error code
	std::stringstream ss;
	ss << std::hex << errorCode;
	std::wstring hrHex{ AnsiToWString(ss.str()) };

	std::wstring eCode(std::to_wstring(errorCode));


	std::wstring errorMessage{ L"File Name: " + filename + L"\n\n" + L"Function Name: " + functionName + L"\n\n" +
		L"Line Number: " + std::to_wstring(lineNumber) + L"\n\n" + L"Error Code: " + eCode +
		L"(0x" + hrHex + L")" + L"\n\n" + L"Error Code Description: " + msg };

	std::vector<std::wstring> messages;

	if (infoQueue != nullptr)
	{
		//Get the number of messages in the queue.
		UINT64 numOfMessages = infoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

		for (UINT64 i = 0; i < numOfMessages; ++i)
		{
			//Get the length of the current message.
			SIZE_T messageLength{ 0 };
			infoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);

			//Allocate enough memory to store the message.
			std::unique_ptr<unsigned char[]> bytes = std::make_unique<unsigned char[]>(messageLength);
			DXGI_INFO_QUEUE_MESSAGE* pMsg = (DXGI_INFO_QUEUE_MESSAGE*)bytes.get();

			//Retrieve the message. It will be stored in pMsg.
			infoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMsg, &messageLength);

			//Store the message.
			std::string tempMessage{ pMsg->pDescription };
			messages.emplace_back(AnsiToWString(tempMessage));
		}
	}

	for (int i = 0; i < messages.size(); ++i)
	{
		errorMessage += L"\n";
		errorMessage += messages[i];
	}

	return errorMessage;
}

/**@brief Use when calling a Direct3D or DXGI function that returns a HRESULT value to check if the function failed or not.
*/
#ifndef ExitIfFailed
#define ExitIfFailed(x)																				\
{																									\
	HRESULT hr = (x);																				\
	if (FAILED(hr))																					\
	{																								\
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> infoQueue;											\
		CreateInfoQueue(infoQueue);																	\
		std::wstring filename(AnsiToWString(__FILE__));												\
		std::wstring errMsg = ErrorMessage(hr, L#x, filename, __LINE__,	infoQueue);					\
		MessageBox(nullptr, errMsg.c_str(), L"DirectX Error", MB_OK);								\
		exit(-1);																					\
	}																								\
}
#endif