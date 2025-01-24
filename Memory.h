#pragma once
#include "framework.h"

namespace Memory
{
	uintptr_t GetAddress(uintptr_t Offset)
	{
		return InSDKUtils::GetImageBase() + Offset;
	}

	MH_STATUS CreateHook(uintptr_t pTarget, LPVOID pDetour, LPVOID* ppOriginal = nullptr)
	{
		MH_CreateHook((LPVOID)pTarget, pDetour, ppOriginal);
		return MH_EnableHook((LPVOID)pTarget);
	}

	void VirtualHook(void* Object, int Index, void* Detour, void** OG = nullptr)
	{
		auto vft = *(void***)Object;
		if (!vft || !vft[Index])
		{
			return;
		}

		if (OG)
			*OG = vft[Index];

		DWORD oldProtection;

		VirtualProtect(&vft[Index], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);
		vft[Index] = Detour;
		VirtualProtect(&vft[Index], 8, oldProtection, NULL);
	}

	static void* nullptrForHook = nullptr;

	template <typename T = void*>
	static void ExecHook(UFunction* func, void* detour, T& og = nullptrForHook)
	{
		if (!func)
			return;

		if (!is_same_v<T, void*>)
			og = (T)func->ExecFunction;

		func->ExecFunction = reinterpret_cast<UFunction::FNativeFuncPtr>(detour);
	}
}