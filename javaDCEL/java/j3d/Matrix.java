/*
 * @(#)Matrix.java
 * 
 * Copyright (c) 1995-1998 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Sun grants you ("Licensee") a non-exclusive, royalty free, license
 * to use, modify and redistribute this software in source and binary
 * code form, provided that i) this copyright notice and license
 * appear on all copies of the software; and ii) Licensee does not
 * utilize the software in a manner which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN
 * AND ITS LICENSORS SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY
 * LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING THE
 * SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS LICENSORS
 * BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES,
 * HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING
 * OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * This software is not designed or intended for use in on-line
 * control of aircraft, air traffic, aircraft navigation or aircraft
 * communications; or in the design, construction, operation or
 * maintenance of any nuclear facility. Licensee represents and
 * warrants that it will not use or redistribute the Software for
 * such purposes.
 */
package j3d;

/**
 * Standard 4x4 matrix for performing 3D transformations.
 */
public class Matrix {

   float xx;
   float xy;
   float xz;
   float xo;
   float yx;
   float yy;
   float yz;
   float yo;
   float zx;
   float zy;
   float zz;
   float zo;

   public Matrix() {
      identity();
   }
   
   public void identity() {
      xo = 0.0F;
      xx = 1.0F;
      xy = 0.0F;
      xz = 0.0F;
      yo = 0.0F;
      yx = 0.0F;
      yy = 1.0F;
      yz = 0.0F;
      zo = 0.0F;
      zx = 0.0F;
      zy = 0.0F;
      zz = 1.0F;
   }

   public void scale(float xf, float yf, float zf) {
      xx *= xf;
      xy *= xf;
      xz *= xf;
      xo *= xf;
      yx *= yf;
      yy *= yf;
      yz *= yf;
      yo *= yf;
      zx *= zf;
      zy *= zf;
      zz *= zf;
      zo *= zf;
   }

   public void translate(float x, float y, float z) {
      xo += x;
      yo += y;
      zo += z;
   }

   public void rotate(float x, float y, float z) {
      xrot(x);
      yrot(y);
      zrot(z);
   }
   
   public void xrot(float theta) {
      theta *= (Math.PI / 180.0);
      float ct = (float)Math.cos(theta);
      float st = (float)Math.sin(theta);
      float Nyx = yx * ct + zx * st;
      float Nyy = yy * ct + zy * st;
      float Nyz = yz * ct + zz * st;
      float Nyo = yo * ct + zo * st;
      float Nzx = zx * ct - yx * st;
      float Nzy = zy * ct - yy * st;
      float Nzz = zz * ct - yz * st;
      float Nzo = zo * ct - yo * st;
      yo = Nyo;
      yx = Nyx;
      yy = Nyy;
      yz = Nyz;
      zo = Nzo;
      zx = Nzx;
      zy = Nzy;
      zz = Nzz;
   }
   
   public void yrot(float theta) {
      theta *= (Math.PI / 180.0);
      float ct = (float)Math.cos(theta);
      float st = (float)Math.sin(theta);
      float Nxx = xx * ct + zx * st;
      float Nxy = xy * ct + zy * st;
      float Nxz = xz * ct + zz * st;
      float Nxo = xo * ct + zo * st;
      float Nzx = zx * ct - xx * st;
      float Nzy = zy * ct - xy * st;
      float Nzz = zz * ct - xz * st;
      float Nzo = zo * ct - xo * st;
      xo = Nxo;
      xx = Nxx;
      xy = Nxy;
      xz = Nxz;
      zo = Nzo;
      zx = Nzx;
      zy = Nzy;
      zz = Nzz;
   }
   
   public void zrot(float theta) {
      theta *= (Math.PI / 180.0);
      float ct = (float)Math.cos(theta);
      float st = (float)Math.sin(theta);
      float Nyx = yx * ct + xx * st;
      float Nyy = yy * ct + xy * st;
      float Nyz = yz * ct + xz * st;
      float Nyo = yo * ct + xo * st;
      float Nxx = xx * ct - yx * st;
      float Nxy = xy * ct - yy * st;
      float Nxz = xz * ct - yz * st;
      float Nxo = xo * ct - yo * st;
      yo = Nyo;
      yx = Nyx;
      yy = Nyy;
      yz = Nyz;
      xo = Nxo;
      xx = Nxx;
      xy = Nxy;
      xz = Nxz;
   }

   public void mult(Matrix rhs) {
      float lxx = xx * rhs.xx + yx * rhs.xy + zx * rhs.xz;
      float lxy = xy * rhs.xx + yy * rhs.xy + zy * rhs.xz;
      float lxz = xz * rhs.xx + yz * rhs.xy + zz * rhs.xz;
      float lxo = xo * rhs.xx + yo * rhs.xy + zo * rhs.xz + rhs.xo;
      float lyx = xx * rhs.yx + yx * rhs.yy + zx * rhs.yz;
      float lyy = xy * rhs.yx + yy * rhs.yy + zy * rhs.yz;
      float lyz = xz * rhs.yx + yz * rhs.yy + zz * rhs.yz;
      float lyo = xo * rhs.yx + yo * rhs.yy + zo * rhs.yz + rhs.yo;
      float lzx = xx * rhs.zx + yx * rhs.zy + zx * rhs.zz;
      float lzy = xy * rhs.zx + yy * rhs.zy + zy * rhs.zz;
      float lzz = xz * rhs.zx + yz * rhs.zy + zz * rhs.zz;
      float lzo = xo * rhs.zx + yo * rhs.zy + zo * rhs.zz + rhs.zo;
      xx = lxx;
      xy = lxy;
      xz = lxz;
      xo = lxo;
      yx = lyx;
      yy = lyy;
      yz = lyz;
      yo = lyo;
      zx = lzx;
      zy = lzy;
      zz = lzz;
      zo = lzo;
   }
   
   public void transform(Vector v, Vector t) {
      t.x = (v.x * xx + v.y * xy + v.z * xz + xo);
      t.y = (v.x * yx + v.y * yy + v.z * yz + yo);
      t.z = (v.x * zx + v.y * zy + v.z * zz + zo);
   }

   public void transform(Vertex v, Vertex t) {
      t.x = (v.x * xx + v.y * xy + v.z * xz + xo);
      t.y = (v.x * yx + v.y * yy + v.z * yz + yo);
      t.z = (v.x * zx + v.y * zy + v.z * zz + zo);
   }
}
