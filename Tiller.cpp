#include "Tiller.h"
#include <iostream>
#include <string>
#include <sstream>

#include "Tileset.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"

using namespace tile;

bool Tiller::Load(std::string path, std::string tmxName)
{
	m_Path = path;
	auto fullPath = path + tmxName + ".tmx";

	std::cout << GREEN << "TILLER - Loading file: " << fullPath << RESET << ENDL;
	return Parse("level1", fullPath);
}

bool Tiller::Parse(std::string mapId, std::string source)
{
	tinyxml2::XMLDocument xml;
	tinyxml2::XMLError eResult = xml.LoadFile(source.c_str());

	if (eResult != tinyxml2::XML_SUCCESS) {
		std::cout << RED << "TILLER - Error loading file: " << eResult << RESET << ENDL;
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
			std::cout << GREEN << "TILLER - Loading tileset: " << tileset.ID << RESET << ENDL;

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
			tilegroup.Opacity = e->IntAttribute("opacity", 1);
			tilegroup.Name = e->IntAttribute("name");
			m_Map->AddGroup(tilegroup);

			std::cout << YELLOW << "TILLER - Loading group: " << tilegroup.ID << RESET << ENDL;

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

					std::cout << BLUE << "\tTILLER - Loading tilelayer: " << tilelayer.ID << RESET << ENDL;
					m_Map->AddLayer(tilelayer);

					std::vector<std::vector<int>> dataParsed = ParseLayerData(l, tilelayer);

					std::cout << GREEN << "\rTILLER - SAVING DATA " << tilegroup.ID << " - " << tilelayer.ID << RESET << ENDL;
					m_Map->AddData(tilegroup.ID, tilelayer.ID, dataParsed);
				}
			}

		}
	}

	return true;
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
		std::cerr << RED <<  "TILLER - No data element found in layer" << RESET << ENDL;
		return dataParsed;
	}

	const char* dataText = data->GetText();
	if (!dataText) {
		std::cerr << RED << "TILLER - No text found in data element" << RESET << std::endl;
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
	std::cout << GREEN << "TILER - Loading tileset : " << (m_Path + sourceFile) << RESET << ENDL;

	Tileset tileset;

	tinyxml2::XMLError eResult = xml.LoadFile((m_Path + sourceFile).c_str());
	if (eResult != tinyxml2::XML_SUCCESS) {
		std::cout << RED << "TILLER - Error loading tileset file: " << eResult << RESET << ENDL;
		return tileset;
	}

	// TODO: Should raise error
	if (eResult != tinyxml2::XML_SUCCESS) {
		std::cout << RED << "Error loading file: " << eResult << RESET << std::endl;
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

