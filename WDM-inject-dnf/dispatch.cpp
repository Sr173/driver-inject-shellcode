#include "dispatch.h"
#include <ntddk.h>
#include "Common.h"
#include "Inject.h"

NTSTATUS driver_irp_dispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	_CRT_UNUSED(DeviceObject);
	auto irp_sp = IoGetCurrentIrpStackLocation(Irp);
	auto in_buffer_lenth = irp_sp->Parameters.DeviceIoControl.InputBufferLength;
	auto out_buffer_lenth = irp_sp->Parameters.DeviceIoControl.OutputBufferLength;
	auto in_buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;
	auto out_buffer = (PUCHAR)Irp->AssociatedIrp.SystemBuffer;
	_CRT_UNUSED(out_buffer);
	_CRT_UNUSED(out_buffer_lenth);

	Irp->IoStatus.Status = STATUS_SUCCESS;
	NTSTATUS nt_status;
	do {
		switch (irp_sp->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_GET_LAST_THREAD_ID: {
			break;
		}
		case IOCTL_SET_BLACK_MEMORY: {
			if (in_buffer_lenth != sizeof(void*)) {
				DPRINT("[CrashDump]:input size invaild");
				Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
				break;
			}
			void* temp;
			RtlCopyMemory(&temp, in_buffer, sizeof(temp));
			set_black_memory(temp);
			DPRINT("[CrashDump]:set buffer %X", temp);
			break;
		}
		default:
			break;
		}

	} while (false);
	nt_status = Irp->IoStatus.Status;
	Irp->IoStatus.Information = 0;
	DPRINT("[CrashDump]:dispatch 0x%X", nt_status);
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return nt_status;
}