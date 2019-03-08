#include "DriverControl.h"

int main() {
	DriverControl d(L"CrashDump");
	d.set_black_memory(0x41000);
	system("pause");
}