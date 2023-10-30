#include "Debug.h"
#include "String.h"

VOID
AssertFail(
	IN CONST CHAR8 *Cond,
	IN CONST CHAR8 *File,
	IN INT32        Line
) {
	Print(L"Assertion '%S' failed: %S@%d", Cond, File, Line);
	__debugbreak();
}