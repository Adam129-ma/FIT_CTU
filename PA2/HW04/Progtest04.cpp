#ifndef __PROGTEST__
#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <memory>
#include <functional>
#endif /* __PROGTEST__ */


class CComponent {
public:
    CComponent() = default;
    virtual ~CComponent() = default;

    virtual std::ostream& print(std::ostream& os) const = 0;
    virtual CComponent* clone() const = 0;
    std::string indent;
};

class CComputer {
public:
    CComputer(const std::string& name);
    CComputer(const CComputer& other);
    ~CComputer();

    CComputer& operator=(CComputer other);

    CComputer & addComponent(const CComponent& component);
    CComputer & addAddress(const std::string& address);

    std::string getName() const;

    friend std::ostream& operator<<(std::ostream& os, const CComputer& computer);
    std::string indentCComp;
    bool last;
private:
    std::string name;
    std::vector<std::unique_ptr<CComponent>> components;
    std::vector<std::string> addresses;
};

CComputer::CComputer(const std::string& name) : name(name) {}

CComputer::CComputer(const CComputer& other) : name(other.name) {
    if (this != &other) {
        components.clear();

        for (const auto& component : other.components) {
            components.emplace_back(component->clone());
        }

        addresses = other.addresses;
    }
}

CComputer::~CComputer() {}

CComputer& CComputer::operator=(CComputer other) {
    std::swap(name, other.name);
    std::swap(components, other.components);
    std::swap(addresses, other.addresses);
    return *this;
}

CComputer & CComputer::addComponent(const CComponent& component) {
    components.emplace_back(component.clone());
    return *this;
}

CComputer & CComputer::addAddress(const std::string& address) {
    addresses.push_back(address);
    return *this;
}

std::string CComputer::getName() const {
    return name;
}

std::ostream& operator<<(std::ostream& os, const CComputer& computer) {
    if(!computer.last)
        {os << "Host: "<< computer.name << "\n";}
    else
    {os << "Host: "<< computer.name << "\n";}

    for (const auto& address : computer.addresses) {
        os << computer.indentCComp;
        os << "+-" << address << "\n";
    }

    size_t i = 0;
    for (const auto& component : computer.components) {
        os << computer.indentCComp;
        if (i == computer.components.size() - 1) {
            os << "\\-";
            component->indent=computer.indentCComp+"  ";
        } else {
            os << "+-";
            component->indent=computer.indentCComp+"| ";
        }
        component->print(os) ;
        i++;
    }
    return os;
}

class CNetwork {
public:
    CNetwork(const std::string& name);
    CNetwork(const CNetwork& other);
    ~CNetwork();

    CNetwork& operator=(CNetwork other);

    CNetwork& addComputer(const CComputer& computer);
    CComputer* findComputer(const std::string& name) const;

    friend std::ostream& operator<<(std::ostream& os, const CNetwork& network);
    std::string indentCNetw;

private:
    std::string name;
    std::vector<std::unique_ptr<CComputer>> computers;
};

CNetwork::CNetwork(const std::string& name) : name(name) {}

CNetwork::CNetwork(const CNetwork& other) : name(other.name) {
    if (this != &other) {
        computers.clear();
        for (const auto& computer : other.computers) {
            computers.push_back(std::make_unique<CComputer>(*computer));
        }
    }
}

CNetwork::~CNetwork() {}

CNetwork& CNetwork::operator=(CNetwork other) {
    std::swap(name, other.name);
    std::swap(computers, other.computers);
    return *this;
}

CNetwork& CNetwork::addComputer(const CComputer& computer) { // Updated method definition
    computers.emplace_back(std::make_unique<CComputer>(computer));
    return *this;
}

CComputer* CNetwork::findComputer(const std::string& name) const {
    for (const auto& computer : computers) {
        if (computer->getName() == name) {
            return computer.get();
        }
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const CNetwork& network) {
    os << "Network: " << network.name << "\n";
    size_t i = 0;
    for (const auto& computer : network.computers) {
        if (i == network.computers.size() - 1){
            (*computer).last=true;
            (*computer).indentCComp="  ";
            os << "\\-";
        }
        else{
            (*computer).last=false;
            (*computer).indentCComp="| ";
            os << "+-";
        }
        os << (*computer);
        ++i;
    }
    for(const auto& computer : network.computers){
        (*computer).indentCComp = "";
    }
    return os;
}


class CCPU : public CComponent {
public:
    CCPU(int cores, int frequency);
    std::ostream& print(std::ostream& os) const override;
    CCPU* clone() const override;
private:
    int m_Cores;
    int m_Frequency;
};

CCPU::CCPU(int cores, int frequency)
        : m_Cores(cores), m_Frequency(frequency) {}


std::ostream& CCPU::print(std::ostream& os) const {
    os << "CPU, "<< m_Cores << " cores @ " << m_Frequency << "MHz\n";
    return os;
}

CCPU* CCPU::clone() const {
    return new CCPU(*this);
}

class CMemory : public CComponent {
public:
    CMemory(int size);
    std::ostream& print(std::ostream& os) const override;
    CMemory* clone() const override;
private:
    int m_Size;
};

CMemory::CMemory(int size)
        : m_Size(size) {}

std::ostream& CMemory::print(std::ostream& os) const {
    os << "Memory, " << m_Size << " MiB\n";
    return os;
}

CMemory* CMemory::clone() const {
    return new CMemory(*this);
}

class CDisk : public CComponent {
public:
    enum DiskType { SSD, MAGNETIC };
    CDisk(DiskType type, int size);
    CDisk(const CDisk& other);
    CDisk addPartition(int sizeGiB, const std::string& identifier); // Method chaining
    std::ostream& print(std::ostream& os) const override;
    CDisk* clone() const override;
private:
    DiskType m_Type;
    int m_Size;
    std::vector<std::pair<int, std::string>> m_Partitions;
};

CDisk::CDisk(DiskType type, int size)
        : m_Type(type), m_Size(size) {}

CDisk::CDisk(const CDisk& other) : m_Type(other.m_Type), m_Size(other.m_Size){
    if (this != &other) {
        m_Partitions.clear();
        m_Partitions = other.m_Partitions;
    }
}
CDisk CDisk::addPartition(int sizeGiB, const std::string& identifier) {
    m_Partitions.emplace_back(sizeGiB, identifier);
    return *this;
}

std::ostream& CDisk::print(std::ostream& os) const {
    if (m_Type == SSD) {
        os << "SSD, "<< m_Size <<" GiB\n";
    } else {
        os << "HDD, "<< m_Size << " GiB\n";
    }
    size_t i = 0;
    for (const auto& partition : m_Partitions) {
        if (i == m_Partitions.size() - 1) {
            os << indent +"\\-";
        } else {
            os << indent +"+-";
        }
        os << "[" << i << "]: " << partition.first << " GiB, " << partition.second << "\n";
        i++;
    }
    return os;
}

CDisk* CDisk::clone() const {
    return new CDisk(*this);
}

#ifndef __PROGTEST__
template<typename T_>
std::string toString ( const T_ & x )
{
  std::ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ()
{
  CNetwork n ( "FIT network" );
  n . addComputer (
        CComputer ( "progtest.fit.cvut.cz" ) .
          addAddress ( "147.32.232.142" ) .
          addComponent ( CCPU ( 8, 2400 ) ) .
          addComponent ( CCPU ( 8, 1200 ) ) .
          addComponent ( CDisk ( CDisk::MAGNETIC, 1500 ) .
            addPartition ( 50, "/" ) .
            addPartition ( 5, "/boot" ) .
            addPartition ( 1000, "/var" ) ).
          addComponent ( CDisk ( CDisk::SSD, 60 ) .
            addPartition ( 60, "/data" )  ) .
          addComponent ( CMemory ( 2000 ) ) .
          addComponent ( CMemory ( 2000 ) ) ).
      addComputer (
        CComputer ( "courses.fit.cvut.cz" ) .
          addAddress ( "147.32.232.213" ) .
          addComponent ( CCPU ( 4, 1600 ) ) .
          addComponent ( CMemory ( 4000 ) ).
          addComponent ( CDisk ( CDisk::MAGNETIC, 2000 ) .
            addPartition ( 100, "/" )   .
            addPartition ( 1900, "/data" ) )) .
      addComputer (
        CComputer ( "imap.fit.cvut.cz" ) .
          addAddress ( "147.32.232.238" ) .
          addComponent ( CCPU ( 4, 2500 ) ) .
          addAddress ( "2001:718:2:2901::238" ) .
          addComponent ( CMemory ( 8000 ) ) );
  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );
  CNetwork x = n;
  auto c = x . findComputer ( "imap.fit.cvut.cz" );
  assert ( toString ( *c ) ==
    "Host: imap.fit.cvut.cz\n"
    "+-147.32.232.238\n"
    "+-2001:718:2:2901::238\n"
    "+-CPU, 4 cores @ 2500MHz\n"
    "\\-Memory, 8000 MiB\n" );
  c -> addComponent ( CDisk ( CDisk::MAGNETIC, 1000 ) .
         addPartition ( 100, "system" ) .
         addPartition ( 200, "WWW" ) .
         addPartition ( 700, "mail" ) );
  assert ( toString ( x ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  +-Memory, 8000 MiB\n"
    "  \\-HDD, 1000 GiB\n"
    "    +-[0]: 100 GiB, system\n"
    "    +-[1]: 200 GiB, WWW\n"
    "    \\-[2]: 700 GiB, mail\n" );
  assert ( toString ( n ) ==
    "Network: FIT network\n"
    "+-Host: progtest.fit.cvut.cz\n"
    "| +-147.32.232.142\n"
    "| +-CPU, 8 cores @ 2400MHz\n"
    "| +-CPU, 8 cores @ 1200MHz\n"
    "| +-HDD, 1500 GiB\n"
    "| | +-[0]: 50 GiB, /\n"
    "| | +-[1]: 5 GiB, /boot\n"
    "| | \\-[2]: 1000 GiB, /var\n"
    "| +-SSD, 60 GiB\n"
    "| | \\-[0]: 60 GiB, /data\n"
    "| +-Memory, 2000 MiB\n"
    "| \\-Memory, 2000 MiB\n"
    "+-Host: courses.fit.cvut.cz\n"
    "| +-147.32.232.213\n"
    "| +-CPU, 4 cores @ 1600MHz\n"
    "| +-Memory, 4000 MiB\n"
    "| \\-HDD, 2000 GiB\n"
    "|   +-[0]: 100 GiB, /\n"
    "|   \\-[1]: 1900 GiB, /data\n"
    "\\-Host: imap.fit.cvut.cz\n"
    "  +-147.32.232.238\n"
    "  +-2001:718:2:2901::238\n"
    "  +-CPU, 4 cores @ 2500MHz\n"
    "  \\-Memory, 8000 MiB\n" );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
