/*
 * @(#)Canvas3D.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
package j3d;

import java.util.List;
import java.awt.Color;
import java.awt.Graphics;

/**
 * The Canvas3D object is responsible for rendering 3D polytopes.
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Canvas3D {

   private Vector light;   // Direction of a distant light source
   private int xpick;
   private int ypick;
   private Vertex tv;
   private Vector tnorm;

   public Canvas3D() { 
      this.light = new Vector(1, 0, 1);
      this.light.normalize();
      this.xpick = -1;
      this.ypick = -1;
      this.tv = new Vertex();
      this.tnorm = new Vector();
   }

/**
 * Sets the pick tool location
 */
   public void setPickPoint(int x, int y) {
      this.xpick = x;
      this.ypick = y;
   }

/**
 * Render a convex 3D polytope on a graphics object
 */
   public void render(Polytope model, Graphics g) {
      Vertex v;
      Facet f;

      /* Draw vertices */
      for (int i=0; i<model.getVertexCount(); i++) {
         drawVertex(model, model.getVertex(i), g);
      }

      /* Draw unfilled facets */
      for (int i=0; i<model.getFacetCount(); i++) {
         f = model.getFacet(i);
         if (!f.isFilled()) drawFacet(model, f, g);
      }

      /* Draw all forward facing facets */
      for (int i=0; i<model.getFacetCount(); i++) {
         f = model.getFacet(i);
         if (visible(model, f)) {
            drawFacet(model, model.getFacet(i), g);
         }
      }

      //--------------------------------------------------
      // Hack that takes advantage of the fact that the
      // polytope is a convex hull
      //--------------------------------------------------
      List l = new java.util.ArrayList();
      for (int i=0; i<model.getFacetCount(); i++) {
         f = model.getFacet(i);
         if (visible(model, f)) {
            ((hull.ConflictList)f.getData()).getVertices(l);
         }
         for (int j=0; j<l.size(); j++) {
            v = (Vertex)l.get(j);
            if (v.isVisible()) { drawVertex(model, v, g); }
         }
      }
   }

/**
 * Returns true if a facet if facing towards the camera based on its
 * normal vector.
 */
   protected boolean visible(Polytope p, Facet f) {
      p.getMatrix().transform(f.getNormal(), tnorm);
      return (tnorm.z < 0);
   }

/**
 * Draw an individual vertex
 */
   protected void drawVertex(Polytope p, Vertex v, Graphics g) {
      if (!v.isVisible()) return;
      g.setColor(Color.black);
      p.getMatrix().transform(v, tv);
      g.fillOval((int)tv.x - 2, (int)tv.y - 2, 4, 4);
   }

/**
 * Draw an individual facet of the polytope
 */
   protected void drawFacet(Polytope p, Facet f, Graphics g) {
      if (!f.isVisible()) return;
      
      // Compute the Lambert cosine for shading
      p.getMatrix().transform(f.getNormal(), tnorm);
      tnorm.normalize();
      float ic = (tnorm.dot(light) / -2.0F) + 0.7F;
      if (ic > 1.0F) ic = 1.0F;

      // Transform each vertex of the facet and convert to planar
      // coordinates 
      for (int c = 0; c < 4; c++) {
         p.getMatrix().transform(f.getVertex(c % 3), tv);
         px[c] = (int)tv.x;
         py[c] = (int)tv.y;
      }

//      if (isSelected(px, py)) {
//         g.setColor(new Color(ic, ic, ic * 0.6F));
//      } else {
         g.setColor(new Color(ic, ic, ic));
//      }
      
      if (f.isFilled()) { 
         g.fillPolygon(px, py, px.length);
      }
      g.setColor(Color.black);
      g.drawPolygon(px, py, px.length);
   }

   private boolean isSelected(int px[], int py[]) {
      return (right(px[3], py[3], px[2], py[2], xpick, ypick) &&
              right(px[2], py[2], px[1], py[1], xpick, ypick) &&
              right(px[1], py[1], px[0], py[0], xpick, ypick));
   }

   private boolean right(int x1, int y1,
                           int x2, int y2,
                           int x3, int y3)
   {
      int dx1 = x2 - x1;
      int dx2 = x3 - x2;
      int dy1 = y2 - y1;
      int dy2 = y3 - y2;
      return dy2 * dx1 < dy1 * dx2;
   }
   
   private int px[] = new int[4];
   private int py[] = new int[4];
   
}
