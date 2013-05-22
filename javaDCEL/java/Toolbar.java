/*
 * @(#)Toolbar.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
import java.awt.*;
import java.util.List;

/**
 * Toolbar which contains FancyButtons
 *
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Toolbar {
   
   public static final int BUTTON_WIDTH = 45;
   
   protected List buttons;
   protected int x;
   protected int y;
   protected int width;
   protected int height;
   protected Viewer manager;
   
   public Toolbar(Viewer manager, int x, int y, int w, int h) {
      this.buttons = new java.util.ArrayList();
      this.width = w;
      this.height = h;
      this.x = x;
      this.y = y;
      this.manager = manager;
   }

   public int getWidth() {
      return this.width;
   }

   public void setWidth(int width) {
      this.width = width;
   }
   
   public void action(String command) { 
      this.manager.action(command);
   }
   
   public FancyButton addButton(String label) {
      int bx = buttons.size() * BUTTON_WIDTH + 2;
      FancyButton b = new FancyButton(label,
                                      bx, y + 2,
                                      BUTTON_WIDTH - 2,
                                      height - 5);
      this.buttons.add(b);
      return b;
   }

   public FancyButton addToggleButton(String label) { 
      FancyButton b = addButton(label);
      b.setToggle(true);
      b.setToggleState(true);
      return b;
   }
   
   public void draw(Graphics g) {
      g.setFont(new Font("SansSerif", Font.PLAIN, 10));
      g.setColor(new Color(100, 100, 100));
      g.fillRect(x, y, width, height);
      FancyButton b;
      for (int i=0; i<buttons.size(); i++) { 
         b = (FancyButton)buttons.get(i);
         b.draw(g);
      }
   }
   
   public boolean intersects(int x, int y) { 
      return (x >= this.x && y >= this.y && 
              x <= (this.x + this.width) &&
              y <= (this.y + this.height));
   }
   
   public boolean mouseDown(int x, int y) {
      FancyButton b;
      for (int i=0; i<buttons.size(); i++) { 
         b = (FancyButton)buttons.get(i);
         if (b.intersects(x, y)) {
            b.setDown(true);
            if (b.isToggle()) { b.toggleState(); }
            return true;
         }
      }
      return false;
   }
   
   public boolean mouseUp(int x, int y) { 
      boolean refresh = false;
      FancyButton b;
      for (int i=0; i<buttons.size(); i++) { 
         b = (FancyButton)buttons.get(i);
         if (b.isDown()) {
            b.setDown(false);
            if (b.intersects(x, y)) {
               manager.action(b.getLabel());
            }
            refresh = true;
         }
      }
      return refresh;
   }
   
   public boolean mouseMove(int x, int y) {
      boolean refresh = false;
      FancyButton b;
      for (int i=0; i<buttons.size(); i++) { 
         b = (FancyButton)buttons.get(i);
         if (b.intersects(x, y) && !b.isOver()) { 
            b.setOver(true);
            refresh = true;
         }
         else if (!b.intersects(x, y) && b.isOver()) { 
            b.setOver(false);
            refresh = true;
         }
      }
      return refresh;
   }
}


/**
 * A nice-looking button.
 */
class FancyButton { 

   protected String label;
   protected int width;
   protected int height; 
   protected int x;
   protected int y;
   protected boolean over;
   protected boolean down;
   protected boolean toggle;
   protected boolean toggle_state;
   
   public FancyButton(String label, int x, int y, int w, int h) {
      this.label = label;
      this.x = x;
      this.y = y;
      this.width = w;
      this.height = h;
      this.over = false;
      this.down = false;
      this.toggle = false;
      this.toggle_state = false;
   }
   
   public String getLabel() { 
      return this.label;
   }
   
   public void setLabel(String label) { 
      this.label = label;
   }
   
   public void draw(Graphics g) {
      g.setColor(new Color(100, 100, 100));
      g.fillRect(x, y, width, height);

      int x2 = x + width;
      int y2 = y + height;

      Color c1, c2;
      if (down || toggle_state) {
         c1 = Color.white;
         c2 = Color.darkGray;
      } else { 
         c1 = Color.darkGray;
         c2 = Color.white;
      }
      
      if (over || toggle_state) {
         g.setColor(c1);
         g.drawLine(x, y2, x2, y2);
         g.drawLine(x2, y, x2, y2);
         g.setColor(c2);
         g.drawLine(x, y, x2, y);
         g.drawLine(x, y, x, y2);
      } 

      int fw = g.getFontMetrics().stringWidth(label);
      int fh = g.getFontMetrics().getHeight();
      int fx = x + width/2 - fw/2;
      int fy = y + height - fh/2;
      if (down) { fx += 1; fy += 1; }
      if (over || down) {
         g.setColor(Color.white);
      } else {
         g.setColor(Color.lightGray);
      }
      g.drawString(label, fx, fy);
   }
   
   public boolean intersects(int x, int y) { 
      return (x >= this.x && y >= this.y && 
              x <= (this.x + this.width) &&
              y <= (this.y + this.height));
   }
   
   public boolean isOver() {
      return this.over;
   }
   
   public void setOver(boolean over) {
      this.over = over;
   }
   
   public boolean isDown() { 
      return this.down;
   }
   
   public void setDown(boolean down) { 
      this.down = down;
   }

   public void setToggle(boolean toggle) {
      this.toggle = toggle;
   }

   public boolean isToggle() {
      return this.toggle;
   }

   public void toggleState() {
      this.toggle_state = !this.toggle_state;
   }

   public void setToggleState(boolean state) {
      this.toggle_state = state;
   }
}
