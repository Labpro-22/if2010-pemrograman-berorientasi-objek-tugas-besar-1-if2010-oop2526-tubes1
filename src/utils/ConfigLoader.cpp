#include "utils/ConfigLoader.hpp"

std::ifstream ConfigLoader::open(std::string path){
    std::ifstream file(path);
    if (!file){
        throw FileNotFoundException(path);
    }
    return file;
}


std::map<int, int> ConfigLoader::loadIntMap(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    std::map<int, int> map;
    try{
        while (!file.eof()){
            int key, value;
            file >> key >> value;
            map[key] = value;
        }
    }
    catch (std::exception e){
        throw InvalidFileDataException();
    } 
    return map;
}


std::tuple<int, int> ConfigLoader::loadTuple2(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    int num1, num2;
    try{
        file >> num1 >> num2;
    }
    catch (std::exception e){
        throw InvalidFileDataException();
    } 
    return std::make_tuple(num1, num2);
}
    

std::tuple<int, int, int> ConfigLoader::loadTuple3(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    int num1, num2, num3;
    try{
        file >> num1 >> num2 >> num3;
    }
    catch (std::exception e){
        throw InvalidFileDataException();
    } 
    return std::make_tuple(num1, num2, num3);
}


Color ConfigLoader::colorTypeToEnum(std::string color){
    if (color.compare("DEFAULT")){
        return Color::DEFAULT;
    }
    else if (color.compare("COKLAT")){
        return Color::BROWN;
    }
    else if (color.compare("BIRU_MUDA")){
        return Color::LIGHTBLUE;
    }
    else if (color.compare("MERAH_MUDA")){
        return Color::PINK;
    }
    else if (color.compare("ABU_ABU")){
        return Color::GRAY;
    }
    else if (color.compare("ORANGE")){
        return Color::ORANGE;
    }
    else if (color.compare("MERAH")){
        return Color::RED;
    }
    else if (color.compare("KUNING")){
        return Color::YELLOW;
    }
    else if (color.compare("HIJAU")){
        return Color::GREEN;
    }
    else if (color.compare("BIRU_TUA")){
        return Color::DARKBLUE;
    }
    else{
        throw UnknownColorException(color);
    }
}

std::vector<std::pair<int, Plot*>> ConfigLoader::loadProperty(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    std::vector<std::pair<int, Plot*>> tiles;
    try{
        while (!file.eof()){
            int id, buyPrice, mortgageValue;
            std::string code, name, type, color;
            std::map<int, int> rentPriceTable; 

            //Baca tipe
            file >> id >> code >> name >> type >> color;

            
            //Ubah color (string) menjadi enum Color
            Color colorEnum = ConfigLoader::colorTypeToEnum(color);

            //Buat plot sesuai tipenya
            Plot* plot;
    
            if (type.compare("STREET")){
                //Baca data lainnya
                int upgHousePrice, upgHotelPrice;
                file >> buyPrice >> mortgageValue >> upgHousePrice >> upgHotelPrice;

                //Isi rentPriceTable
                file >> id >> code >> name >> type;
                for (int i = 0; i <= 5; i++){
                    int value;
                    file >> value;
                    rentPriceTable[i] = value;
                }       

                plot = new LandPlot(name, code, colorEnum, mortgageValue, buyPrice, 
                                    upgHousePrice, upgHotelPrice, rentPriceTable);

            }
            else if (type.compare("RAILROAD")){
                plot = new StationPlot(name, code, colorEnum, mortgageValue);
            }
            else if (type.compare("UTILITY")){
                plot = new UtilityPlot(name, code, colorEnum, mortgageValue);
            }
            else{
                throw UnknownTypeException(type);
            }

            tiles.push_back(std::make_pair(id, plot));
        }
    }
    catch (GameException e){
        throw e;
    }
    catch (std::exception e){
        throw InvalidFileDataException();
    } 

    return tiles;
}


std::map<int, int> ConfigLoader::loadRailroad(std::string path){
    return loadIntMap(path);
}


std::map<int, int> ConfigLoader::loadUtility(std::string path){
    return loadIntMap(path);
}


std::tuple<int, int, int> ConfigLoader::loadTax(std::string path){
    return loadTuple3(path);
}


std::tuple<int, int> ConfigLoader::loadSpecial(std::string path){
    return loadTuple2(path);
}


std::tuple<int, int> ConfigLoader::loadMisc(std::string path){
    return loadTuple2(path);
}