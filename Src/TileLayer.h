#ifndef TILELAYER_H
#define TILELAYER_H

#pragma once

#include <string>

namespace tiller {

	struct TileLayer
	{
		int ID;
		std::string Name;
		int Width;
		int Height;
		int OffsetX;
		int OffsetY;
	};

};

#endif // TILELAYER_H