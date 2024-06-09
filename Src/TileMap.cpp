#include "TileMap.h"
#include "Tile.h"
#include "TileException.h"

using namespace tiller;


std::optional<int> TileMap::FindFirstGid(int tileId) const {
	auto it = m_Tilesets.lower_bound(tileId);
	if (it == m_Tilesets.begin()) {
		// All elements are greater than gID
		return std::nullopt;
	}
	if (it == m_Tilesets.end() || it->first != tileId) {
		--it;
	}
	return it->first;
}

void TileMap::PrintRawMap() const {
	for (const auto& groupPair : m_MAP)
	{
		std::cout << "Group ID: " << groupPair.first << std::endl;
		for (const auto& layerPair : groupPair.second)
		{
			std::cout << "  Layer ID: " << layerPair.first << std::endl;
			for (const auto& row : layerPair.second)
			{
				for (int val : row)
				{
					std::cout << val << " ";
				}
				std::cout << std::endl;
			}
		}
	}
}


std::optional<std::vector<std::vector<int>>> TileMap::RawLayer(int groupId, int layerId) const {
	auto groupIt = m_MAP.find(groupId);
	if (groupIt != m_MAP.end()) 
	{
		auto layerIt = groupIt->second.find(layerId);
		if (layerIt != groupIt->second.end()) 
		{
			return layerIt->second;
		}
	}
	return std::nullopt;
}

std::optional<std::map<int, std::vector<std::vector<int>>>> TileMap::RawGroup(int groupId) const {
	auto it = m_MAP.find(groupId);
	if (it != m_MAP.end())
	{
		return it->second;
	}
	else
	{
		return std::nullopt;
	}

}
std::optional<std::vector<Tile>> TileMap::FormattedLayer(int groupId, int layerId) const {
	auto groupIt = m_ParsedData.find(groupId);
	if (groupIt != m_ParsedData.end())
	{
		auto layerIt = groupIt->second.find(layerId);
		if (layerIt != groupIt->second.end())
		{
			return layerIt->second;
		}
	}
	return std::nullopt;
}


std::optional<std::map<int, std::vector<Tile>>> TileMap::FormattedGroup(int groupId) const {
	auto it = m_ParsedData.find(groupId);
	if (it != m_ParsedData.end())
	{
		return it->second;
	}
	else
	{
		return std::nullopt;
	}
}

Tileset TileMap::GetTileset(int tilesetId) const {
	auto it = m_Tilesets.find(tilesetId);
	if (it != m_Tilesets.end()) {
		return it->second;
	}
	else {
		throw TillerException("Tileset ID not found");
	}
}