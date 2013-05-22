/*
 * @(#)Vector.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Dept. of Computer Science
 * Spring 2004
 */
package j3d;

/**
 * A standard 3-dimensional vector
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Vector {

   public float x;
   public float y;
   public float z;
   
   public Vector() {
      x = y = z = 0.0F;
   }

   public Vector(float x, float y, float z) {
      this.x = x;
      this.y = y;
      this.z = z;
   }

   public Vector(double x, double y, double z) {
      this.x = (float)x;
      this.y = (float)y;
      this.z = (float)z;
   }

   public Vector(Vector a) {
      this.x = a.x;
      this.y = a.y;
      this.z = a.z;
   }
   
   public Vector subtract(Vector b) {
      return new Vector(x - b.x, y - b.y, z - b.z);
   }

   public Vector cross(Vector b) {
      return new Vector(
         y * b.z - z * b.y,
         z * b.x - x * b.z,
         x * b.y - y * b.x);
   }

   public float dot(Vector b) {
      return x * b.x + y * b.y + z * b.z;
   }

   public float dot(Vertex b) {
      return x * b.x + y * b.y + z * b.z;
   }

   public void normalize() {
      float len = length();
      if (len > 0.0F) {
         x /= len;
         y /= len;
         z /= len;
      }
   }
   
   public void negate() {
      x *= -1;
      y *= -1;
      z *= -1;
   }
   
   public float length() {
      return (float)Math.sqrt(x * x + y * y + z * z);
   }
}
