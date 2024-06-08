#ifndef TILEMAP_H
#define TILEMAP_H

#pragma once

#include <map>
#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include "tinyxml2.h"
#include "TileGroup.h"
#include "TileLayer.h"
#include "Tileset.h"

namespace tile {

	class TileMap
	{
		public:
			TileMap(std::string name, std::string source, std::string orientation, 
				int width, int height, int tilewidth, int tileheight) : m_Name(name), m_Source(source),
				m_Orientation(orientation), m_Width(width), m_Height(height), m_TileWidth(tilewidth), m_TileHeight(tileheight)
			{}

			inline void AddGroup(TileGroup group) { m_Groups.insert({ group.ID, group }); }
			inline void AddLayer(TileLayer layer) { m_Layers.insert({ layer.ID, layer }); }
			inline void AddTileset(Tileset tileset) { m_Tilesets.insert({ tileset.ID, tileset }); }

			std::map<int, Tileset>& TileSet() { return m_Tilesets; }

			void AddData(int groupId, int layerId, std::vector<std::vector<int>> data)
			{
				m_MAP[groupId][layerId] = data;
			}

			void PrintMap() const {
				for (const auto& groupPair : m_MAP) {
					std::cout << "Group ID: " << groupPair.first << std::endl;
					for (const auto& layerPair : groupPair.second) {
						std::cout << "  Layer ID: " << layerPair.first << std::endl;
						for (const auto& row : layerPair.second) {
							for (int val : row) {
								std::cout << val << " ";
							}
							std::cout << std::endl;
						}
					}
				}
			}


			std::optional<std::vector<std::vector<int>>> Layer(int groupId, int layerId) const {
				auto groupIt = m_MAP.find(groupId);
				if (groupIt != m_MAP.end()) {
					auto layerIt = groupIt->second.find(layerId);
					if (layerIt != groupIt->second.end()) {
						return layerIt->second;
					}
				}
				return std::nullopt;

			}

			std::optional<std::map<int, std::vector<std::vector<int>>>> Group(int groupId) const {
				auto it = m_MAP.find(groupId);
				if (it != m_MAP.end()) {
					return it->second;
				}
				else {
					return std::nullopt;
				}
			}

		private:
			int m_Width;
			int m_Height;
			int m_TileWidth;
			int m_TileHeight;

			std::string m_Name;
			std::string m_Source;
			std::string m_Orientation;

			std::map<int, Tileset>		m_Tilesets;
			std::map<int, TileGroup>	m_Groups;
			std::map<int, TileLayer>	m_Layers;

			std::map<int, std::map<int, std::vector<std::vector<int>> > > m_MAP; // <GroupID <LAYERID, TILEID>>
	};

};

#endif // TILESET_H