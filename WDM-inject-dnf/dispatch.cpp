#include "dispatch.h"
#include <ntddk.h>

NTSTATUS driver_irp_dispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	_CRT_UNUSED(DeviceObject);
	auto irp_sp = IoGetCurrentIrpStackLocation(Irp);
	auto in_buffer_lenth = irp_sp->Parameters.DeviceIoControl.InputBufferLength;
	auto out_buffer_lenth = irp_sp->Parameters.DeviceIoControl.OutputBufferLength;
	auto in_buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;
	auto out_buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;
	auto nt_status = STATUS_SUCCESS;

	do {
		if (!in_buffer_lenth || !out_buffer_lenth) {
			nt_status = STATUS_INVALID_PARAMETER;
			break;
		}
		switch (irp_sp->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_GET_LAST_THREAD_ID: {
			break;
		}
		case IOCTL_SET_BLACK_MEMORY: {
			break;
		}
		default:
			break;
		}

	} while (false);
	Irp->IoStatus.Status = nt_status;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return nt_status;
}