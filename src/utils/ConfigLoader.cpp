#include "utils/ConfigLoader.hpp"

const std::string ConfigLoader::configPath = "config/ ";
const std::string ConfigLoader::defaultPropertyFileName = "property.txt";
const std::string ConfigLoader::defaultStationFileName = "railroad.txt";
const std::string ConfigLoader::defaultUtilityFileName = "utility.txt";
const std::string ConfigLoader::defaultTaxFileName = "tax.txt";
const std::string ConfigLoader::defaultActionFileName = "aksi.txt";
const std::string ConfigLoader::defaultSpecialFileName = "special.txt";
const std::string ConfigLoader::defaultMiscFileName = "misc.txt";

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
    catch (const std::exception&){
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
    catch (const std::exception&){
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
    catch (const std::exception&){
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

std::vector<std::pair<int, std::unique_ptr<Plot>>> ConfigLoader::loadProperty(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    std::vector<std::pair<int, std::unique_ptr<Plot>>> tiles;
    try{
        while (!file.eof()){
            int id, buyPrice, mortgageValue;
            std::string code, name, type, color;
            std::map<int, int> rentPriceTable; 

            //Baca tipe
            file >> id >> code >> name >> type >> color >> buyPrice >> mortgageValue;

            
            //Ubah color (string) menjadi enum Color
            Color colorEnum = ConfigLoader::colorTypeToEnum(color);

            //Buat plot sesuai tipenya
            std::unique_ptr<Plot> plot;
    
            if (type.compare("STREET")){
                //Baca data lainnya
                int upgHousePrice, upgHotelPrice;
                file >> upgHousePrice >> upgHotelPrice;

                //Isi rentPriceTable
                file >> id >> code >> name >> type;
                for (int i = 0; i <= 5; i++){
                    int value;
                    file >> value;
                    rentPriceTable[i] = value;
                }       

                plot = std::make_unique<Plot>(LandPlot(name, code, colorEnum, buyPrice, mortgageValue,
                                    upgHousePrice, upgHotelPrice, rentPriceTable));

            }
            else if (type.compare("RAILROAD")){
                plot = std::make_unique<Plot>(StationPlot(name, code, colorEnum, buyPrice, mortgageValue));
            }
            else if (type.compare("UTILITY")){
                plot = std::make_unique<Plot>(UtilityPlot(name, code, colorEnum, buyPrice, mortgageValue));
            }
            else{
                throw UnknownTypeException(type);
            }

            tiles.push_back(std::make_pair(id, plot));
        }
    }
    catch (const GameException&){
        throw;
    }
    catch (const std::exception&){
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

std::vector<std::pair<int, std::unique_ptr<Plot>>> ConfigLoader::loadAction(std::string path){
    std::ifstream file = ConfigLoader::open(path);
    std::vector<std::pair<int, std::unique_ptr<Plot>>> tiles;
    try{
        while (!file.eof()){
            int id;
            std::string code, name, type, color;

            //Baca tipe
            file >> id >> code >> name >> type >> color;

            
            //Ubah color (string) menjadi enum Color
            Color colorEnum = ConfigLoader::colorTypeToEnum(color);

            //Buat plot sesuai tipenya
            std::unique_ptr<Plot> plot;
    
            if (type.compare("SPESIAL")){
                if (code.compare("GO")){
                    plot = std::make_unique<Plot>(StartPlot(name, code, colorEnum));
                }
                else if (code.compare("PEN")){
                    plot = std::make_unique<Plot>(PrisonPlot(name, code, colorEnum));
                }
                else if (code.compare("BBP")){
                    plot = std::make_unique<Plot>(FreeParkPlot(name, code, colorEnum));
                }
                else if (code.compare("PPJ")){
                    plot = std::make_unique<Plot>(GoPrisonPlot(name, code, colorEnum));
                }
                else{
                    throw UnknownTypeException(type + " dengan kode " + code);
                }
            }
            else if (type.compare("KARTU")){
                if (code.compare("DNU")){
                    plot = std::make_unique<Plot>(CommunityChestCardPlot(name, code, colorEnum));
                }
                else if (code.compare("KSP")){
                    plot = std::make_unique<Plot>(ChanceCardPlot(name, code, colorEnum));
                }
                else{
                    throw UnknownTypeException(type + " dengan kode " + code);
                }
            }
            else if (type.compare("PAJAK")){
                if (code.compare("PPH")){
                    plot = std::make_unique<Plot>(IncomeTaxPlot(name, code, colorEnum));
                }
                else if (code.compare("PBM")){
                    plot = std::make_unique<Plot>(LuxuryTaxPlot(name, code, colorEnum));
                }
                else{
                    throw UnknownTypeException(type + " dengan kode " + code);
                }
            }
            else if (type.compare("FESTIVAL")){
                plot = std::make_unique<Plot>(FestivalPlot(name, code, colorEnum));
            }
            else{
                throw UnknownTypeException(type);
            }

            tiles.push_back(std::make_pair(id, plot));
        }
    }
    catch (const GameException&){
        throw;
    }
    catch (const std::exception&){
        throw InvalidFileDataException();
    } 

    return tiles;
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
