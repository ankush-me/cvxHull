
#include "Vertex.h"
#include "ConflictGraph.h"

Vertex::Vertex(int _idx=-1) : node_idx(_idx), visible(true), cList(new ConflictList(false)) {}

