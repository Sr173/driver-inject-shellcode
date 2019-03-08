#pragma once
#include <windows.h>
#include <string>
#include <winternl.h>

class DriverLoader
{
public:
	DriverLoader(std::wstring service_name, std::wstring path = L"");
	NTSTATUS load();
	NTSTATUS unload();
protected:
	HANDLE _hDriver;
private:
	LSTATUS prepare_driver_reg_entry(const std::wstring& svcName, const std::wstring& path);
	std::wstring get_parent(const std::wstring& path);
	std::wstring get_exec_path();

	BOOL up();

	std::wstring service_name;
	std::wstring path;
	decltype(RtlInitUnicodeString)* RtlInitUnicodeString;
	NTSTATUS(NTAPI* NtLoadDriver)(
		IN PUNICODE_STRING path
		);
	NTSTATUS(NTAPI* NtUnloadDriver)(
		IN PUNICODE_STRING path
		); 
};

