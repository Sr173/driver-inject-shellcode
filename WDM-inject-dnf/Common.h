#pragma once
#include <ntddk.h>
#include "ComDef.h"

LONG SafeSearchString(IN PUNICODE_STRING source, IN PUNICODE_STRING target, IN BOOLEAN CaseInSensitive);
