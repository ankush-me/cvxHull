/*
 * @(#)ConvexHull.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
package hull;

import j3d.*;
import java.util.List;
import java.util.Random;

/**
 * ConvexHull is a 3D polytope which implements the randomized
 * incremental algorithm for constructing a convex hull from a point
 * cloud.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class ConvexHull extends Polytope {

   /** Current vertex to add to the hull */
   protected int  current;

   /** List of newly created facets */
   protected List created;

   /** List of edges on the horizon */
   protected List horizon;

   /** List of facets visible to the current vertex */
   protected List visible;

   public ConvexHull() {
      this.created = new java.util.ArrayList();
      this.horizon = new java.util.ArrayList();
      this.visible = new java.util.ArrayList();
      restart();
   }

/**
 * Clear existing vertices and facets and generate a new random point
 * cloud.
 */
   public void restart() { 
      clear();  // clear all vertices and facets
      this.current = 0;
      this.created.clear();
      this.horizon.clear();
      this.visible.clear();

      Vertex v;
      Random rand = new Random(System.currentTimeMillis());
      float d = getDiameter();   // diameter
      float r = d / 2.0F;        // radius
      for (int i=0; i<40; i++) {
         v = new Vertex((rand.nextFloat() * d) - r, 
                        (rand.nextFloat() * d) - r,
                        (rand.nextFloat() * d) - r);
         addVertex(v);
         v.setData(new ConflictList(false));
      }
   }

/**
 * Add the next vertex to the convex hull
 */
   public void step() {
      if (current == 0) {
         prep();
      } else if (created.size() == 0) {
         stepA();
         stepB();
      } else {
         stepC();
      }
   }
   
/**
 * To begin the convex hull algorithm, we create a tetrahedron from
 * the first four vertices in the point cloud. 
 */
   public void prep() { 
      Vertex a = getVertex(0);
      Vertex b = getVertex(1);
      Vertex c = getVertex(2);
      Vertex d = getVertex(3);

      Facet f1 = new Facet(a, b, c, d);
      Facet f2 = new Facet(a, c, d, b);
      Facet f3 = new Facet(a, b, d, c);
      Facet f4 = new Facet(b, c, d, a);
      
      f1.setData(new ConflictList(true));
      f2.setData(new ConflictList(true));
      f3.setData(new ConflictList(true));
      f4.setData(new ConflictList(true));

      addFacet(f1);
      addFacet(f2);
      addFacet(f3);
      addFacet(f4);

      f1.connect(f2, a, c);
      f1.connect(f3, a, b);
      f1.connect(f4, b, c);
      f2.connect(f3, a, d);
      f2.connect(f4, c, d);
      f3.connect(f4, b, d);

      this.current = 4;

      /*
       * Initialize the conflict graph
       */
      Vertex v;
      for (int i=4; i<getVertexCount(); i++) {
         v = getVertex(i);
         if (!f1.behind(v)) addConflict(f1, v);
         if (!f2.behind(v)) addConflict(f2, v);
         if (!f3.behind(v)) addConflict(f3, v);
         if (!f4.behind(v)) addConflict(f4, v);
      }

      a.setVisible(false);
      b.setVisible(false);
      c.setVisible(false);
      d.setVisible(false);
   }


/**
 * StepA begins an incremental step of the algorithm. 
 * <ul>
 * <li>Identify the next vertex v.  O(1)
 * <li>Identify all facets visible to v.  O(F(v))
 * <li>Find the list of horizon edges for v.  O(F(v))
 * </ul>
 * F(v) refers to the facets visible to vertex v.
 */
   public void stepA() { 
      if (this.current >= getVertexCount()) return;

      this.created.clear();
      this.horizon.clear();
      this.visible.clear();
      
      /*
       * Get list of visible facets for v.
       */
      Vertex v = getVertex(current);
      ((ConflictList)v.getData()).getFacets(visible);
      
      /*
       * If v is already inside the convex hull, try the next point
       */
      if (visible.size() == 0) {
         v.setVisible(false);
         current++;
         stepA();
         return;
      }

      /*
       * Flag visible facets 
       */
      for (int i=0; i<visible.size(); i++) { 
         ((Facet)visible.get(i)).setMarked(true);
      }

      /*
       * Find horizon edges 
       */
      Edge e;
      for (int i=0; i<visible.size(); i++) {
         e = ((Facet)visible.get(i)).getHorizonEdge();
         if (e != null) {
            e.findHorizon(horizon);
            break;
         }
      }
   }


/** 
 * StepB continues the incremental step by conneting vertex v to
 * each edge of the horizon.
 */
   public void stepB() {

      if (this.current >= getVertexCount()) return;
      
      Vertex v = getVertex(current);
      
      /*
       * Create new facets to connect to the horizon  O(v)
       */
      Facet old, last = null, first = null;
      
      for (int i=0; i<horizon.size(); i++) {

         Edge e = (Edge)horizon.get(i);
         old = e.getTwin().getFacet();
         
         /*
          * Create a new facet
          */
         Facet f = new Facet(v, e.getDest(), e.getSource());
         f.setData(new ConflictList(true));
         addFacet(f);
         created.add(f);
         f.setFilled(false);

         /*
          * Connect it to the hull
          */
         f.connect(e);
         if (last != null) f.connect(last, v, e.getSource());
         last = f;
         if (first == null) first = f;

         /*
          * Update conflict graph for the new facet
          */
         addConflicts(f, old, e.getFacet());
      }
      
      if (last != null && first != null) {
         last.connect(first, v, first.getEdge(1).getDest());
      }
   }

/**
 * StepC cleans up the process started in steps A and B by removing
 * all of the previously visible facets (including the corresponding
 * nodes and edges in the conflict graph).
 */
   public void stepC() {

      /*
       * Hide the just-processed vertex
       */
      getVertex(current).setVisible(false);
      
      /*
       * Remove all previously visible facets
       */
      Facet f;
      for (int i=0; i<visible.size(); i++) {
         f = (Facet)visible.get(i);
         removeConflicts(f);
         removeFacet(f);
      }
      
      /*
       * Fill in all newly created facets
       */
      for (int i=0; i<created.size(); i++) {
         ((Facet)created.get(i)).setFilled(true);
      }
      created.clear();
      
      this.current++;
   }

/**
 * Add an arc to the conflict graph connecting the given facet and
 * vertex.
 */
   public void addConflict(Facet f, Vertex v) {
      GraphArc arc = new GraphArc(f, v);
      ((ConflictList)f.getData()).add(arc);
      ((ConflictList)v.getData()).add(arc);
   }

   
/**
 * Remove all conflicts for the given facet.
 */
   public void removeConflicts(Facet f) {
      ((ConflictList)f.getData()).clear();
   }

   
/**
 * Test all conflicts for the existing facet with the new facet.  Add
 * conflict arc for all vertices that can now see the new facet.
 */
   public void addConflicts(Facet f, Facet old1, Facet old2) {

      List l1 = new java.util.ArrayList();
      List l2 = new java.util.ArrayList();
      List l = new java.util.ArrayList();

      ((ConflictList)old1.getData()).getVertices(l1);
      ((ConflictList)old2.getData()).getVertices(l2);

      Vertex v1, v2;
      int i1 = 0, i2 = 0;

      while (i1 < l1.size() || i2 < l2.size()) {

         if (i1 < l1.size() && i2 < l2.size()) {
            v1 = (Vertex)l1.get(i1);
            v2 = (Vertex)l2.get(i2);
            if (v1.getIndex() == v2.getIndex()) {
               l.add(v1);
               i1++;
               i2++;
            }
            else if (v1.getIndex() > v2.getIndex()) {
               l.add(v1);
               i1++;
            }
            else {
               l.add(v2);
               i2++;
            }
         }
         else if (i1 < l1.size()) {
            l.add(l1.get(i1++));
         }
         else {
            l.add(l2.get(i2++));
         }
      }

      Vertex v;
      for (int i=l.size() - 1; i >= 0; i--) {
         v = (Vertex)l.get(i);
         if (!f.behind(v)) addConflict(f, v);
      }
   }

}
