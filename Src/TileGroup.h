#ifndef TILEGROUP_H
#define TILEGROUP_H

#pragma once

#include <string>

namespace tile {

	struct TileGroup
	{
		int ID;
		float Opacity = 1.0f;
		std::string Name;
	};

};

#endif // TILEGROUP_H