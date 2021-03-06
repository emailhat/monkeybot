#include "Config.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>

namespace {

const std::vector<std::string> ShipNames = { 
    "Warbird", "Javelin", "Spider", "Leviathan", "Terrier", "Weasel", "Lancaster", "Shark"
};

std::regex VecRE(R"::(\(([0-9]+),\s*?([0-9]+)\))::");

} // ns

Config::Config() 
    : XPercent(75),
      RunPercent(30),
      TargetDistance(10),
      RunDistance(30),
      StopBombing(90),
      BombDelay(500),
      FireBombs(true),
      FireGuns(true),
      Level(""),
      BulletDelay(20),
      ScaleDelay(true),
      CenterOnly(true),
      Patrol(true),
      Attach(false),
      MinGunRange(0),
      SpawnX(512),
      SpawnY(512),
      Baseduel(false),
      CenterRadius(250),
      IgnoreDelayDistance(10),
      RepelPercent(25),
      UseBurst(true),
      DecoyDelay(0),
      LogFile(""),
      Taunt(false),
      TauntCooldown(6000),
      Hyperspace(true),
      Commander(false),
      Survivor(false),
      Zone("Hyperspace")
{

}

void Config::LoadFromNode(const Json::Value& node) {
    #define NODELOAD(x) if (node.isMember(#x)) x = node.get(#x, std::to_string(x))

    NODELOAD(XPercent).asInt();
    NODELOAD(RunPercent).asInt();
    NODELOAD(TargetDistance).asInt();
    NODELOAD(RunDistance).asInt();
    NODELOAD(StopBombing).asInt();
    NODELOAD(BombDelay).asInt();
    NODELOAD(FireBombs).asBool();
    NODELOAD(FireGuns).asBool();
    NODELOAD(BulletDelay).asInt();
    NODELOAD(ScaleDelay).asBool();
    NODELOAD(CenterOnly).asBool();
    NODELOAD(Patrol).asBool();
    NODELOAD(Attach).asBool();
    NODELOAD(MinGunRange).asInt();
    NODELOAD(SpawnX).asInt();
    NODELOAD(SpawnY).asInt();
    NODELOAD(Baseduel).asBool();
    NODELOAD(CenterRadius).asInt();
    NODELOAD(IgnoreDelayDistance).asInt();
    NODELOAD(RepelPercent).asInt();
    NODELOAD(UseBurst).asBool();
    NODELOAD(DecoyDelay).asInt();
    NODELOAD(Taunt).asBool();
    NODELOAD(TauntCooldown).asInt();
    NODELOAD(Hyperspace).asBool();
    NODELOAD(Commander).asBool();
    NODELOAD(Survivor).asBool();
    
    if (node.isMember("Level"))
        Level   = node.get("Level", Level).asString();
    if (node.isMember("LogFile"))
        LogFile = node.get("LogFile", LogFile).asString();

    const Json::Value waypoints = node["Waypoints"];

    if (!waypoints.isNull()) {
        Waypoints.clear();

        for (size_t i = 0; i < waypoints.size(); ++i) {
            std::string waypoint = waypoints[i].asString();

            std::sregex_iterator begin(waypoint.begin(), waypoint.end(), VecRE);
            std::sregex_iterator end;

            if (begin == end) continue;
            std::smatch match = *begin;

            int x = atoi(std::string(match[1]).c_str());
            int y = atoi(std::string(match[2]).c_str());

            Waypoints.push_back(Vec2(x, y));
        }
    }

    const Json::Value taunts = node["Taunts"];

    if (!taunts.isNull()) {
        Taunts.clear();

        for (size_t i = 0; i < taunts.size(); ++i)
            Taunts.push_back(taunts[i].asString());
    }

    const Json::Value whitelist = node["TauntWhitelist"];

    if (!whitelist.isNull()) {
        TauntWhitelist.clear();

        for (size_t i = 0; i < whitelist.size(); ++i) {
            std::string name = whitelist[i].asString();

            std::transform(name.begin(), name.end(), name.begin(), tolower);
            TauntWhitelist.push_back(name);
        }
    }

    const Json::Value staff = node["Staff"];

    if (!staff.isNull()) {
        Staff.clear();

        for (size_t i = 0; i < staff.size(); ++i) {
            std::string name = staff[i].asString();

            std::transform(name.begin(), name.end(), name.begin(), tolower);
            Staff.push_back(name);
        }
    }

    const Json::Value rotations = node["Rotations"];

    if (!rotations.isNull()) {
        for (size_t ship = 0; ship < ShipNames.size(); ++ship) {
            ShipRotations[ship].clear();

            const Json::Value rot_values = rotations[ShipNames[ship]];

            for (size_t i = 0; i < rot_values.size(); ++i)
                ShipRotations[ship].push_back(rot_values[i].asInt64());
        }
    }
}

void Config::LoadShip(Ship ship) {
    std::string name = ShipNames[(int)ship];
    Json::Value zone_node = m_Root[Zone];
    Json::Value ship_node = zone_node[name];
    
    if (ship_node.isNull()) return;
    
    LoadFromNode(ship_node);
}

bool Config::Load(const std::string& jsonfile) {
    std::ifstream input(jsonfile, std::ios::binary);
    if (!input.is_open()) return false;

    Json::Reader reader;

    if (!reader.parse(input, m_Root)) {
        std::cerr << reader.getFormattedErrorMessages() << std::endl;
        return false;
    }

    Zone = m_Root.get("Zone", "Hyperspace").asString();

    Json::Value zone_node = m_Root[Zone];

    if (!zone_node.isNull()) {
        LoadFromNode(zone_node);
    } else {
        std::cerr << "Zone " << Zone <<  " not found in config." << std::endl;
        return false;
    }
    
    return true;
}

std::ostream& operator<<(std::ostream& os, const Config& c) {
    #define OUTPUT(x) os << #x << ": " << std::boolalpha << c.x << std::endl

    OUTPUT(XPercent);
    OUTPUT(RunPercent);
    OUTPUT(TargetDistance);
    OUTPUT(RunDistance);
    OUTPUT(StopBombing);
    OUTPUT(BombDelay);
    OUTPUT(FireBombs);
    OUTPUT(FireGuns);
    OUTPUT(Level);
    OUTPUT(BulletDelay);
    OUTPUT(ScaleDelay);
    OUTPUT(CenterOnly);
    OUTPUT(Patrol);
    OUTPUT(Attach);
    OUTPUT(MinGunRange);
    OUTPUT(SpawnX);
    OUTPUT(SpawnY);
    OUTPUT(Baseduel);
    OUTPUT(CenterRadius);
    OUTPUT(IgnoreDelayDistance);
    OUTPUT(RepelPercent);
    OUTPUT(UseBurst);
    OUTPUT(DecoyDelay);
    OUTPUT(LogFile);
    OUTPUT(Taunt);
    OUTPUT(TauntCooldown);
    OUTPUT(Hyperspace);
    OUTPUT(Commander);
    OUTPUT(Survivor);
    OUTPUT(Zone);

    os << "Waypoints: ";
    for (size_t i = 0; i < c.Waypoints.size(); ++i) {
        if (i != 0) os << ", ";
        os << c.Waypoints[i];
    }
    os << std::endl;

    os << "Staff: ";
    for (size_t i = 0; i < c.Staff.size(); ++i) {
        if (i != 0) os << ", ";
        os << c.Staff[i];
    }
    os << std::endl;

    os << "Taunt count: " << c.Taunts.size() << std::endl;
    os << "Taunt whitelist count: " << c.TauntWhitelist.size() << std::endl;
    return os;
}
