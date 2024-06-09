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
#include "Tile.h"

namespace tile {

	class TileMap
	{
		public:
			TileMap(std::string name, std::string source, std::string orientation, 
				int width, int height, int tilewidth, int tileheight) : m_Name(name), m_Source(source),
				m_Orientation(orientation), m_Width(width), m_Height(height), m_TileWidth(tilewidth), m_TileHeight(tileheight)
			{}
			
			inline std::string Name() const { return m_Name; }
			inline std::map<int, Tileset>& TileSet() { return m_Tilesets; }
			Tileset GetTileset(int tilesetId) const;
			inline std::map<int, std::vector< std::vector<int> >> Group(int groupId) { return m_MAP[groupId]; }
			std::optional<int> FindFirstGid(int tileId) const;

			void PrintRawMap() const;

			inline std::vector<Tile> FormattedLayer(int groupId, int layerId) { return m_ParsedData[groupId][layerId]; }
			inline std::vector< std::vector<int> > RawLayer(int groupId, int layerId) { return m_MAP[groupId][layerId]; }
			
			inline void AddGroup(TileGroup group) { m_Groups.insert({ group.ID, group }); }
			inline void AddLayer(TileLayer layer) { m_Layers.insert({ layer.ID, layer }); }
			inline void AddTileset(Tileset tileset) { m_Tilesets.insert({ tileset.ID, tileset }); }

			inline void AddRawData(int groupId, int layerId, std::vector<std::vector<int>> data) { m_MAP[groupId][layerId] = data; }
			inline void AddFormattedData(int groupId, int layerId, std::vector<Tile> data) { m_ParsedData[groupId][layerId] = data; }

			std::optional<std::vector<std::vector<int>>> RawLayer(int groupId, int layerId) const;
			std::optional<std::map<int, std::vector<std::vector<int>>>> RawGroup(int groupId) const;


			std::optional<std::vector<Tile>> FormattedLayer(int groupId, int layerId) const;
			std::optional<std::map<int, std::vector<Tile>>> FormattedGroup(int groupId) const;

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
			std::map<int, std::map<int, std::vector<Tile>>> m_ParsedData; // <GroupID <LAYERID, TILE>>
	};

};

#endif // TILEMAP_H