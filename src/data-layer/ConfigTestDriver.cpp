#include "../../include/data-layer/Config.hpp"
#include "../../include/data-layer/ConfigHandler.hpp"
#include "../../include/data-layer/ConfigComposer.hpp"

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

static int totalTests = 0;
static int failedTests = 0;

static const string TEST_CONFIG_DIR = "config/test-driver";
static const string PROPERTY_PATH = TEST_CONFIG_DIR + "/property.txt";
static const string RAILROAD_PATH = TEST_CONFIG_DIR + "/railroad.txt";
static const string UTILITY_PATH = TEST_CONFIG_DIR + "/utility.txt";
static const string TAX_PATH = TEST_CONFIG_DIR + "/tax.txt";
static const string ACTION_PATH = TEST_CONFIG_DIR + "/aksi.txt";
static const string SPECIAL_PATH = TEST_CONFIG_DIR + "/special.txt";
static const string MISC_PATH = TEST_CONFIG_DIR + "/misc.txt";

static void printLine()
{
    cout << "------------------------------------------------------------\n";
}

static void check(bool condition, const string &message)
{
    totalTests++;

    if (condition)
    {
        cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedTests++;
        cout << "[FAIL] " << message << '\n';
    }
}

static void expectInt(int actual, int expected, const string &message)
{
    totalTests++;

    if (actual == expected)
    {
        cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedTests++;
        cout << "[FAIL] " << message
             << " | expected: " << expected
             << ", got: " << actual << '\n';
    }
}

static void expectSize(size_t actual, size_t expected, const string &message)
{
    totalTests++;

    if (actual == expected)
    {
        cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedTests++;
        cout << "[FAIL] " << message
             << " | expected: " << expected
             << ", got: " << actual << '\n';
    }
}

static void expectString(const string &actual, const string &expected, const string &message)
{
    totalTests++;

    if (actual == expected)
    {
        cout << "[PASS] " << message << '\n';
    }
    else
    {
        failedTests++;
        cout << "[FAIL] " << message
             << " | expected: " << expected
             << ", got: " << actual << '\n';
    }
}

static void writeFile(const string &path, const string &content)
{
    ofstream out(path);

    if (!out.is_open())
    {
        throw runtime_error("Cannot create test config file: " + path);
    }

    out << content;
}

static void prepareTestConfigFiles()
{
#ifdef _WIN32
    system("if not exist config\\test-driver mkdir config\\test-driver");
#else
    system("mkdir -p config/test-driver");
#endif

    writeFile(PROPERTY_PATH,
        "ID KODE NAMA JENIS WARNA HARGA_LAHAN NILAI_GADAI UPG_RUMAH UPG_HT RENT_L0 RENT_L1 RENT_L2 RENT_L3 RENT_L4 RENT_L5\n"
        "2 GRT GARUT STREET COKLAT 60 40 20 50 2 10 30 90 160 250\n"
        "4 TSK TASIKMALAYA STREET COKLAT 60 40 50 50 4 20 60 180 320 450\n"
    );

    writeFile(RAILROAD_PATH,
        "JUMLAH_RAILROAD BIAYASEWA\n"
        "1 25\n"
        "2 50\n"
        "3 100\n"
        "4 200\n"
    );

    writeFile(UTILITY_PATH,
        "JUMLAH_UTILITY FAKTOR_PENGALI\n"
        "1 4\n"
        "2 10\n"
    );

    writeFile(TAX_PATH,
        "PPH_FLAT PPH_PERSENTASE PBM_FLAT\n"
        "150 10 200\n"
    );

    writeFile(ACTION_PATH,
        "ID KODE NAMA JENIS_PETAK WARNA\n"
        "1 GO Petak_Mulai SPESIAL DEFAULT\n"
        "3 DNU Dana_Umum KARTU DEFAULT\n"
        "5 PPH Pajak_Penghasilan PAJAK DEFAULT\n"
    );

    writeFile(SPECIAL_PATH,
        "GO_SALARY JAIL_FINE\n"
        "200 50\n"
    );

    writeFile(MISC_PATH,
        "MAX_TURN SALDO_AWAL\n"
        "15 1000\n"
    );
}

static void runCase(const string &caseName, void (*testFunction)())
{
    printLine();
    cout << caseName << '\n';

    try
    {
        testFunction();
    }
    catch (const exception &e)
    {
        totalTests++;
        failedTests++;
        cout << "[FAIL] " << caseName << " threw exception: " << e.what() << '\n';
    }
    catch (...)
    {
        totalTests++;
        failedTests++;
        cout << "[FAIL] " << caseName << " threw unknown exception\n";
    }
}

static void testPropertyConfigHandler()
{
    PropertyConfig handler(PROPERTY_PATH);
    handler.loadConfig();

    vector<tuple<int, string, string, string, string, int, int, vector<int>>> data = handler.getData();

    expectSize(data.size(), 2, "PropertyConfig loads 2 property rows");

    tuple<int, string, string, string, string, int, int, vector<int>> first = data.at(0);

    expectInt(get<0>(first), 2, "PropertyConfig first id is 2");
    expectString(get<1>(first), "GRT", "PropertyConfig first code is GRT");
    expectString(get<2>(first), "GARUT", "PropertyConfig first name is GARUT");
    expectString(get<3>(first), "STREET", "PropertyConfig first type is STREET");
    expectString(get<4>(first), "COKLAT", "PropertyConfig first color is COKLAT");
    expectInt(get<5>(first), 60, "PropertyConfig first land price is 60");
    expectInt(get<6>(first), 40, "PropertyConfig first mortgage value is 40");

    vector<int> expectedRent = {20, 50, 2, 10, 30, 90, 160, 250};

    check(get<7>(first) == expectedRent, "PropertyConfig first rent vector is correct");
}

static void testRailroadConfigHandler()
{
    RailroadConfig handler(RAILROAD_PATH);
    handler.loadConfig();

    map<int, int> table = handler.getRailRoadRentTable();

    expectSize(table.size(), 4, "RailroadConfig loads 4 rows");
    expectInt(table[1], 25, "RailroadConfig rent for 1 railroad is 25");
    expectInt(table[4], 200, "RailroadConfig rent for 4 railroads is 200");
}

static void testUtilityConfigHandler()
{
    UtilityConfig handler(UTILITY_PATH);
    handler.loadConfig();

    map<int, int> table = handler.getUtilityMultiplier();

    expectSize(table.size(), 2, "UtilityConfig loads 2 rows");
    expectInt(table[1], 4, "UtilityConfig multiplier for 1 utility is 4");
    expectInt(table[2], 10, "UtilityConfig multiplier for 2 utilities is 10");
}

static void testTaxConfigHandler()
{
    TaxConfig handler(TAX_PATH);
    handler.loadConfig();

    tuple<int, int, int> tax = handler.getTaxConfig();

    expectInt(get<0>(tax), 150, "TaxConfig PPH flat is 150");
    expectInt(get<1>(tax), 10, "TaxConfig PPH percentage is 10");
    expectInt(get<2>(tax), 200, "TaxConfig PBM flat is 200");
}

static void testActionTileConfigHandler()
{
    ActionTileConfig handler(ACTION_PATH);
    handler.loadConfig();

    vector<tuple<int, string, string, string, string>> actions = handler.getActionTileConfig();

    expectSize(actions.size(), 3, "ActionTileConfig loads 3 rows");
    expectString(get<1>(actions.at(0)), "GO", "ActionTileConfig first code is GO");
    expectString(get<2>(actions.at(0)), "Petak_Mulai", "ActionTileConfig first name is Petak_Mulai");
}

static void testSpecialTileConfigHandler()
{
    SpecialTileConfig handler(SPECIAL_PATH);
    handler.loadConfig();

    tuple<int, int> special = handler.getSpecialTileConfig();

    expectInt(get<0>(special), 200, "SpecialTileConfig GO salary is 200");
    expectInt(get<1>(special), 50, "SpecialTileConfig jail fine is 50");
}

static void testMiscTileConfigHandler()
{
    MiscTileConfig handler(MISC_PATH);
    handler.loadConfig();

    tuple<int, int> misc = handler.getMiscTileConfig();

    expectInt(get<0>(misc), 15, "MiscTileConfig max turn is 15");
    expectInt(get<1>(misc), 1000, "MiscTileConfig initial balance is 1000");
}

static Config createManualConfig()
{
    vector<tuple<int, string, string, string, string, int, int, vector<int>>> properties;

    properties.push_back(make_tuple(
        2,
        "GRT",
        "GARUT",
        "STREET",
        "COKLAT",
        60,
        40,
        vector<int>{20, 50, 2, 10, 30, 90, 160, 250}
    ));

    properties.push_back(make_tuple(
        4,
        "TSK",
        "TASIKMALAYA",
        "STREET",
        "COKLAT",
        60,
        40,
        vector<int>{50, 50, 4, 20, 60, 180, 320, 450}
    ));

    map<int, int> railroadRent;
    railroadRent[1] = 25;
    railroadRent[2] = 50;
    railroadRent[3] = 100;
    railroadRent[4] = 200;

    map<int, int> utilityMultiplier;
    utilityMultiplier[1] = 4;
    utilityMultiplier[2] = 10;

    tuple<int, int, int> tax = make_tuple(150, 10, 200);
    tuple<int, int> special = make_tuple(200, 50);
    tuple<int, int> misc = make_tuple(15, 1000);

    vector<tuple<int, string, string, string, string>> actions;

    actions.push_back(make_tuple(
        1,
        "GO",
        "Petak_Mulai",
        "SPESIAL",
        "DEFAULT"
    ));

    actions.push_back(make_tuple(
        3,
        "DNU",
        "Dana_Umum",
        "KARTU",
        "DEFAULT"
    ));

    return Config(
        properties,
        railroadRent,
        utilityMultiplier,
        tax,
        special,
        misc,
        actions
    );
}

static void testConfigClass()
{
    Config config = createManualConfig();

    tuple<int, string, string, string, string, int, int, vector<int>> garut = config.getPropertyConfig("GRT");

    expectInt(get<0>(garut), 2, "Config can find property GRT");
    expectString(get<2>(garut), "GARUT", "Config returns property name GARUT");
    expectInt(get<5>(garut), 60, "Config returns property price 60");

    tuple<int, string, string, string, string, int, int, vector<int>> missingProperty = config.getPropertyConfig("XXX");

    expectInt(get<0>(missingProperty), -1, "Config returns id -1 for missing property");

    expectSize(config.getPropertyConfigAll().size(), 2, "Config returns all property configs");

    expectInt(config.getRailroadRent(1), 25, "Config railroad rent for count 1 is 25");
    expectInt(config.getRailroadRent(4), 200, "Config railroad rent for count 4 is 200");
    expectInt(config.getRailroadRent(0), 25, "Config railroad rent below min uses min rent");
    expectInt(config.getRailroadRent(5), 200, "Config railroad rent above max uses max rent");

    expectInt(config.getUtilityMultiplier(1), 4, "Config utility multiplier for count 1 is 4");
    expectInt(config.getUtilityMultiplier(2), 10, "Config utility multiplier for count 2 is 10");
    expectInt(config.getUtilityMultiplier(0), 4, "Config utility multiplier below min uses min multiplier");
    expectInt(config.getUtilityMultiplier(3), 10, "Config utility multiplier above max uses max multiplier");

    expectInt(config.getTaxConfig(PPH), 150, "Config PPH flat is 150");
    expectInt(config.getTaxConfig(PERSEN), 10, "Config tax percentage is 10");
    expectInt(config.getTaxConfig(PBM), 200, "Config PBM flat is 200");

    expectInt(config.getSpecialConfig(GO_SALARY), 200, "Config GO salary is 200");
    expectInt(config.getSpecialConfig(JAIL_FINE), 50, "Config jail fine is 50");

    expectInt(config.getMiscConfig(MAX_TURN), 15, "Config max turn is 15");
    expectInt(config.getMiscConfig(SALDO_AWAL), 1000, "Config initial balance is 1000");

    tuple<int, string, string, string, string> go = config.getActionTileConfig("GO");

    expectInt(get<0>(go), 1, "Config can find action tile GO");
    expectString(get<2>(go), "Petak_Mulai", "Config returns action tile name Petak_Mulai");

    tuple<int, string, string, string, string> missingAction = config.getActionTileConfig("XXX");

    expectInt(get<0>(missingAction), -1, "Config returns id -1 for missing action tile");
}

static void testConfigComposer()
{
    ConfigComposer composer(
        PROPERTY_PATH,
        RAILROAD_PATH,
        UTILITY_PATH,
        TAX_PATH,
        ACTION_PATH,
        SPECIAL_PATH,
        MISC_PATH
    );

    Config &config = composer.getConfig();

    expectSize(config.getPropertyConfigAll().size(), 2, "ConfigComposer loads 2 property configs");

    tuple<int, string, string, string, string, int, int, vector<int>> garut = config.getPropertyConfig("GRT");

    expectInt(get<0>(garut), 2, "ConfigComposer config can find property GRT");
    expectString(get<2>(garut), "GARUT", "ConfigComposer config returns property name GARUT");

    expectInt(config.getRailroadRent(2), 50, "ConfigComposer config railroad rent for count 2 is 50");
    expectInt(config.getUtilityMultiplier(2), 10, "ConfigComposer config utility multiplier for count 2 is 10");
    expectInt(config.getTaxConfig(PPH), 150, "ConfigComposer config PPH flat is 150");
    expectInt(config.getSpecialConfig(GO_SALARY), 200, "ConfigComposer config GO salary is 200");
    expectInt(config.getMiscConfig(SALDO_AWAL), 1000, "ConfigComposer config initial balance is 1000");

    tuple<int, string, string, string, string> go = config.getActionTileConfig("GO");

    expectInt(get<0>(go), 1, "ConfigComposer config can find action tile GO");
}

int main()
{
    cout << "Config, ConfigHandler, and ConfigComposer Test Driver\n";
    cout << "Run this program from the project root directory.\n";

    runCase("Prepare config/test-driver files", prepareTestConfigFiles);

    runCase("PropertyConfig handler test", testPropertyConfigHandler);
    runCase("RailroadConfig handler test", testRailroadConfigHandler);
    runCase("UtilityConfig handler test", testUtilityConfigHandler);
    runCase("TaxConfig handler test", testTaxConfigHandler);
    runCase("ActionTileConfig handler test", testActionTileConfigHandler);
    runCase("SpecialTileConfig handler test", testSpecialTileConfigHandler);
    runCase("MiscTileConfig handler test", testMiscTileConfigHandler);

    runCase("Config class test", testConfigClass);
    runCase("ConfigComposer test", testConfigComposer);

    printLine();

    cout << "Total checks : " << totalTests << '\n';
    cout << "Passed       : " << (totalTests - failedTests) << '\n';
    cout << "Failed       : " << failedTests << '\n';

    if (failedTests == 0)
    {
        cout << "RESULT       : ALL TESTS PASSED\n";
        return 0;
    }

    cout << "RESULT       : SOME TESTS FAILED\n";
    return 1;
}