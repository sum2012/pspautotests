#include "shared.h"

inline void testSet(const char *title, SceUID vtimer, u64 t) {
	int result = sceKernelSetVTimerTime(vtimer, (SceKernelSysClock *)&t);
	if (result == 0) {
		checkpoint(NULL);
		schedf("%s: OK %016x ", title, t);
	} else {
		checkpoint(NULL);
		schedf("%s: Failed (%08x) %016llx ", title, result, t);
	}
	schedfVTimer(vtimer);
}

inline void testSetWide(const char *title, SceUID vtimer, s64 t) {
	s64 updated = sceKernelSetVTimerTimeWide(vtimer, t);
	checkpoint(NULL);
	schedf("%s: %016llx ", title, updated);
	schedfVTimer(vtimer);
}

extern "C" int main(int argc, char *argv[]) {
	SceUID vtimer = sceKernelCreateVTimer("test", NULL);

	// Crashes.
	//sceKernelSetVTimerTime(vtimer, NULL);

	checkpointNext("Objects");
	testSet("  Normal", vtimer, 0x1337);
	testSet("  NULL", 0, 0x1337);
	testSet("  Invalid", 0xDEADBEEF, 0x1337);
	sceKernelDeleteVTimer(vtimer);
	testSet("  Deleted", vtimer, 0x1337);

	vtimer = sceKernelCreateVTimer("test", NULL);
	checkpointNext("Values");
	testSet("  1", vtimer, 1);
	testSet("  0x1234567890123", vtimer, 0x1234567890123ULL);
	testSet("  -1", vtimer, -1);
	testSet("  0", vtimer, 0);

	checkpointNext("Objects (wide)");
	testSetWide("  Normal", vtimer, 0);
	testSetWide("  NULL", 0, 0);
	testSetWide("  Invalid", 0xDEADBEEF, 0);
	sceKernelDeleteVTimer(vtimer);
	testSetWide("  Deleted", vtimer, 0);

	vtimer = sceKernelCreateVTimer("test", NULL);
	checkpointNext("Values (wide)");
	testSetWide("  1", vtimer, 1);
	testSetWide("  0x1234567890123", vtimer, 0x1234567890123ULL);
	testSetWide("  -1", vtimer, -1);
	testSetWide("  0", vtimer, 0);

	return 0;
}