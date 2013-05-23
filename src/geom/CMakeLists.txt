
# defines basic geometric structures and conflict graph

add_library(geom ConflictGraph.cpp
                 Face.cpp
                 HalfEdge.cpp
                 Vertex.cpp
                 SubD.cpp)
                 
target_link_libraries(geom utils)
