#ifndef TYPES_H
#define TYPES_H

#include <utility>
#include <vector>
#include <map>
#include <set>

#define N 200

using namespace std;

typedef vector<int> TPath;
typedef pair<TPath, TPath> TPaths;
typedef pair<int, int> TPathLength;
typedef pair<long, long> TPathCost;

typedef pair<int, int> TVertex;
typedef TVertex TVertices[N];

typedef pair<int, int> TInsertionCost;
typedef vector<TInsertionCost> TInsertionCosts;

typedef pair<int, int> TEdge;
typedef pair<TEdge, TEdge> TEdges;

typedef pair<TPaths, TPathCost> TSolution;
typedef vector<TSolution> TSolutions;

enum MoveState
{
    APPLICABLE,
    NOT_APPLICABLE,
    FUTURE_APPLICABLE
};

class NearestNeighbourQueue
{
public:
    void add(int v, int n, int d)
    {
        bool isin = false;
        for (int i = 0; i < this->neighbours[v].size(); i++)
        {
            if (this->neighbours[v][i].first == n)
            {
                isin = true;
                break;
            }
        }

        if (!isin)
        {
            printf("Adding %d, %d\n", n, d);
            this->neighbours[v].push_back(make_pair(n, d));
            this->idx[v] = 0;
        }
    }

    void remove(int v, int n)
    {
        // vec.erase(vec.begin() + index);
        for (int i = 0; i < this->neighbours.size(); i++)
        {
            if (this->neighbours[v][i].first == n)
            {
                this->neighbours[v].erase(this->neighbours[v].begin() + i);
                break;
            }
        }
    }

    void reset()
    {
        for (map<int, int> :: iterator it = this->idx.begin(); it != idx.end(); it++){
            it->second = 0;
        }
    }

    bool end(int v)
    {
        if (this->idx[v] == this->neighbours[v].size())
        {
            return true;
        }
        return false;
    }

    pair<int, int> topnext(int v)
    {
        pair<int, int> retval = this->neighbours[v][this->idx[v]];
        this->idx[v] += 1;

        return retval;
    }

    void sortit()
    {
        // loop over the map
        for (map<int, vector<pair<int, int> > >::iterator it = this->neighbours.begin(); it != this->neighbours.end(); ++it)
        {
            // sort the vector using a lambda function to compare the second element of each pair
            sort(it->second.begin(), it->second.end(), this->compare);
        }
    }

private:
    map<int, vector<pair<int, int> > > neighbours;
    map<int, int> idx;
    static bool compare(const pair<int, int> &a, const pair<int, int> &b) { return a.second < b.second; }
};

typedef NearestNeighbourQueue Tnnq;

#endif