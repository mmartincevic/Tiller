#include "Tiller.h"
#include <iostream>
#include <string>
#include <sstream>

#include "Tileset.h"

using namespace tiller;

bool Tiller::Load(std::string path, std::string tmxName)
{
	m_Path = path;
	auto fullPath = path + tmxName + ".tmx";

	Log(GREEN, "Loading file: " + fullPath);
	return Parse("level1", fullPath);
}

bool Tiller::Parse(std::string mapId, std::string source)
{
	tinyxml2::XMLDocument xml;
	tinyxml2::XMLError eResult = xml.LoadFile(source.c_str());

	if (eResult != tinyxml2::XML_SUCCESS) {
		Log(RED, "Error loading file: " + eResult);
		return false;
	}

	tinyxml2::XMLElement* root = xml.RootElement();

	m_Map = new TileMap(mapId, source, root->Attribute("orientation"),
		root->IntAttribute("width", 0), root->IntAttribute("height", 0), 
		root->IntAttribute("tilewidth", 0), root->IntAttribute("tileheight", 0));


	// Fetch tilesets
	Tileset tileset;
	for (tinyxml2::XMLElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("tileset"))
		{
			if (e->ClosingType() == tinyxml2::XMLElement::ElementClosingType::OPEN)
			{
				tileset = ParseTileset(e);
			}
			else
			{
				tileset = ParseClosedTileset(e);
				tileset.ID = e->IntAttribute("firstgid");
			}

			Log(GREEN, "Loading tileset: " + tileset.ID);
			m_Map->AddTileset(tileset);
		}
	}

	// Fetch group
	for (tinyxml2::XMLElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("group"))
		{
			TileGroup tilegroup;
			tilegroup.ID = e->IntAttribute("id", 0);
			tilegroup.Opacity = e->FloatAttribute("opacity", 1.0f);
			tilegroup.Name = e->IntAttribute("name");
			m_Map->AddGroup(tilegroup);

			Log(YELLOW, "Loading group: " + tilegroup.ID);
			for (tinyxml2::XMLElement* l = e->FirstChildElement(); l != nullptr; l = l->NextSiblingElement())
			{
				if (l->Value() == std::string("layer"))
				{
					TileLayer tilelayer;
					tilelayer.ID = l->IntAttribute("id", 0);
					tilelayer.Name = l->Attribute("name");
					tilelayer.Width = l->IntAttribute("width", 0);
					tilelayer.Height = l->IntAttribute("height", 0);
					tilelayer.OffsetX = l->IntAttribute("offsetx", 0);
					tilelayer.OffsetY = l->IntAttribute("offsety", 0);

					Log(YELLOW, "Loading tilelayer: " + std::to_string(tilelayer.ID), "\t");
					m_Map->AddLayer(tilelayer);

					std::vector<std::vector<int>> dataParsed = ParseLayerData(l, tilelayer);

					Log(GREEN, "SAVING DATA " + std::to_string(tilegroup.ID) + " - " + std::to_string(tilelayer.ID), "\t");
					m_Map->AddRawData(tilegroup.ID, tilelayer.ID, dataParsed);

					m_Map->AddFormattedData(tilegroup.ID, tilelayer.ID, FormatLayerData(tilegroup, tilelayer, dataParsed));
				}
			}

		}
	}

	return true;
}

std::vector<Tile> Tiller::FormatLayerData(TileGroup tileGroup, TileLayer tileLayer, std::vector<std::vector<int>> rawData)
{
	std::vector<Tile> fData;

	for (int row = 0; row < rawData.size(); ++row)
	{
		for (int col = 0; col < rawData[row].size(); ++col)
		{
			auto gId = rawData[row][col];

			if (gId > 0)
			{
				unsigned global_tile_id = gId;

				bool flipped_horizontally = (global_tile_id & FLIPPED_HORIZONTALLY_FLAG);
				bool flipped_vertically = (global_tile_id & FLIPPED_VERTICALLY_FLAG);
				bool flipped_diagonally = (global_tile_id & FLIPPED_DIAGONALLY_FLAG);
				bool rotated_hex120 = (global_tile_id & ROTATED_HEXAGONAL_120_FLAG);


				global_tile_id &= ~(FLIPPED_HORIZONTALLY_FLAG |
					FLIPPED_VERTICALLY_FLAG |
					FLIPPED_DIAGONALLY_FLAG |
					ROTATED_HEXAGONAL_120_FLAG);

				auto firstgid = m_Map->FindFirstGid(global_tile_id);

				Tile tile;
				tile.gID = gId;
				tile.lID = global_tile_id - *firstgid;
				tile.row = row;
				tile.col = col;
				tile.groupID = tileGroup.ID;
				tile.layerID = tileLayer.ID;
				tile.opacity = tileGroup.Opacity; // not good
				tile.width = tileLayer.Width;
				tile.height = tileLayer.Height;
				tile.imageSrc = m_Map->GetTileset(*firstgid).ImgSource;
				tile.tilesetID = *firstgid;

				if (flipped_horizontally) {
					tile.rFlag = FLIPPED_HORIZONTALLY_FLAG;
				}
				else if (flipped_vertically) {
					tile.rFlag = FLIPPED_VERTICALLY_FLAG;
				}
				else if (flipped_diagonally) {
					tile.rFlag = FLIPPED_DIAGONALLY_FLAG;
				}
				else if (rotated_hex120) {
					tile.rFlag = ROTATED_HEXAGONAL_120_FLAG;
				}

				fData.push_back(tile);
			}
		}
	}

	return fData;
}

std::vector<std::vector<int>> Tiller::ParseLayerData(tinyxml2::XMLElement* xmlLayer, TileLayer& layer) {
	tinyxml2::XMLElement* data = nullptr;
	std::vector<std::vector<int>> dataParsed(layer.Height, std::vector<int>(layer.Width, 0));

	for (tinyxml2::XMLElement* e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (strcmp(e->Value(), "data") == 0) {
			data = e;
			break;
		}
	}

	if (!data) {
		Log(RED, "No data element found in layer", "\t");
		return dataParsed;
	}

	const char* dataText = data->GetText();
	if (!dataText) {
		Log(RED, "No text found in data element", "\t");
		return dataParsed;
	}

	std::string matrix(dataText);
	std::istringstream iss(matrix);
	std::string id;

	for (int row = 0; row < layer.Height; ++row) {
		for (int col = 0; col < layer.Width; ++col) {
			getline(iss, id, ',');
			std::stringstream convertor(id);
			convertor >> dataParsed[row][col];

			if (!iss.good())
			{
				break;
			}
		}
	}

	return dataParsed;
}

Tileset Tiller::ParseTileset(tinyxml2::XMLElement* xmlTileset)
{
	Tileset tileset;
	tileset.ID = xmlTileset->IntAttribute("firstgid");
	tileset.Name = xmlTileset->Attribute("name");
	tileset.TileWidth = xmlTileset->IntAttribute("tilewidth");
	tileset.TileHeight = xmlTileset->IntAttribute("tileheight");
	tileset.TileCount = xmlTileset->IntAttribute("tilecount");
	tileset.Columns = xmlTileset->IntAttribute("columns");
	tileset.LastID = (tileset.ID + tileset.TileCount) - 1;
	tileset.RowCount = tileset.TileCount / tileset.Columns;
	tinyxml2::XMLElement* image = xmlTileset->FirstChildElement();
	tileset.ImgSource = image->Attribute("source");
	return tileset;
}

Tileset Tiller::ParseClosedTileset(tinyxml2::XMLElement* xmlTileset)
{
	auto sourceFile = xmlTileset->Attribute("source");

	tinyxml2::XMLDocument xml;

	Log(GREEN, "Loading tileset: " + (m_Path + sourceFile));
	Tileset tileset;

	tinyxml2::XMLError eResult = xml.LoadFile((m_Path + sourceFile).c_str());
	if (eResult != tinyxml2::XML_SUCCESS) {
		Log(RED, "Error loading tileset file: " + eResult);
		return tileset;
	}

	// TODO: Should raise error
	if (eResult != tinyxml2::XML_SUCCESS) {
		Log(RED, "Error loading file: " + eResult);
		return tileset;
	}

	tinyxml2::XMLElement* root = xml.RootElement();
 	tileset.Name = root->Attribute("name");
	tileset.TileWidth = root->IntAttribute("tilewidth");
	tileset.TileHeight = root->IntAttribute("tileheight");
	tileset.TileCount = root->IntAttribute("tilecount");
	tileset.Columns = root->IntAttribute("columns");
	tileset.LastID = (tileset.ID + tileset.TileCount) - 1;
	tileset.RowCount = tileset.TileCount / tileset.Columns;
	tinyxml2::XMLElement* image = root->FirstChildElement();
	tileset.ImgSource = image->Attribute("source");
	return tileset;
}

void Tiller::PrintTile(const Tile& tile) {
	std::cout << "Tile properties:" << std::endl;
	std::cout << "gID: " << tile.gID << std::endl;
	std::cout << "lID: " << tile.lID << std::endl;
	std::cout << "rFlag: " << tile.rFlag << std::endl;
	std::cout << "tilesetID: " << tile.tilesetID << std::endl;
	std::cout << "layerID: " << tile.layerID << std::endl;
	std::cout << "groupID: " << tile.groupID << std::endl;
	std::cout << "opacity: " << tile.opacity << std::endl;
	std::cout << "width: " << tile.width << std::endl;
	std::cout << "height: " << tile.height << std::endl;
	std::cout << "row: " << tile.row << std::endl;
	std::cout << "col: " << tile.col << std::endl;
	std::cout << "imageSrc: " << tile.imageSrc << std::endl;
}