#include "Debug.h"
#include "String.h"

VOID
AssertFail(
	IN const CHAR8 *Cond,
	IN const CHAR8 *File,
	IN INT32        Line
) {
	Print(L"Assertion '%S' failed: %S@%d", Cond, File, Line);
	__debugbreak();
}