#pragma once

#include <Phosdb.hh>

namespace Phosdb::GUI {
class Window {
public:
	virtual VOID Update() = 0;
	virtual VOID Render() = 0;
};
}