/*
 * @(#)Viewer.java
 * 
 * Final Project
 * COMP163: Computational Geometry
 * Tufts University, Department of Computer Science
 * Spring 2004
 */
import j3d.*;
import hull.*;
import java.awt.*;
import java.awt.event.*;
import java.applet.Applet;

/**
 * Main applet that controls visualization and user interaction.
 * This part of the code is essentially throw-away, so browse at your
 * own risk. 
 * 
 * @author Michael Horn
 * @version $Revision$, $Date$
 */
public class Viewer extends Applet implements
Runnable, MouseListener, MouseMotionListener, WindowListener {

   /** Dimensions of the window */
   public static final int WIDTH = 400;

   private ConvexHull hull;       // Implements the algorithm
   private Canvas3D canvas;       // Renders the 3D convex hull
   private Toolbar buttons;       // The control buttons
   private Image offscreen;       // for double buffering
   private Thread thread;         // controls spinning
   private boolean spinning;      // whether the hull is spinning
   private boolean mousedown;     // current mouse state
   private Matrix rotation;       // stores the user driven rotation
   private int width;
   private int height;

/*******************************************************************/
   public Viewer() { }

/**
 * Applet has been loaded into the system.
 */
   public void init() { 
      this.hull = new ConvexHull();
      this.canvas = new Canvas3D();
      this.offscreen = null;
      this.spinning = true;
      this.mousedown = false;
      this.buttons = new Toolbar(this, 0, 0, getWidth(), 30);
      this.buttons.addButton("Restart");
      this.buttons.addButton("Step");
      this.buttons.addToggleButton("Spin");
      this.rotation = new Matrix();
      this.thread = new Thread(this);
      this.thread.setPriority(7);
      this.thread.start();
      this.width = getWidth();
      this.height = getHeight();

      setBackground(new Color(220, 220, 220));
      addMouseListener(this);
      addMouseMotionListener(this);
   }

/**
 * Callback for button clicks
 */
   public void action(String command) {
      if (command.equals("Step")) { 
         this.hull.step();
      }
      else if (command.equals("Restart")) {
         this.hull.restart();
      }
      else if (command.equals("Spin")) {
         this.spinning = !this.spinning;
      }
   }

/**
 * Paint simply calls update
 */
   public void paint(Graphics g) { update(g); }

/**
 * Draws the convex hull and buttons to the screen.
 */
   public void update(Graphics graphics) {
      if (offscreen == null ||
          width != getWidth() ||
          height != getHeight()) {
         offscreen = createImage(getWidth(), getHeight());
         buttons.setWidth(getWidth());
         width = getWidth();
         height = getHeight();
      }
      
      Graphics g = offscreen.getGraphics();
      g.clearRect(0, 0, getWidth(), getHeight());

      float w = (width < height)? width : height;
      float scale = 0.65F * w / hull.getDiameter();
      Matrix m = this.hull.getMatrix();
      m.identity();
      m.mult(rotation);
      m.scale(scale, -scale, scale);
      m.translate(getWidth() / 2.0F, getHeight() / 2.0F, 0);
      this.canvas.render(hull, g);
      this.buttons.draw(g);
      
      graphics.drawImage(offscreen, 0, 0, null);
   }
   

/**
 * Spin the convex hull...
 */
   public void run() {
      while (true) { 
         if (spinning && !mousedown) {
            this.rotation.rotate(0F, -2.0F, 0F);
            repaint();
         }
         try { Thread.sleep(50); }
         catch(InterruptedException ix) { ; }
      }
   }
   
   int prevx;
   int prevy;

   
   public void mousePressed(MouseEvent e) {
      int x = e.getX();
      int y = e.getY();
      if (buttons.mouseDown(x, y)) {
         repaint();
      }
      else if (!buttons.intersects(e.getX(), e.getY())) {
         prevx = x;
         prevy = y;
         mousedown = true;
         this.canvas.setPickPoint(x, y);
         repaint();
      }
   }

   public void mouseReleased(MouseEvent e) {
      mousedown = false;
      this.canvas.setPickPoint(-1, -1);
      if (buttons.mouseUp(e.getX(), e.getY())) {
         repaint();
      }
   }

   public void mouseMoved(MouseEvent e) {
      if (buttons.mouseMove(e.getX(), e.getY())) {
         repaint();
      }
   }

   public void mouseDragged(MouseEvent e) {
      int x = e.getX();
      int y = e.getY();
      if (mousedown) { 
         float xtheta = ((float)(y - prevy) * 360F) / getWidth();
         float ytheta = ((float)(prevx - x) * 360F) / getHeight();
         
         this.canvas.setPickPoint(x, y);
         this.rotation.rotate(xtheta, ytheta, 0F);
         repaint();
         
         prevx = x;
         prevy = y;
      }
   }
   
   public void mouseClicked(MouseEvent e) { }
   public void mouseEntered(MouseEvent e) { }
   public void mouseExited(MouseEvent e) { }
   public void windowActivated(WindowEvent e) { }
   public void windowClosed(WindowEvent e) { }
   public void windowClosing(WindowEvent e) { System.exit(0); }
   public void windowDeactivated(WindowEvent e) { }
   public void windowDeiconified(WindowEvent e) { }
   public void windowIconified(WindowEvent e) { }
   public void windowOpened(WindowEvent e) { }
   
   public String getAppletInfo() {
      return "3D Convex Hull Visalization";
   }
   
   public static void main(String args[]) {
      Frame frame = new Frame("ConvexHull");
      frame.setSize(WIDTH, WIDTH);
      Viewer v = new Viewer();
      v.setSize(WIDTH, WIDTH);
      v.init();
      frame.add(v);
      frame.addWindowListener(v);
      frame.setVisible(true);
   }
}
