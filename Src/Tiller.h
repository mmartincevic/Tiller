#ifndef TILLER_H
#define TILLER_H

#pragma once

#include <map>
#include <string>
#include "tinyxml2.h"
#include "TileMap.h"

#define ENDL "\n"

namespace tile {


	const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
	const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
	const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

	class Tiller
	{
		public:

			static Tiller& Instance() {
				static Tiller instance;
				return instance;
			}

			bool Load(std::string path, std::string tmxName);

			Tileset ParseTileset(tinyxml2::XMLElement* xmlTileset);
			Tileset ParseClosedTileset(tinyxml2::XMLElement* xmlTileset);
			std::vector<std::vector<int>> ParseLayerData(tinyxml2::XMLElement* xmlLayer, TileLayer& layer);
			std::vector<Tile> FormatLayerData(TileGroup tileGroup, TileLayer tileLayer, std::vector<std::vector<int>> rawData);
			void PrintTile(const Tile& tile);

			inline TileMap* Map() { return m_Map; }

		private:
			bool Parse(std::string mapId, std::string);


		private:
			Tiller() = default;  // Default constructor
			~Tiller() = default; // Default destructor

			TileMap* m_Map;
			std::string m_Path; 
	};

};

#endif // TILLER_H