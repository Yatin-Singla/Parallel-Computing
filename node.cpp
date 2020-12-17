#include "node.h"

node::node()
{
    this->id = -1;
    this->visit_count = 0;
}

node::node(int id)
{
    this->id = id;
    this->visit_count = 0;
}
