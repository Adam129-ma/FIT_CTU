#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

#endif /* __PROGTEST__ */

struct Property {
    std::string city;
    std::string addr;
    std::string region;
    unsigned int id;
    std::string owner;

    Property(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) :
            city(city), addr(addr), region(region), id(id) {}


    bool operator==(const Property &other) const {
        return (city == other.city && addr == other.addr) || (region == other.region && id == other.id);
    }

    bool operator<(const Property &other) const {
        if (city == other.city)
            return addr < other.addr;
        return city < other.city;
    }

    bool operator>(const Property &other) const {
        if (region == other.region)
            return id > other.id;
        return region > other.region;
    }

};
std::string toLower(const std::string &str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}
bool areOwnersEqual(const std::string& owner1, const std::string& owner2) {
    if (owner1.length() != owner2.length()) {
        return false;
    }
    for (size_t i = 0; i < owner1.length(); ++i) {
        if (std::tolower(owner1[i]) != std::tolower(owner2[i])) {
            return false;
        }
    }
    return true;
}

class CIterator {
public:
    CIterator(const std::vector<Property> &data);

    ~CIterator();

    bool atEnd() const;

    void next();

    std::string city() const;

    std::string addr() const;

    std::string region() const;

    unsigned int id() const;

    std::string owner() const;

private:
    const std::vector<Property> &m_data;
    size_t m_position;
};

CIterator::CIterator(const std::vector<Property> &data) :
        m_data(data), m_position(0) {}

CIterator::~CIterator() {}


bool CIterator::atEnd() const {
    return m_position >= m_data.size();
}


void CIterator::next() {
    if (!atEnd()) {
        m_position++;
    }
}

std::string CIterator::city() const {
    return m_data[m_position].city;
}

std::string CIterator::addr() const {
    return m_data[m_position].addr;
}

std::string CIterator::region() const {
    return m_data[m_position].region;
}

unsigned int CIterator::id() const {
    return m_data[m_position].id;
}

std::string CIterator::owner() const {
    return m_data[m_position].owner;
}

class CLandRegister {
public:
    CLandRegister();

    ~CLandRegister();

    bool add(const std::string &city,
             const std::string &addr,
             const std::string &region,
             unsigned int id);

    bool del(const std::string &city,
             const std::string &addr);

    bool del(const std::string &region,
             unsigned int id);

    bool getOwner(const std::string &city,
                  const std::string &addr,
                  std::string &owner) const;

    bool getOwner(const std::string &region,
                  unsigned int id,
                  std::string &owner) const;

    bool newOwner(const std::string &city,
                  const std::string &addr,
                  const std::string &owner);

    bool newOwner(const std::string &region,
                  unsigned int id,
                  const std::string &owner);

    size_t count(const std::string &owner) const;

    CIterator listByAddr() const;

    CIterator listByOwner(const std::string &owner) const;

private:


    std::vector<Property> cityAddrVec;
    std::vector<Property> regionIdVec;
    std::vector<Property> empty;
    std::vector<std::pair<std::string, std::vector<Property>>> propertiesByOwner;

    std::vector<Property>::iterator findProperty(const std::string &city, const std::string &addr);

    std::vector<Property>::iterator findProperty(const std::string &region, unsigned int id);
};
std::vector<Property>::iterator CLandRegister::findProperty(const std::string& city, const std::string& addr) {
    Property searchProperty(city, addr, "", 0);

    auto it = std::lower_bound(cityAddrVec.begin(), cityAddrVec.end(), searchProperty);

    if (it != cityAddrVec.end() && it->city == city && it->addr == addr) {
        return it;
    }

    return cityAddrVec.end();
}

std::vector<Property>::iterator CLandRegister::findProperty(const std::string& region, unsigned int id) {
    Property searchProperty("", "", region, id);

    auto it = std::lower_bound(regionIdVec.begin(), regionIdVec.end(), searchProperty, std::greater<Property>());

    if (it != regionIdVec.end() && it->region == region && it->id == id) {
        return it;
    }

    return regionIdVec.end();
}


CLandRegister::CLandRegister() {}

CLandRegister::~CLandRegister() {}

bool CLandRegister::add(const std::string &city, const std::string &addr, const std::string &region, unsigned int id) {
    Property newProperty(city, addr, region, id);

    if (findProperty(city, addr) != cityAddrVec.end() || findProperty(region, id) != regionIdVec.end()) {
        return false;
    }

    auto cityAddrIter = std::lower_bound(cityAddrVec.begin(), cityAddrVec.end(), newProperty);

    cityAddrVec.insert(cityAddrIter, newProperty);

    auto regionIdIter = std::lower_bound(regionIdVec.begin(), regionIdVec.end(), newProperty, std::greater<Property>());

    regionIdVec.insert(regionIdIter, newProperty);

    if (propertiesByOwner.empty() || propertiesByOwner.begin()->first != "") {
        propertiesByOwner.emplace(propertiesByOwner.begin(), "", std::vector<Property>{});
    }

    propertiesByOwner.begin()->second.push_back(newProperty);

    return true;
}

bool CLandRegister::del(const std::string &city, const std::string &addr) {
    auto it = std::lower_bound(cityAddrVec.begin(), cityAddrVec.end(), Property(city, addr, "", 0));

    if (it != cityAddrVec.end() && it->city == city && it->addr == addr) {
        auto regionIdIter = std::lower_bound(regionIdVec.begin(), regionIdVec.end(), *it, std::greater<Property>());

        std::string lowercaseOwner = toLower(it->owner);

        auto ownerIter = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                                          [](const auto &pair, const std::string &owner) {
                                              return pair.first < owner;
                                          });

        if (ownerIter != propertiesByOwner.end() && ownerIter->first == lowercaseOwner) {
            auto& properties = ownerIter->second;
            auto propIter = std::find(properties.begin(), properties.end(), *it);
            if (propIter != properties.end()) {
                properties.erase(propIter);
            }
        }

        if (regionIdIter != regionIdVec.end() && *regionIdIter == *it) {
            regionIdVec.erase(regionIdIter);
        }
        cityAddrVec.erase(it);

        return true;
    }

    return false;
}

bool CLandRegister::del(const std::string &region, unsigned int id) {
    auto it = std::lower_bound(regionIdVec.begin(), regionIdVec.end(), Property("", "", region, id), std::greater<Property>());

    if (it != regionIdVec.end() && it->region == region && it->id == id) {
        auto cityAddrIter = std::lower_bound(cityAddrVec.begin(), cityAddrVec.end(), *it);

        std::string lowercaseOwner = toLower(it->owner);

        auto ownerIter = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                                          [](const auto &pair, const std::string &owner) {
                                              return pair.first < owner;
                                          });

        if (ownerIter != propertiesByOwner.end() && ownerIter->first == lowercaseOwner) {
            auto& properties = ownerIter->second;
            auto propIter = std::find(properties.begin(), properties.end(), *it);
            if (propIter != properties.end()) {
                properties.erase(propIter);
            }
        }

        if (cityAddrIter != cityAddrVec.end() && *cityAddrIter == *it) {
            cityAddrVec.erase(cityAddrIter);
        }
        regionIdVec.erase(it);

        return true;
    }

    return false;
}

bool CLandRegister::getOwner(const std::string &city, const std::string &addr, std::string &owner) const {
    Property searchProperty(city, addr, "", 0);
    auto it = std::lower_bound(cityAddrVec.begin(), cityAddrVec.end(), searchProperty);

    if (it != cityAddrVec.end() && it->city == city && it->addr == addr) {
        owner = it->owner;
        return true;
    }
    return false;
}

bool CLandRegister::getOwner(const std::string &region, unsigned int id, std::string &owner) const {
    Property searchProperty("", "", region, id);
    auto it = std::lower_bound(regionIdVec.begin(), regionIdVec.end(), searchProperty, std::greater<Property>());

    if (it != regionIdVec.end() && it->region == region && it->id == id) {
        owner = it->owner;
        return true;
    }
    return false;
}

bool CLandRegister::newOwner(const std::string &city, const std::string &addr, const std::string &owner) {
    std::string lowercaseOwner = toLower(owner);

    Property searchProperty(city, addr, "", 0);

    auto it = std::lower_bound(cityAddrVec.begin(), cityAddrVec.end(), searchProperty);

    if (it != cityAddrVec.end() && it->city == city && it->addr == addr) {
        if (areOwnersEqual(it->owner, lowercaseOwner)) {
            return false;
        }

        auto ownerIter = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                                          [](const auto &pair, const std::string &owner) {
                                              return pair.first < owner;
                                          });

        // If the current owner is "", remove the property from the first owner
        if (areOwnersEqual(it->owner, "")) {
            if (!propertiesByOwner.empty()) {
                auto &firstOwnerProps = propertiesByOwner.front().second;
                auto propIter = std::find(firstOwnerProps.begin(), firstOwnerProps.end(), *it);
                if (propIter != firstOwnerProps.end()) {
                    firstOwnerProps.erase(propIter);
                }
            }
        } else if (ownerIter != propertiesByOwner.end() && areOwnersEqual(ownerIter->first, lowercaseOwner)) {
            ownerIter->second.erase(std::remove(ownerIter->second.begin(), ownerIter->second.end(), *it), ownerIter->second.end());
        }

        it->owner = owner;

        ownerIter = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                                     [](const auto &pair, const std::string &owner) {
                                         return pair.first < owner;
                                     });

        if (ownerIter != propertiesByOwner.end() && areOwnersEqual(ownerIter->first, lowercaseOwner)) {
            ownerIter->second.push_back(*it);
        } else {
            propertiesByOwner.insert(ownerIter, std::make_pair(lowercaseOwner, std::vector<Property>{*it}));
        }

        for (auto &property : regionIdVec) {
            if (property.city == city && property.addr == addr) {
                property.owner = owner;
                break;
            }
        }

        return true;
    }

    return false;
}


bool CLandRegister::newOwner(const std::string &region, unsigned int id, const std::string &owner) {
    std::string lowercaseOwner = toLower(owner);

    Property searchProperty("", "", region, id);

    auto it = std::lower_bound(regionIdVec.begin(), regionIdVec.end(), searchProperty, std::greater<Property>());

    if (it != regionIdVec.end() && it->region == region && it->id == id) {
        if (areOwnersEqual(it->owner, lowercaseOwner)) {
            return false;
        }

        auto ownerIter = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                                          [](const auto &pair, const std::string &owner) {
                                              return pair.first < owner;
                                          });

        // If the current owner is "", remove the property from the first owner
        if (areOwnersEqual(it->owner, "")) {
            if (!propertiesByOwner.empty()) {
                auto &firstOwnerProps = propertiesByOwner.front().second;
                auto propIter = std::find(firstOwnerProps.begin(), firstOwnerProps.end(), *it);
                if (propIter != firstOwnerProps.end()) {
                    firstOwnerProps.erase(propIter);
                }
            }
        } else if (ownerIter != propertiesByOwner.end() && areOwnersEqual(ownerIter->first, lowercaseOwner)) {
            ownerIter->second.erase(std::remove(ownerIter->second.begin(), ownerIter->second.end(), *it), ownerIter->second.end());
        }

        it->owner = owner;

        ownerIter = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                                     [](const auto &pair, const std::string &owner) {
                                         return pair.first < owner;
                                     });

        if (ownerIter != propertiesByOwner.end() && areOwnersEqual(ownerIter->first, lowercaseOwner)) {
            ownerIter->second.push_back(*it);
        } else {
            propertiesByOwner.insert(ownerIter, std::make_pair(lowercaseOwner, std::vector<Property>{*it}));
        }

        for (auto &property : cityAddrVec) {
            if (property.region == region && property.id == id) {
                property.owner = owner;
                break;
            }
        }

        return true;
    }

    return false;
}

size_t CLandRegister::count(const std::string &owner) const {
    std::string lowercaseOwner = toLower(owner);
    auto it = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                               [](const auto &pair, const std::string &owner) {
                                   return pair.first < owner;
                               });

    if (it != propertiesByOwner.end() && it->first == lowercaseOwner) {
        return it->second.size();
    }

    return 0;
}


CIterator CLandRegister::listByAddr() const {
    return CIterator(cityAddrVec);
}

CIterator CLandRegister::listByOwner(const std::string &owner) const {
    std::string lowercaseOwner = toLower(owner);

    auto it = std::lower_bound(propertiesByOwner.begin(), propertiesByOwner.end(), lowercaseOwner,
                               [](const auto &pair, const std::string &owner) {
                                   return pair.first < owner;
                               });

    if (it != propertiesByOwner.end() && toLower(it->first) == lowercaseOwner) {
        return CIterator(it->second);
    }

    return CIterator(empty);
}


#ifndef __PROGTEST__

static void test0() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (x.add("Plzen", "Evropska", "Plzen mesto", 78901));
    assert (x.add("Liberec", "Evropska", "Librec", 4552));
    
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Liberec"
            && i0.addr() == "Evropska"
            && i0.region() == "Librec"
            && i0.id() == 4552
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Plzen"
            && i0.addr() == "Evropska"
            && i0.region() == "Plzen mesto"
            && i0.id() == 78901
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());

    assert (x.count("") == 5);
    CIterator i1 = x.listByOwner("");
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Evropska"
            && i1.region() == "Vokovice"
            && i1.id() == 12345
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Technicka"
            && i1.region() == "Dejvice"
            && i1.id() == 9873
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Plzen"
            && i1.addr() == "Evropska"
            && i1.region() == "Plzen mesto"
            && i1.id() == 78901
            && i1.owner() == "");
    i1.next();
    assert (!i1.atEnd()
            && i1.city() == "Liberec"
            && i1.addr() == "Evropska"
            && i1.region() == "Librec"
            && i1.id() == 4552
            && i1.owner() == "");
    i1.next();
    assert (i1.atEnd());

    assert (x.count("CVUT") == 0);
    CIterator i2 = x.listByOwner("CVUT");
    assert (i2.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (x.newOwner("Dejvice", 9873, "CVUT"));
    assert (x.newOwner("Plzen", "Evropska", "Anton Hrabis"));
    assert (x.newOwner("Librec", 4552, "Cvut"));
    assert (x.getOwner("Prague", "Thakurova", owner) && owner == "CVUT");
    assert (x.getOwner("Dejvice", 12345, owner) && owner == "CVUT");
    assert (x.getOwner("Prague", "Evropska", owner) && owner == "");
    assert (x.getOwner("Vokovice", 12345, owner) && owner == "");
    assert (x.getOwner("Prague", "Technicka", owner) && owner == "CVUT");
    assert (x.getOwner("Dejvice", 9873, owner) && owner == "CVUT");
    assert (x.getOwner("Plzen", "Evropska", owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Plzen mesto", 78901, owner) && owner == "Anton Hrabis");
    assert (x.getOwner("Liberec", "Evropska", owner) && owner == "Cvut");
    assert (x.getOwner("Librec", 4552, owner) && owner == "Cvut");
    CIterator i3 = x.listByAddr();
    assert (!i3.atEnd()
            && i3.city() == "Liberec"
            && i3.addr() == "Evropska"
            && i3.region() == "Librec"
            && i3.id() == 4552
            && i3.owner() == "Cvut");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Plzen"
            && i3.addr() == "Evropska"
            && i3.region() == "Plzen mesto"
            && i3.id() == 78901
            && i3.owner() == "Anton Hrabis");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Evropska"
            && i3.region() == "Vokovice"
            && i3.id() == 12345
            && i3.owner() == "");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Technicka"
            && i3.region() == "Dejvice"
            && i3.id() == 9873
            && i3.owner() == "CVUT");
    i3.next();
    assert (!i3.atEnd()
            && i3.city() == "Prague"
            && i3.addr() == "Thakurova"
            && i3.region() == "Dejvice"
            && i3.id() == 12345
            && i3.owner() == "CVUT");
    i3.next();
    assert (i3.atEnd());

    assert (x.count("cvut") == 3);
    CIterator i4 = x.listByOwner("cVuT");
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Thakurova"
            && i4.region() == "Dejvice"
            && i4.id() == 12345
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Prague"
            && i4.addr() == "Technicka"
            && i4.region() == "Dejvice"
            && i4.id() == 9873
            && i4.owner() == "CVUT");
    i4.next();
    assert (!i4.atEnd()
            && i4.city() == "Liberec"
            && i4.addr() == "Evropska"
            && i4.region() == "Librec"
            && i4.id() == 4552
            && i4.owner() == "Cvut");
    i4.next();
    assert (i4.atEnd());

    assert (x.newOwner("Plzen mesto", 78901, "CVut"));
    assert (x.count("CVUT") == 4);
    CIterator i5 = x.listByOwner("CVUT");
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Thakurova"
            && i5.region() == "Dejvice"
            && i5.id() == 12345
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Prague"
            && i5.addr() == "Technicka"
            && i5.region() == "Dejvice"
            && i5.id() == 9873
            && i5.owner() == "CVUT");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Liberec"
            && i5.addr() == "Evropska"
            && i5.region() == "Librec"
            && i5.id() == 4552
            && i5.owner() == "Cvut");
    i5.next();
    assert (!i5.atEnd()
            && i5.city() == "Plzen"
            && i5.addr() == "Evropska"
            && i5.region() == "Plzen mesto"
            && i5.id() == 78901
            && i5.owner() == "CVut");
    i5.next();
    assert (i5.atEnd());

    assert (x.del("Liberec", "Evropska"));
    assert (x.del("Plzen mesto", 78901));
    assert (x.count("cvut") == 2);
    CIterator i6 = x.listByOwner("cVuT");
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Thakurova"
            && i6.region() == "Dejvice"
            && i6.id() == 12345
            && i6.owner() == "CVUT");
    i6.next();
    assert (!i6.atEnd()
            && i6.city() == "Prague"
            && i6.addr() == "Technicka"
            && i6.region() == "Dejvice"
            && i6.id() == 9873
            && i6.owner() == "CVUT");
    i6.next();
    assert (i6.atEnd());

    assert (x.add("Liberec", "Evropska", "Librec", 4552));
}

static void test1() {
    CLandRegister x;
    std::string owner;

    assert (x.add("Prague", "Thakurova", "Dejvice", 12345));
    assert (x.add("Prague", "Evropska", "Vokovice", 12345));
    assert (x.add("Prague", "Technicka", "Dejvice", 9873));
    assert (!x.add("Prague", "Technicka", "Hradcany", 7344));
    assert (!x.add("Brno", "Bozetechova", "Dejvice", 9873));
    assert (!x.getOwner("Prague", "THAKUROVA", owner));
    assert (!x.getOwner("Hradcany", 7343, owner));
    CIterator i0 = x.listByAddr();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Evropska"
            && i0.region() == "Vokovice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Technicka"
            && i0.region() == "Dejvice"
            && i0.id() == 9873
            && i0.owner() == "");
    i0.next();
    assert (!i0.atEnd()
            && i0.city() == "Prague"
            && i0.addr() == "Thakurova"
            && i0.region() == "Dejvice"
            && i0.id() == 12345
            && i0.owner() == "");
    i0.next();
    assert (i0.atEnd());

    assert (x.newOwner("Prague", "Thakurova", "CVUT"));
    assert (!x.newOwner("Prague", "technicka", "CVUT"));
    assert (!x.newOwner("prague", "Technicka", "CVUT"));
    assert (!x.newOwner("dejvice", 9873, "CVUT"));
    assert (!x.newOwner("Dejvice", 9973, "CVUT"));
    assert (!x.newOwner("Dejvice", 12345, "CVUT"));
    assert (x.count("CVUT") == 1);
    CIterator i1 = x.listByOwner("CVUT");
    assert (!i1.atEnd()
            && i1.city() == "Prague"
            && i1.addr() == "Thakurova"
            && i1.region() == "Dejvice"
            && i1.id() == 12345
            && i1.owner() == "CVUT");
    i1.next();
    assert (i1.atEnd());

    assert (!x.del("Brno", "Technicka"));
    assert (!x.del("Karlin", 9873));
    assert (x.del("Prague", "Technicka"));
    assert (!x.del("Prague", "Technicka"));
    assert (!x.del("Dejvice", 9873));
}

int main(void) {
    test0();
    test1();
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
