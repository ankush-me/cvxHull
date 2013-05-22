/*
 * @(#)ConflictList.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
package hull;

import java.util.List;

/**
 * Every vertex and facet in this convex hull algorithm keeps a
 * ConflictList.  For a vertex, this list contains all of the facets
 * that are visible to it.  For a facet, the list contains all of the
 * vertices that can see it.  The conflict lists are stored as doubly-
 * connected lists of GraphArcs and are designed to support constant-
 * time add and remove operations.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class ConflictList {

   protected GraphArc head;
   private boolean facet;

   public ConflictList(boolean facet) {
      this.head = null;
      this.facet = facet;
   }

   public void add(GraphArc arc) {
      if (this.facet) {
         if (this.head != null) { this.head.prevf = arc; }
         arc.nextf = this.head;
         this.head = arc;
      } else {
         if (this.head != null) { this.head.prevv = arc; }
         arc.nextv = this.head;
         this.head = arc;
      }
   }

   public boolean empty() {
      return (this.head == null);
   }

   public void clear() {
      while (this.head != null) {
         this.head.delete();
      }
   }

/**
 * Fill a list of verticies by recursively walking the doubly-linked
 * facet list.
 */
   public void getVertices(List list) {
      GraphArc arc = this.head;
      while (arc != null) { 
         list.add(arc.vertex);
         arc = arc.nextf;
      }
   }

/**
 * Fill a list of facets by recursively walking the doubly-linked
 * facet list.
 */
   public void getFacets(List list) {
      GraphArc arc = this.head;
      while (arc != null) { 
         list.add(arc.facet);
         arc = arc.nextv;
      }
   }

   public void printFacetList() {
      System.out.print("Facets [");
      GraphArc arc = this.head;
      while (arc != null) {
         System.out.print(" " + arc.facet.getIndex());
         arc = arc.nextv;
      }
      System.out.println(" ]");
   }
   
   public void printVertexList() {
      System.out.print("Vertices [");
      GraphArc arc = this.head;
      while (arc != null) { 
         System.out.print(" " + arc.vertex.getIndex() + " ");
         arc = arc.nextf;
      }
      System.out.println(" ]");
   }
}
