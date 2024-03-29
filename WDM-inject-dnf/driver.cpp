#include <ntddk.h>
#include <ntdef.h>
#include "Common.h"
#include "dispatch.h"
#include "Callback.h"

#define DRIVER_NAME  L"\\Device\\CrashDump"
#define DRIVER_DOS_NAME  L"\\DosDevices\\CrashDump"

void driver_unload(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING driver_dos_name;
	RtlInitUnicodeString(&driver_dos_name, DRIVER_DOS_NAME);
	IoDeleteSymbolicLink(&driver_dos_name);
	PsRemoveLoadImageNotifyRoutine(load_iimage_notfy);
	IoDeleteDevice(DriverObject->DeviceObject);
}

extern "C"
NTSTATUS
DriverEntry(
	_In_ PDRIVER_OBJECT  DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	_CRT_UNUSED(RegistryPath);

	UNICODE_STRING driver_name;
	UNICODE_STRING driver_dos_name;
	PDEVICE_OBJECT device_object;

	RtlInitUnicodeString(&driver_name, DRIVER_NAME);
	auto status = IoCreateDevice(
		DriverObject,
		0,
		&driver_name,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		false,
		&device_object
	);
	if (!NT_SUCCESS(status)) {
		DPRINT("[CrashDump]:create device object error 0x%X", status);
		return status;
	}
	DriverObject->DriverUnload = driver_unload;

	DriverObject->MajorFunction[IRP_MJ_CREATE] =
	DriverObject->MajorFunction[IRP_MJ_CLOSE] =
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver_irp_dispatch;

	RtlInitUnicodeString(&driver_dos_name, DRIVER_DOS_NAME);
	status = IoCreateSymbolicLink(
		&driver_dos_name,
		&driver_name
	);
	if (!NT_SUCCESS(status)) {
		DPRINT("[CrashDump]:link device object error 0x%X", status);
		return status;
	}
	status = PsSetLoadImageNotifyRoutine(load_iimage_notfy);
	if (!NT_SUCCESS(status)) {
		DPRINT("[CrashDump]:set image callback error 0x%X", status);
		return status;
	}
	else {
		DPRINT("[CrashDump]:set image callback success");
	}
	DPRINT("[CrashDump]:Load Success");
	return STATUS_SUCCESS;
}