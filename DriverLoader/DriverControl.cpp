#include "DriverControl.h"
#include "../WDM-inject-dnf/ComDef.h"

DriverControl::DriverControl(std::wstring service_name, std::wstring path)
	:DriverLoader(service_name,path)
{
	auto nt_status = load();
	if (!NT_SUCCESS(nt_status)) {
		wchar_t buffer[100];
		wsprintf(buffer, L"¼ÓÔØÇý¶¯Ê§°Ü %X", nt_status);
		MessageBoxW(0, buffer, 0, 0);
	}
}

bool DriverControl::set_black_memory(DWORD64 memory)
{
	DWORD bytes = 0;
	return DeviceIoControl(
		_hDriver,
		IOCTL_SET_BLACK_MEMORY,
		&memory,
		sizeof(memory),
		0,
		0,
		&bytes,
		0
	);
}
