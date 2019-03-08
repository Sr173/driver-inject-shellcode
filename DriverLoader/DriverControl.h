#pragma once
#include "DriverLoader.h"

class DriverControl : public DriverLoader
{
public:
	DriverControl(std::wstring service_name, std::wstring path = L"");
	bool set_black_memory(DWORD64 memory);
};

