#include "FADirectXException.h"

DirectXException::DirectXException(HRESULT hr, const std::wstring& functionName, const std::wstring& fileName, int lineNumber) :
	errorCode{ hr }, functionName{ functionName }, fileName{ fileName }, lineNumber{ lineNumber }
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
	DXGIGetDebugInterface(IID_PPV_ARGS(&mInfoQueue));
#endif
}

std::wstring DirectXException::ErrorMsg() const
{
	//the _com_error class lets us retrieve the error message associated with the HRESULT error code
	_com_error error(errorCode);
	std::wstring msg = error.ErrorMessage();

	//Get the hex value of the error code
	std::stringstream ss;
	ss << std::hex << errorCode;
	std::wstring hrHex{ AnsiToWString(ss.str()) };

	std::wstring eCode(std::to_wstring(errorCode));


	std::wstring errorMessage{ L"File Name: " + fileName + L"\n\n" + L"Function Name: " + functionName + L"\n\n" +
		L"Line Number: " + std::to_wstring(lineNumber) + L"\n\n" + L"Error Code: " + eCode +
		L"(0x" + hrHex + L")" + L"\n\n" + L"Error Code Description: " + msg };

	std::vector<std::wstring> messages;

	if (mInfoQueue != nullptr)
	{
		//Get the number of messages in the queue.
		UINT64 numOfMessages = mInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

		for (UINT64 i = 0; i < numOfMessages; ++i)
		{
			//Get the length of the current message.
			SIZE_T messageLength{ 0 };
			mInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength);

			//Allocate enough memory to store the message.
			std::unique_ptr<unsigned char[]> bytes = std::make_unique<unsigned char[]>(messageLength);
			DXGI_INFO_QUEUE_MESSAGE* pMsg = (DXGI_INFO_QUEUE_MESSAGE*)bytes.get();

			//Retrieve the message. It will be stored in pMsg.
			mInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMsg, &messageLength);

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
