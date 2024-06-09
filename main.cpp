///-------------------------------------------------------------------------------------------------
/// File:	tiller.cpp.
///
/// Summary:	Demo app for parsing Tiled files
///-------------------------------------------------------------------------------------------------


#include <iostream>
#include "Src\Tiller.h"

int main()
{
    std::cout << "Hello World!\n";

    tile::Tiller::Instance().EnableLog();
    tile::Tiller::Instance().Load("Resources/level2/", "level2");
    tile::Tiller::Instance().DisableLog();

    //tile::Tiller::Instance().Map()->PrintMap();  // Print and test your map is being loaded

    // If we run through tilesets we can get exactly which images we need to preload
    // Also there are few things that we can use to index those like tilesed.ID or tileset.Name
    // We can even combine it with map name
    for (auto tileset : tile::Tiller::Instance().Map()->TileSet())
    {
        std::cout << "Tileset name: " << tileset.second.Name << " - ";
        std::cout << "Map name:" << tile::Tiller::Instance().Map()->Name() << " - ";
        std::cout << "Img src : " << tileset.second.ImgSource << ENDL;
    }

    std::vector<std::vector<int>> layerData = tile::Tiller::Instance().Map()->RawLayer(35, 36);
    
    for (int row = 0; row < 30; ++row)
    {
        std::vector<int> rowData = layerData[row];

        for (int column = 0; column < 400; ++column)
        {

            auto global_tile_id = rowData[column];
            if (global_tile_id > 0)
            {
                std::cout << "Tile: " << global_tile_id << ENDL;
                bool flipped_horizontally = (global_tile_id & tile::FLIPPED_HORIZONTALLY_FLAG);
                bool flipped_vertically = (global_tile_id & tile::FLIPPED_VERTICALLY_FLAG);
                bool flipped_diagonally = (global_tile_id & tile::FLIPPED_DIAGONALLY_FLAG);
                bool rotated_hex120 = (global_tile_id & tile::ROTATED_HEXAGONAL_120_FLAG);

                if (flipped_horizontally)
                {
                    std::cout << "Tile: " << global_tile_id << " is flipped horizontaly" << ENDL;
                }

                if (flipped_vertically)
                {
                    std::cout << "Tile: " << global_tile_id << " is flipped vertically" << ENDL;
                }

                if (flipped_diagonally)
                {
                    std::cout << "Tile: " << global_tile_id << " is flipped diagonally" << ENDL;
                }

                if (rotated_hex120)
                {
                    std::cout << "Tile: " << global_tile_id << " is rotated hex120" << ENDL;
                }

                global_tile_id &= ~(tile::FLIPPED_HORIZONTALLY_FLAG |
                    tile::FLIPPED_VERTICALLY_FLAG |
                    tile::FLIPPED_DIAGONALLY_FLAG |
                    tile::ROTATED_HEXAGONAL_120_FLAG);



                std::cout << "Global tile ID: " << global_tile_id << ENDL;
                std::cout << "Local tile ID: " << global_tile_id - 5121 << ENDL;
            }
        }
    }

    
    // Now you can use raw data as above but you can also use formatted and slim down version of data

    std::vector<tile::Tile> formattedData = tile::Tiller::Instance().Map()->FormattedLayer(35, 36);

    for (auto tile : formattedData)
    {
        // All data for printing will be located in the Tile struct so we don't need to run
        // additional calculations each frame rendering but just once on loading
        // ++ we eliminate empty rows from data structures = less weight
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

}