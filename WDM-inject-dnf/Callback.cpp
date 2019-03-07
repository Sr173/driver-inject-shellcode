#include "Callback.h"
#include "Common.h"

HANDLE last_dnf_id = 0;

VOID load_iimage_notfy(
	PUNICODE_STRING FullImageName,
	HANDLE ProcessId,
	PIMAGE_INFO ImageInfo
) {
	_CRT_UNUSED(FullImageName);
	_CRT_UNUSED(ProcessId);
	_CRT_UNUSED(ImageInfo);
	UNICODE_STRING dnf_str;
	RtlInitUnicodeString(&dnf_str, L"dnf.exe");
	UNICODE_STRING user32_str;
	RtlInitUnicodeString(&user32_str, L"user32.dll");
	auto result_index = SafeSearchString(FullImageName, &dnf_str, TRUE);
	if (result_index != -1) {
		last_dnf_id = PsGetCurrentProcessId();
		DPRINT("[CrashDump]:dnf process id [%d]", last_dnf_id);
	}
	if (PsGetCurrentProcessId() == last_dnf_id && SafeSearchString(FullImageName, &user32_str, TRUE) != -1) {
		DPRINT("[CrashDump]:pre inject id [%d]", last_dnf_id);
	}
}