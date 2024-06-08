#ifndef TILLER_H
#define TILLER_H

#pragma once

#include <map>
#include <string>
#include "tinyxml2.h"
#include "TileMap.h"

#define ENDL "\n"

namespace tile {

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
			inline TileMap* Map() { return m_Map; }

		private:
			bool Parse(std::string mapId, std::string);


		private:
			//MapParser() = default;  // Default constructor
			//~MapParser() = default; // Default destructor
			//std::map<std::string, Map*> m_Maps;

			TileMap* m_Map;
			std::string m_Path; 
	};

};

#endif // TILLER_H