#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <compare>

class CDate {
public:
    CDate(int y,
          int m,
          int d)
            : m_Y(y),
              m_M(m),
              m_D(d) {
    }

    std::strong_ordering operator<=>(const CDate &other) const = default;

    friend std::ostream &operator<<(std::ostream &os,
                                    const CDate &d) {
        return os << d.m_Y << '-' << d.m_M << '-' << d.m_D;
    }

private:
    int m_Y;
    int m_M;
    int m_D;
};

enum class ESortKey {
    NAME,
    BIRTH_DATE,
    ENROLL_YEAR
};
#endif /* __PROGTEST__ */
int number_of_students = 0;
class CStudent {
public:
    CStudent(const std::string &name, const CDate &born, int enrolled)
            : name(name), born(born), enrolled(enrolled), order(number_of_students++){

        std::istringstream iss(name);
        std::string part;
        while (iss >> part) {
            std::transform(part.begin(), part.end(), part.begin(), ::tolower);
            part += ' ';
            normalized_name_parts.push_back(part);
        }
        std::sort(normalized_name_parts.begin(), normalized_name_parts.end());
        for (const auto &part : normalized_name_parts) {
            normalized_name += part;
        }
    }

    bool operator==(const CStudent &other) const {
        return (enrolled == other.enrolled) &&
               (born == other.born) &&
               (name == other.name);
    }

    bool operator!=(const CStudent &other) const {
        return !(*this == other);
    }

    bool operator<(const CStudent &other) const {
        if (enrolled != other.enrolled)
            return enrolled < other.enrolled;
        if (born != other.born)
            return born < other.born;
        return name < other.name;
    }
    std::string name;
    std::string normalized_name;
    std::vector<std::string> normalized_name_parts;
    CDate born;
    int enrolled;
    int order;
private:
};

class CFilter {
public:
    CFilter() : born_before(INT_MAX, INT_MAX, INT_MAX), born_after(INT_MIN, INT_MIN, INT_MIN), enrolled_before(INT_MAX), enrolled_after(INT_MIN) {}

    CFilter &name(const std::string &name) {
        names.push_back(normalizeName(name));
        return *this;
    }

    CFilter &bornBefore(const CDate &date) {
        born_before = date;
        return *this;
    }

    CFilter &bornAfter(const CDate &date) {
        born_after = date;
        return *this;
    }

    CFilter &enrolledBefore(int year) {
        enrolled_before = year;
        return *this;
    }

    CFilter &enrolledAfter(int year) {
        enrolled_after = year;
        return *this;
    }

    bool matches(const CStudent &student) const {
        if (student.enrolled >= enrolled_before || student.enrolled <= enrolled_after) {
            return false;
        }

        if (student.born >= born_before || student.born <= born_after) {
            return false;
        }

        if (names.empty()) {
            return true;
        }

        for (const auto &filter_name : names) {
            if (student.normalized_name == filter_name) {
                return true;
            }
        }

        return false;
    }

private:
    std::vector<std::string> names;
    CDate born_before;
    CDate born_after;
    int enrolled_before;
    int enrolled_after ;

    std::string normalizeName(const std::string &name) const {
        std::string normalized;
        std::istringstream iss(name);
        std::vector<std::string> parts;
        std::string part;
        while (iss >> part) {
            std::transform(part.begin(), part.end(), part.begin(), [](unsigned char c){ return std::tolower(c); });
            parts.push_back(part);
        }
        std::sort(parts.begin(), parts.end());
        for (const auto &p : parts) {
            normalized += p + ' ';
        }
        return normalized;
    }
};

class CSort {
public:
    CSort() {}

    CSort &addKey(ESortKey key, bool ascending) {
        sorting_criteria.push_back({key, ascending});
        return *this;
    }
    struct SortingCriterion {
        ESortKey key;
        bool ascending;
    };
    std::vector<SortingCriterion> sorting_criteria;
private:
};

class CStudyDept {
public:
    CStudyDept() {}

    bool addStudent(const CStudent &x) {
        if (students.find(x) != students.end()) {
            return false;
        }
        students.insert(x);
        return true;
    }

    bool delStudent(const CStudent &x) {
        size_t removed = students.erase(x);

        return removed > 0;
    }


    std::list<CStudent> search(const CFilter &flt, const CSort &sortOpt) const {
        std::list<CStudent> filtered_students;

        for (const auto &student : students) {
            if (flt.matches(student)) {
                filtered_students.push_back(student);
            }
        }

        if (!sortOpt.sorting_criteria.empty()) {
            auto sortByKey = [&](const CStudent &a, const CStudent &b) {
                auto sortByCriterion = [&](const ESortKey &key, bool asc) {
                    if (key == ESortKey::NAME) {
                        return asc ? a.name < b.name : b.name < a.name;
                    } else if (key == ESortKey::BIRTH_DATE) {
                        return asc ? a.born < b.born : b.born < a.born;
                    } else if (key == ESortKey::ENROLL_YEAR) {
                        return asc ? a.enrolled < b.enrolled : b.enrolled < a.enrolled;
                    }
                    return false;
                };

                for (const auto &criteria : sortOpt.sorting_criteria) {
                    if (sortByCriterion(criteria.key, criteria.ascending)) {
                        return true;
                    } else if (sortByCriterion(criteria.key, !criteria.ascending)) {
                        return false;
                    }
                }
                return a.order < b.order;
            };

            filtered_students.sort(sortByKey);
        } else {
            filtered_students.sort([](const CStudent &a, const CStudent &b) {
                return a.order < b.order;
            });
        }

        return filtered_students;
    }

    std::set<std::string> suggest(const std::string &name) const {
        std::set<std::string> suggested_names;

        std::vector<std::string> name_parts;
        std::istringstream iss(name);
        std::string part;
        while (iss >> part) {
            std::transform(part.begin(), part.end(), part.begin(), ::tolower);
            part += ' ';
            name_parts.push_back(part);
        }

        for (const auto &student : students) {
            std::string normalized_student_name = student.normalized_name;

            bool contains_all_words = true;
            for (const auto &word : name_parts) {
                if (normalized_student_name.find(word) == std::string::npos) {
                    contains_all_words = false;
                    break;
                }
            }

            if (contains_all_words) {
                suggested_names.insert(student.name);
            }
        }

        return suggested_names;
    }

private:
    std::set<CStudent> students;
    std::vector<CStudent> insertion_order;

};

#ifndef __PROGTEST__

int main(void) {
    CStudyDept x0;
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1980, 4, 11), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1980, 4, 11), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2010));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2010)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("James Bond", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) == CStudent("James Bond", CDate(1997, 6, 17), 2016)));
    assert (CStudent("James Bond", CDate(1980, 4, 11), 2010) != CStudent("Peter Peterson", CDate(1997, 6, 17), 2016));
    assert (!(CStudent("James Bond", CDate(1980, 4, 11), 2010) ==
              CStudent("Peter Peterson", CDate(1997, 6, 17), 2016)));
    assert (x0.addStudent(CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014)));
    assert (x0.addStudent(CStudent("John Taylor", CDate(1981, 6, 30), 2012)));
    assert (x0.addStudent(CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)));
    assert (x0.addStudent(CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1982, 7, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 8, 16), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 17), 2013)));
    assert (x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2012)));
    assert (x0.addStudent(CStudent("Bond James", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter(), CSort()) == (std::list<CStudent>
            {
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter(), CSort().addKey(ESortKey::NAME, false)) == (std::list<CStudent>
            {
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011),
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013)
            }));
    assert (x0.search(CFilter(),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("Peter John Taylor", CDate(1984, 1, 17), 2017),
                    CStudent("John Peter Taylor", CDate(1983, 7, 13), 2014),
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012),
                    CStudent("Peter Taylor", CDate(1982, 2, 23), 2011)
            }));
    assert (x0.search(CFilter().name("james bond"),
                      CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
                              ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012)
            }));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (x0.search(CFilter().name("james"), CSort().addKey(ESortKey::NAME, true)) == (std::list<CStudent>
            {
            }));
    assert (x0.suggest("peter") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor",
                    "Peter Taylor"
            }));
    assert (x0.suggest("bond") == (std::set<std::string>
            {
                    "Bond James",
                    "James Bond"
            }));
    assert (x0.suggest("peter joHn") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (x0.suggest("peter joHn bond") == (std::set<std::string>
            {
            }));
    assert (x0.suggest("pete") == (std::set<std::string>
            {
            }));
    assert (x0.suggest("peter joHn PETER") == (std::set<std::string>
            {
                    "John Peter Taylor",
                    "Peter John Taylor"
            }));
    assert (!x0.addStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    assert (x0.search(CFilter().bornAfter(CDate(1980, 4, 11)).bornBefore(CDate(1983, 7, 13)).name("John Taylor").name(
            "james BOND"), CSort().addKey(ESortKey::ENROLL_YEAR, false).addKey(ESortKey::BIRTH_DATE, false).addKey(
            ESortKey::NAME, true)) == (std::list<CStudent>
            {
                    CStudent("James Bond", CDate(1982, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 8, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 17), 2013),
                    CStudent("Bond James", CDate(1981, 7, 16), 2013),
                    CStudent("James Bond", CDate(1981, 7, 16), 2012),
                    CStudent("John Taylor", CDate(1981, 6, 30), 2012)
            }));
    assert (!x0.delStudent(CStudent("James Bond", CDate(1981, 7, 16), 2013)));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
