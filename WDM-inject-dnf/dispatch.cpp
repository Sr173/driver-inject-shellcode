#include "dispatch.h"
#include <ntddk.h>
#include "global_data.hpp"

NTSTATUS driver_irp_dispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	_CRT_UNUSED(DeviceObject);
	_CRT_UNUSED(Irp);

	auto a = GlobalData::get_single_ptr()->black_memory; 
	_CRT_UNUSED(a);
	return STATUS_SUCCESS;
}