#ifndef TILESET_H
#define TILESET_H

#pragma once

#include <map>
#include <string>
#include "tinyxml2.h"

namespace tile {

	struct Tileset
	{
		int ID;
		int ImgWidth;
		int ImgHeight;
		int TileWidth;
		int TileHeight;
		int TileCount;
		int Columns;
		int LastID;
		int RowCount;
		std::string ImgSource;
		std::string Name;

	};
};

#endif // TILESET_H