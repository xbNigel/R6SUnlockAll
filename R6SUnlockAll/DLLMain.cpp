#include <Windows.h>
#include <intrin.h>

extern "C" UINT_PTR GetR14();

UINT_PTR GameBaseAddr = 0;

UINT_PTR(*UnknowCallBack_t)(UINT_PTR rcx, UINT_PTR rdx, UINT_PTR r8) = nullptr;
UINT_PTR UnknowCallBack(UINT_PTR rcx, UINT_PTR rdx, UINT_PTR r8)
{
	UINT_PTR Ret = UnknowCallBack_t(rcx, rdx, r8);
	UINT_PTR RetAddressPtr = (UINT_PTR)_AddressOfReturnAddress();

	if ((UINT_PTR)_ReturnAddress() == (0x27146C9 + GameBaseAddr))
	{
		*(UINT_PTR*)(RetAddressPtr) = ((UINT_PTR)_ReturnAddress() - 0x3DA);

		*(BYTE*)(GetR14() + 0x52) = 0;
		*(BYTE*)(GetR14() + 0x51) = 0;
	}

	return Ret;
}

BOOL WINAPI DllMain(
	_In_ HINSTANCE Instance,
	_In_ DWORD     Reason,
	_In_ LPVOID    Reserved
)
{
	GameBaseAddr = (UINT_PTR)GetModuleHandle(NULL);

	switch (Reason)
	{
	case(DLL_PROCESS_ATTACH):
		UnknowCallBack_t = (decltype(UnknowCallBack_t))(*(UINT_PTR*)(GameBaseAddr + 0x557DAC0));
		*(UINT_PTR*)(GameBaseAddr + 0x557DAC0) = (UINT_PTR)UnknowCallBack;
		break;
	case(DLL_PROCESS_DETACH):
		*(UINT_PTR*)(GameBaseAddr + 0x557DAC0) = (UINT_PTR)UnknowCallBack_t;
		break;
	}

	return TRUE;
}