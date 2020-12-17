#include <vector>
using std::vector;

class node
{
public:
    // default constructor
    node();
    // constructor
    node(int id);

    int id;
    // frequency
    int visit_count;
    // out degree
    vector<int> outlinks;

    // operator overloading
    bool operator<(const node &compare)
    {
        return visit_count < compare.visit_count;
    }
};

struct LessThanByVisit
{
    bool operator()(const node &lhs, const node &rhs) const
    {
        return lhs.visit_count < rhs.visit_count;
    }
};