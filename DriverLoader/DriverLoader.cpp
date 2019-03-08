// DriverLoader.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "DriverLoader.h"
#include "Shlwapi.h"
#include "ntstatus.h"

#pragma comment (lib,"Shlwapi.lib")

DriverLoader::DriverLoader(std::wstring service_name, std::wstring path)
	: service_name(service_name),path(path)
{
	const wchar_t* c = L"GG";
	UNICODE_STRING uc;
	auto ntdll = GetModuleHandle(L"Ntdll.dll");
	RtlInitUnicodeString = (decltype(RtlInitUnicodeString))GetProcAddress(ntdll, "RtlInitUnicodeString");
	NtLoadDriver = (decltype(NtLoadDriver))GetProcAddress(ntdll, "NtLoadDriver");
	NtUnloadDriver = (decltype(NtUnloadDriver))GetProcAddress(ntdll, "NtUnloadDriver");
	if (this->path.empty()) {
		
		this->path = get_exec_path() + L"\\" + service_name + L".sys";
	}
	if (!up()) {
		MessageBox(0, L"提权失败", 0, 0);
	}
	_hDriver = 0;
}

NTSTATUS DriverLoader::load()
{
	std::wstring ser_str = L"\\\\.\\" + service_name;
	_hDriver = CreateFileW(
		ser_str.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL
	);
	if (_hDriver != INVALID_HANDLE_VALUE)
		return STATUS_SUCCESS;
	unload();

	prepare_driver_reg_entry(service_name, path);
	std::wstring regPath = L"\\registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\" + service_name;
	UNICODE_STRING drv_str;
	RtlInitUnicodeString(&drv_str, regPath.c_str());
	auto status = NtLoadDriver(&drv_str);
	if (!NT_SUCCESS(status)) {
		printf("faild to load driver %X", status);
		return status;
	}
	_hDriver = CreateFileW(
		ser_str.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL
	);
	if (_hDriver != INVALID_HANDLE_VALUE)
		return STATUS_SUCCESS;
	else
		printf("faild to open driver %X\n", GetLastError());

	return STATUS_OPEN_FAILED;
}

NTSTATUS DriverLoader::unload()
{
	UNICODE_STRING unload_str = { 0 };

	std::wstring regPath = L"\\registry\\machine\\SYSTEM\\CurrentControlSet\\Services\\" + service_name;
	RtlInitUnicodeString(&unload_str, regPath.c_str());
	// Remove previously loaded instance, if any
	NTSTATUS status = NtUnloadDriver(&unload_str);
	SHDeleteKeyW(HKEY_LOCAL_MACHINE, (L"SYSTEM\\CurrentControlSet\\Services\\" + service_name).c_str());

	return status;
}

LSTATUS DriverLoader::prepare_driver_reg_entry(const std::wstring& svcName, const std::wstring& path)
{
	HKEY svcRoot, svcKey;
	DWORD dwType = 1;
	LSTATUS status = 0;
	WCHAR wszLocalPath[MAX_PATH] = { 0 };

	swprintf_s(wszLocalPath, ARRAYSIZE(wszLocalPath), L"\\??\\%s", path.c_str());

	status = RegOpenKeyW(HKEY_LOCAL_MACHINE, L"system\\CurrentControlSet\\Services", &svcRoot);
	if (status)
		return status;

	status = RegCreateKeyW(svcRoot, svcName.c_str(), &svcKey);
	if (status)
		return status;

	status = RegSetValueExW(
		svcKey, L"ImagePath", 0, REG_SZ,
		reinterpret_cast<const BYTE*>(wszLocalPath),
		static_cast<DWORD>(sizeof(WCHAR) * (wcslen(wszLocalPath) + 1))
	);

	if (status)
		return status;

	return RegSetValueExW(svcKey, L"Type", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&dwType), sizeof(dwType));
}

std::wstring DriverLoader::get_parent(const std::wstring& path)
{
	if (path.empty())
		return path;

	auto idx = path.rfind(L'\\');
	if (idx == path.npos)
		idx = path.rfind(L'/');

	if (idx != path.npos)
		return path.substr(0, idx);
	else
		return path;
}

std::wstring DriverLoader::get_exec_path()
{
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(0, path, MAX_PATH);
	return get_parent(path);
}

BOOL DriverLoader::up()
{
	HANDLE     hToken, hProcess;
	TOKEN_PRIVILEGES tp;
	const wchar_t* pSEDEBUG = L"SeLoadDriverPrivilege";
	hProcess = GetCurrentProcess();

	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		printf("OpenProcessToken");      return FALSE;
	}
	if (!LookupPrivilegeValue(NULL, pSEDEBUG, &tp.Privileges[0].Luid))
	{
		printf("LookupPrivilegeValue");  //printf("无法找到指定权限:%s",pSEDEBUG);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, NULL, NULL) != 0)
		return TRUE;
	else
	{
		printf("AdjustTokenPrivileges");
		return FALSE;
	}
	return true;
}