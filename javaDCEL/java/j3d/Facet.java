/*
 * @(#)Facet.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
package j3d;

/**
 * A triangular facet of a 3D polytope.  Facets contain a list of
 * incident vertices and edges.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Facet extends Face {

   private Vertex v[];     // List of incident vertices
   private Edge e[];       // Boundary edges of the facet
   private Vector normal;  
   private boolean filled; // Shade the facet or just draw an outline
   private boolean marked; // Mark this facet for deletion

   public Facet(Vertex a, Vertex b, Vertex c) {
      this.v = new Vertex[3];
      this.e = new Edge[3];
      this.v[0] = a;
      this.v[1] = b;
      this.v[2] = c;
      this.filled = true;
      this.marked = false;
      this.normal = Vertex.subtract(b,a).cross(Vertex.subtract(c,a));
      this.normal.normalize();
      createEdges();
   }      

   public Facet(Vertex a, Vertex b, Vertex c, Vertex d) {
      this(a,b,c);
      orient(d);
   }

   public Vector getNormal() {
      return this.normal;
   }

   public Vertex getVertex(int index) {
      return v[index];
   }

   public int getVertexCount() {
      return v.length;
   }
   
   public Edge getEdge(int index) {
      if (index < 3 && index >= 0) {
         return e[index];
      } else {
         return null;
      }
   }

   public int getEdgeCount() {
      return e.length;
   }

/**
 * Should this facet be shaded or should only the edges be drawn?
 */
   public boolean isFilled() {
      return this.filled;
   }

   public void setFilled(boolean filled) {
      this.filled = filled;
   }

/**
 * Is this facet set to be removed?
 */
   public boolean isMarked() {
      return this.marked;
   }

   public void setMarked(boolean marked) {
      this.marked = marked;
   }

/**
 * Is the given vertex in front of or behind this face?
 */
   public boolean behind(Vertex test) {
      return normal.dot(test) < normal.dot(v[0]);
   }

   public void connect(Facet adjacent, Vertex a, Vertex b) {
      Edge inner = getMatchingEdge(a, b);
      Edge outter = adjacent.getMatchingEdge(a, b);
      inner.setTwin(outter);
      outter.setTwin(inner);
   }

   public void connect(Edge e) {
      Edge inner = getMatchingEdge(e.getSource(), e.getDest());
      inner.setTwin(e);
      e.setTwin(inner);
   }

   public Edge getMatchingEdge(Vertex a, Vertex b) {
      for (int i=0; i<3; i++) {
         if (e[i].matches(a, b)) return e[i];
      }
      return null;
   }

   public Edge getHorizonEdge() {
      Edge opposite;
      for (int i=0; i<3; i++) {
         opposite = e[i].getTwin();
         if (opposite != null && opposite.onHorizon()) {
            return e[i];
         }
      }
      return null;
   }

   private void orient(Vertex reference) {
      if (!behind(reference)) {
         Vertex tmp = v[1];
         v[1] = v[2];
         v[2] = tmp;
         normal.negate();
         createEdges();
      }
   }

   private void createEdges() {
      e[0] = null;
      e[1] = null;
      e[2] = null;
      e[0] = new Edge(v[0], v[1], this);
      e[1] = new Edge(v[1], v[2], this);
      e[2] = new Edge(v[2], v[0], this);
      e[0].setNext(e[1]);
      e[0].setPrev(e[2]);
      e[1].setNext(e[2]);
      e[1].setPrev(e[0]);
      e[2].setNext(e[0]);
      e[2].setPrev(e[1]);
   }      

}
