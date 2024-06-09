#ifndef TILE_H
#define TILE_H

#pragma once

#include <string>

namespace tiller
{
	struct Tile
	{
		unsigned	gID;
		unsigned	lID;
		unsigned	rFlag;
		int			tilesetID;
		int			layerID;
		int			groupID;
		float		opacity;
		int			width;
		int			height;
		int			row;
		int			col;
		std::string imageSrc;
	};
};

#endif // TILE_H