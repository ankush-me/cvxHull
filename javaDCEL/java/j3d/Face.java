/*
 * @(#)Face.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
package j3d;

/**
 * Generic superclass for all 3D face types: vertices, edges, facets,
 * and polytopes.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public abstract class Face {

   private int index;       // index in a Polytope list
   private Object data;     // pointer for extra user-defined data
   private boolean visible; // should this face be drawn to the screen

   public Face() {
      this.index = -1;
      this.data = null;
      this.visible = true;
   }
   
   public int getIndex() {
      return this.index;
   }

   public void setIndex(int index) {
      this.index = index;
   }

   public Object getData() {
      return this.data;
   }

   public void setData(Object data) {
      this.data = data;
   }

   public boolean isVisible() {
      return this.visible;
   }

   public void setVisible(boolean visible) {
      this.visible = visible;
   }
}
