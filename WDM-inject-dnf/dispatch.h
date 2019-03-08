#pragma once
#include <ntddk.h>

NTSTATUS driver_irp_dispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp);