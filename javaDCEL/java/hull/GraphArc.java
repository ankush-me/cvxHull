/*
 * @(#)GraphArc.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
package hull;

import j3d.*;
import java.util.List;

/**
 * The Conflict Graph is completely constructed of doubly-linked lists
 * of GraphArcs stored in the facets and vertices of the convex hull.
 * Each GraphArc indicates that the given facet and vertex are visible
 * to each other and therefore in conflict.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class GraphArc {

   // The facet and vertex that are visible to each other
   protected Facet facet;
   protected Vertex vertex;

   // Doubly-linked list for a vertex
   protected GraphArc nextv;
   protected GraphArc prevv;

   // Doubly-linked list for a facet
   protected GraphArc nextf;
   protected GraphArc prevf;

/**
 * Create a new arc for the conflict graph.  This arc won't be
 * connected until add() is called. 
 */
   public GraphArc(Facet f, Vertex v) {
      this.vertex = v;
      this.facet = f;
      this.nextv = null;
      this.prevv = null;
      this.nextf = null;
      this.prevf = null;
   }

/**
 * Add this GraphArc to the doubly-linked lists contained in the
 * Facet and the Vertex.
 */
//    public void add() {
//       ConflictList headv = (ConflictList)this.vertex.getData();
//       ConflictList headf = (ConflictList)this.facet.getData();

//       if (headv != null) { headv.prevv = this; }
//       this.nextv = headv;
//       this.vertex.setData(this);
         
//       if (headf != null) { headf.prevf = this; }
//       this.nextf = headf;
//       this.facet.setData(this);
//    }

/**
 * Delete this GraphArc from both doubly-linked lists
 */
   public void delete() {
      if (this.prevv != null) { this.prevv.nextv = this.nextv; }
      if (this.nextv != null) { this.nextv.prevv = this.prevv; }
      if (this.prevf != null) { this.prevf.nextf = this.nextf; }
      if (this.nextf != null) { this.nextf.prevf = this.prevf; }

      ConflictList list;

      if (this.prevv == null) { 
         list = (ConflictList)this.vertex.getData();
         list.head = this.nextv;
      }

      if (this.prevf == null) {
         list = (ConflictList)this.facet.getData();
         list.head = this.nextf;
      }
//       if (this.vertex.getData() == this) {
//          this.vertex.setData(this.nextv);
//       }
//       if (this.facet.getData() == this) {
//          this.facet.setData(this.nextf);
//       }
   }

/**
 * Recursively delete all nodes in that are part of the doubly-linked
 * facet list.
 */
//    public void deleteAllFacets() {
//       if (this.nextf != null) { this.nextf.deleteAllFacets(); }
//       delete();
//    }

/**
 * Fill a list of verticies by recursively walking the doubly-linked
 * vertex list.
 */
//    public void getVertices(List list) {
//       list.add(this.vertex);
//       if (this.nextf != null) this.nextf.getVertices(list);
//    }

/**
 * Fill a list of facets by recursively walking the doubly-linked
 * facet list.
 */
//    public void getFacets(List list) {
//       list.add(this.facet);
//       if (this.nextv != null) this.nextv.getFacets(list);
//    }

//    public void printFacetList() {
//       if (this.prevv == null) System.out.print("Facets [");
//       System.out.print(" " + this.facet.getIndex() + " ");
//       if (this.nextv == null) {
//          System.out.println("]");
//       } else {
//          this.nextv.printFacetList();
//       }
//    }
   
//    public void printVertexList() {
//       if (this.prevf == null) System.out.print("Vertices [");
//       System.out.print(" " + this.vertex.getIndex() + " ");
//       if (this.nextf == null) {
//          System.out.println("]");
//       } else {
//          this.nextf.printVertexList();
//       }
//    }
}
