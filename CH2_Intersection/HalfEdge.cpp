namespace HE {
struct HalfEdge;
struct Vertex;
struct Face;

struct HalfEdge {
  HalfEdge* next;
  HalfEdge* twin;
  Vertex* vert;
  Face* face;
};

struct Vertex {
  double x;
  double y;
  double z;
  HalfEdge* edge;
};

struct Face {
  HalfEdge* edge;
};
};

int main() {
  
  {
    // query vertices and faces which border a half-edge.
  HE::HalfEdge* edge;

  HE::Vertex* vert1 = edge->vert;
  HE::Vertex* vert2 = edge->twin->vert;

  HE::Face* face1 = edge->face;
  HE::Face* face2 = edge->twin->face;
  }

  {
    // query half-edges adjacent to a face.
    HE::Face* face;
    HE::HalfEdge* edge = face->edge;
    do {
      // do something
      edge = edge->next;
    } while (edge != face->edge);
  }

  {
    // query half-edges adjacent to a vertex
    HE::Vertex* vert;
    HE::HalfEdge* edge = vert->edge;
    do {
      // do something
      edge = edge->twin->next;
    } while (edge != vert->edge);
  }

}