#include "Debug.h"
#include "String.h"

VOID
AssertFail(
	const CHAR8 *Cond,
	const CHAR8 *File,
	INT32        Line
) {
	Print(L"Assertion '%S' failed: %S@%d", Cond, File, Line);
	__debugbreak();
}