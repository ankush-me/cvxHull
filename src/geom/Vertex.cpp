
#include "Vertex.h"
#include "ConflictGraph.h"

Vertex::Vertex(int _idx) : node_idx(_idx), visible(true), cList(new ConflictList(false)) {}

