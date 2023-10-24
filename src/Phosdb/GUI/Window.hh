#pragma once

#include <Phosdb.hh>

namespace Phosdb::GUI {
class Window {
public:
	virtual VOID Render() = 0;
};
}