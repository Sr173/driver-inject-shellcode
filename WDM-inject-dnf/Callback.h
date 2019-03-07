#pragma once
#include <ntddk.h>

VOID load_iimage_notfy(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo);
