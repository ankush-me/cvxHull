/*
 * @(#)Polytope.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Dept. of Computer Science
 * Spring 2004
 */
package j3d;

import java.util.List;

/**
 * A collection of vertices, edges, and facets stored in an object-
 * oriented version of a DCEL (doubly connected edge list).  Polytopes
 * also keep a current transformation matrix that determines how they
 * will be rendered in 3D.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Polytope { 

   private List points;     // vertex list
   private List facets;     // facet list
   private Matrix matrix;
   private float diameter;  // diameter of bounding sphere

   public Polytope() {
      this.diameter = 1.0F;
      this.points = new java.util.ArrayList();
      this.facets = new java.util.ArrayList();
      this.matrix = new Matrix();
      clear();
   }

   public void clear() {
      this.points.clear();
      this.facets.clear();
      this.matrix.identity();
   }

   public Matrix getMatrix() {
      return this.matrix;
   }

   public float getDiameter() {
      return this.diameter;
   }

   public void setDiameter(float diameter) {
      this.diameter = diameter;
   }

   public Vertex getVertex(int index) {
      if (index < 0 || index >= getVertexCount()) {
         return null;
      } else {
         return (Vertex)this.points.get(index);
      }
   }

   public int getVertexCount() {
      return this.points.size();
   }

   public void addVertex(Vertex v) {
      v.setIndex(getVertexCount());
      this.points.add(v);
   }

   public void removeVertex(Vertex v) {
      int index = v.getIndex();
      v.setIndex(-1);
      if (index < 0 || index >= getVertexCount()) return;

      if (index == getVertexCount() - 1) {
         this.points.remove(getVertexCount() - 1);
      } else {
         Vertex last = (Vertex)this.points.remove(getVertexCount()-1);
         last.setIndex(index);
         this.points.set(index, last);
      }
   }

   public Facet getFacet(int index) {
      if (index < 0 || index >= getFacetCount()) {
         return null;
      } else {
         return (Facet)this.facets.get(index);
      }
   }

   public int getFacetCount() {
      return this.facets.size();
   }

   public void addFacet(Facet f) {
      f.setIndex(getFacetCount());
      this.facets.add(f);
   }

   public void removeFacet(Facet f) {
      int index = f.getIndex();
      f.setIndex(-1);
      if (index < 0 || index >= getFacetCount()) return;

      if (index == getFacetCount() - 1) {
         this.facets.remove(getFacetCount() - 1);
      } else {
         Facet last = (Facet)this.facets.remove(getFacetCount() - 1);
         last.setIndex(index);
         this.facets.set(index, last);
      }
   }
}
