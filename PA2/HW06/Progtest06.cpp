#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

template <typename M_>
class CContest
{
public:
    CContest() {};

    bool isOrdered(function<int(const M_ &)> comparator) const
    {
        try
        {
            results(comparator);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    list<string> results(const int (*comparator)(const M_ &x)) const
    {
        Komparator ownComp(comparator);
        return results<Komparator>(ownComp);
    }

    template <class TC>
    list<string> results(const TC &comp) const
    {
        list<string> res;
        size_t n = nOfPlayers;
        vector<int> inDegree(n, 0);
        vector<vector<int>> adjList(n);
        vector<int> matchResults(matches1.size());
        for (long unsigned int i = 0; i < matches1.size(); i++)
        {
            int a = matches1[i] / 100;
            int b = matches1[i] % 100;
            int result = comp(matches2[i]);
            if (result > 0)
            {
                inDegree[b]++;
                adjList[a].push_back(b);
            }
            else if (result < 0)
            {
                inDegree[a]++;
                adjList[b].push_back(a);
            }
            else
            {
                throw logic_error("OrderingDoesNotExist");
            }
            matchResults[i] = result;
        }
        while (res.size() < n)
        {
            int player = -1;
            for (long unsigned int i = 0; i < n; i++)
            {
                if (inDegree[i] == 0)
                {
                    if (player != -1)
                    {
                        throw logic_error("OrderingDoesNotExist");
                    }
                    player = i;
                }
            }
            if (player == -1)
            {
                throw logic_error("OrderingDoesNotExist");
            }
            res.push_back(ReverseNameMap.at(player));
            inDegree[player] = -1;
            for (int v : adjList[player])
            {
                inDegree[v]--;
            }
        }
        return res;
    }

    CContest &addMatch(const string a, const string b, const M_ &x)
    {
        auto it1 = NameMap.emplace(a, nOfPlayers);
        bool inserted1 = it1.second;
        long unsigned int player1 = it1.first->second;
        if (inserted1)
        {
            ReverseNameMap.emplace(nOfPlayers, a);
            ++nOfPlayers;
        }

        auto it2 = NameMap.emplace(b, nOfPlayers);
        bool inserted2 = it2.second;
        long unsigned int player2 = it2.first->second;
        if (inserted2)
        {
            ReverseNameMap.emplace(nOfPlayers, b);
            ++nOfPlayers;
        }

        if (find(matches1.begin(), matches1.end(), player1 * 100 + player2) != matches1.end() || find(matches1.begin(), matches1.end(), player2 * 100 + player1) != matches1.end())
        {
            throw logic_error("MatchIsADuplicate");
        }

        matches1.push_back(player1 * 100 + player2);
        matches2.push_back(x);
        if (player1 >= playerGraph.size())
        {
            playerGraph.resize(player1 + 1);
            incomingEdges.resize(player1 + 1);
        }
        if (player2 >= playerGraph.size())
        {
            playerGraph.resize(player2 + 1);
            incomingEdges.resize(player2 + 1);
        }
        playerGraph[player1].insert(player2);
        playerGraph[player2].insert(player1);
        ++incomingEdges[player2];
        return *this;
    }

private:
    class Komparator
    {
    private:
        int (*comparator)(const M_ &x);

    public:
        Komparator(const int (*c)(const M_ &x))
        {
            comparator = c;
        }

        int operator()(const M_ &x) const
        {
            return comparator(x);
        }
    };

    int nOfPlayers = 0;
    vector<int> matches1;
    vector<M_> matches2;
    vector<int> incomingEdges;
    map<string, int> NameMap;
    map<int, string> ReverseNameMap;
    vector<set<int>> playerGraph;
};

#ifndef __PROGTEST__

struct CMatch
{
public:
    CMatch(int a,
           int b)
        : m_A(a),
          m_B(b)
    {
    }

    int m_A;
    int m_B;
};

class HigherScoreThreshold
{
public:
    HigherScoreThreshold(int diffAtLeast)
        : m_DiffAtLeast(diffAtLeast)
    {
    }

    int operator()(const CMatch &x) const
    {
        return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast);
    }

private:
    int m_DiffAtLeast;
};

int HigherScore(const CMatch &x)
{
    return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main(void)
{
    CContest<CMatch> x;

    x.addMatch("C++", "Pascal", CMatch(10, 3))
        .addMatch("C++", "Java", CMatch(8, 1))
        .addMatch("Pascal", "Basic", CMatch(40, 0))
        .addMatch("Java", "PHP", CMatch(6, 2))
        .addMatch("Java", "Pascal", CMatch(7, 3))
        .addMatch("PHP", "Basic", CMatch(10, 0));

    assert(!x.isOrdered(HigherScore));
    try
    {
        list<string> res = x.results(HigherScore);
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e)
    {
    }
    catch (...)
    {
        assert("Invalid exception thrown!" == nullptr);
    }

    x.addMatch("PHP", "Pascal", CMatch(3, 6));

    assert(x.isOrdered(HigherScore));
    try
    {
        list<string> res = x.results(HigherScore);
        assert((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
    }
    catch (...)
    {
        assert("Unexpected exception!" == nullptr);
    }

    assert(!x.isOrdered(HigherScoreThreshold(3)));
    try
    {
        list<string> res = x.results(HigherScoreThreshold(3));
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e)
    {
    }
    catch (...)
    {
        assert("Invalid exception thrown!" == nullptr);
    }

    assert(x.isOrdered([](const CMatch &x)
                       { return (x.m_A < x.m_B) - (x.m_B < x.m_A); }));
    try
    {
        list<string> res = x.results([](const CMatch &x)
                                     { return (x.m_A < x.m_B) - (x.m_B < x.m_A); });
        assert((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
    }
    catch (...)
    {
        assert("Unexpected exception!" == nullptr);
    }

    CContest<bool> y;

    y.addMatch("Python", "PHP", true)
        .addMatch("PHP", "Perl", true)
        .addMatch("Perl", "Bash", true)
        .addMatch("Bash", "JavaScript", true)
        .addMatch("JavaScript", "VBScript", true);

    assert(y.isOrdered([](bool v)
                       { return v ? 10 : -10; }));
    try
    {
        list<string> res = y.results([](bool v)
                                     { return v ? 10 : -10; });
        assert((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
    }
    catch (...)
    {
        assert("Unexpected exception!" == nullptr);
    }

    y.addMatch("PHP", "JavaScript", false);
    assert(!y.isOrdered([](bool v)
                        { return v ? 10 : -10; }));
    try
    {
        list<string> res = y.results([](bool v)
                                     { return v ? 10 : -10; });
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e)
    {
    }
    catch (...)
    {
        assert("Invalid exception thrown!" == nullptr);
    }

    try
    {
        y.addMatch("PHP", "JavaScript", false);
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e)
    {
    }
    catch (...)
    {
        assert("Invalid exception thrown!" == nullptr);
    }

    try
    {
        y.addMatch("JavaScript", "PHP", true);
        assert("Exception missing!" == nullptr);
    }
    catch (const logic_error &e)
    {
    }
    catch (...)
    {
        assert("Invalid exception thrown!" == nullptr);
    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
