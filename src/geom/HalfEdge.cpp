#include "HalfEdge.h"
#include <iostream>

using namespace std;

HalfEdge::HalfEdge(Vertex::Ptr _org, Vertex::Ptr _dst, Face::Ptr _face) : org(_org), dst(_dst), face(_face) {}


bool HalfEdge::onHorizon() {
	return (!twin)? false : (!face->toDelete && twin->face->toDelete);
}

bool HalfEdge::matches(Vertex::Ptr p, Vertex::Ptr  q) {
	return ((*(org->pt) == *(p->pt) && *(dst->pt) == *(q->pt))
			|| (*(org->pt) == *(q->pt) && *(dst->pt) == *(p->pt)));
}

void HalfEdge::findHorizon(std::vector<HalfEdge::Ptr> &horizon) {
	if (onHorizon()) {
		cout << "on horizon"<<endl;
		if (horizon.size() > 0 && this == horizon[0].get()) {
			return;
		} else {
			horizon.push_back(shared_from_this());
			next->findHorizon(horizon);
		}
	} else {
		cout << "NOT on horizon"<<endl;
		if (twin) {
			twin->next->findHorizon(horizon);
		}
	}
}

