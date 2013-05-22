/*
 * @(#)Edge.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
package j3d;
import java.util.List;

/**
 * An edge of a 3D polytope.  Edges are connected to each other to
 * form a doubly-connected edge list (DCEL).
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Edge extends Face {

   private Vertex a;    // source vertex
   private Vertex b;    // destination vertex
   private Facet facet; // incident facet
   private Edge next;   // next edge
   private Edge prev;   // previous edge
   private Edge twin;   // matching edge on adjacent facet

   public Edge(Vertex a, Vertex b, Facet facet) {
      this.a = a;
      this.b = b;
      this.facet = facet;
      this.twin = null;
      this.next = null;
      this.prev = null;
   }

   public Facet getFacet() {
      return this.facet;
   }

   public Edge getNext() {
      return this.next;
   }

   public void setNext(Edge next) {
      this.next = next;
   }

   public Edge getPrev() {
      return this.prev;
   }

   public void setPrev(Edge prev) {
      this.prev = prev;
   }

   public Edge getTwin() {
      return this.twin;
   }

   public void setTwin(Edge twin) {
      this.twin = twin;
   }

   public Vertex getSource() {
      return this.a;
   }

   public Vertex getDest() {
      return this.b;
   }

   public boolean onHorizon() {
      if (twin == null) {
         return false;
      } else {
         return (!facet.isMarked() && twin.getFacet().isMarked());
      }
   }

   public void findHorizon(List horizon) {
      if (onHorizon()) {
         if (horizon.size() > 0 && this == horizon.get(0)) {
            return;
         } else {
            horizon.add(this);
            next.findHorizon(horizon);
         }
      } else {
         if (twin != null) {
            twin.getNext().findHorizon(horizon);
         }
      }
   }

   public boolean matches(Vertex a, Vertex b) {
      return ((this.a == a && this.b == b) ||
              (this.a == b && this.b == a));
   }
}
   
