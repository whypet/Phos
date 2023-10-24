#pragma once

#include <Phosdb.hh>

namespace Phosdb {
class Util {
public:
	static std::wstring Utf8ToWide(
		const std::string &Utf8
	);
};
}