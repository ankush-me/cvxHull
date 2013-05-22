/*
 * @(#)Vertex.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Dept. of Computer Science
 * Spring 2004
 */
package j3d;
import java.util.List;

/**
 * A point in 3-dimensional space.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Vertex extends Face {

   public float x;
   public float y;
   public float z;

   public Vertex() {
      this(0F, 0F, 0F);
   }

   public Vertex(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
   }

   public static Vector subtract(Vertex a, Vertex b) {
      return new Vector(a.x - b.x, a.y - b.y, a.z - b.z);
   }
}
