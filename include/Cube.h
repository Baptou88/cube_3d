#include <Geometry.h>
#include <Geometry.h>
#include <heltec.h>

#if !defined(__CUBE)
#define __CUBE

extern SSD1306Wire *display;

class Cube
{
private:
  pt2D_t cube_pt[8];
  pt3D_t orig_points[8] = {
  {-1,-1,1},
  {1,-1,1},
  {1,1,1},
  {-1,1,1},
  {-1,-1,-1},
  {1,-1,-1},
  {1,1,-1},
  {-1,1,-1}
};
float angle_deg = 30;
float  z_offset = -4;
float cube_size = 70;

pt3D_t rotated_points [8];
public:
  Cube(/* args */);
  ~Cube();
  float shoelace(pt2D_t a,pt2D_t b,pt2D_t c,pt2D_t d);
  void hiddenCorner(pt2D_t a,pt2D_t b);
  void drawFace(pt2D_t a,pt2D_t b,pt2D_t c,pt2D_t d);
  void calcul(void);
  void inc_Angle(int inc);
  void render();
  void Translate(pt3D_t vector);
};

Cube::Cube(/* args */)
{
}

Cube::~Cube()
{
}
float Cube::shoelace(pt2D_t a,pt2D_t b,pt2D_t c,pt2D_t d){
  return {
    (a.x*b.y - a.y*b.x) + 
    (b.x*c.y - b.y*c.x) + 
    (c.x*d.y - c.y*d.x) + 
    (d.x*a.y - d.y*a.x)
  };
}
void Cube::hiddenCorner(pt2D_t a,pt2D_t b){
   int diviser = 3;
  for (size_t i = 0; i < diviser; i++)
  {
    display->setPixel((i*(b.x - a.x)/diviser)+a.x,(i*(b.y - a.y)/diviser)+a.y);
  }
  
}
void Cube::drawFace(pt2D_t a,pt2D_t b,pt2D_t c,pt2D_t d){
  if (shoelace(a,b,c,d)>=0)
  {
    display->drawLine(a.x,a.y,b.x,b.y);
    display->drawLine(b.x,b.y,c.x,c.y);
    display->drawLine(c.x,c.y,d.x,d.y);
    display->drawLine(d.x,d.y,a.x,a.y);

  } else
  {
    display->setPixel(a.x,a.y);
    display->setPixel(b.x,b.y);
    display->setPixel(c.x,c.y);
    display->setPixel(d.x,d.y);

    hiddenCorner(a,b);
    hiddenCorner(b,c);
    hiddenCorner(c,d);
    hiddenCorner(d,a);
  }
  
  
}
void Cube::calcul(void){
  for (size_t i = 0; i < 8; i++)
  {
    // rotate 3d points
    rotated_points[i].x =  orig_points[i].x * cos(radians(angle_deg)) - orig_points[i].z * sin(radians(angle_deg));
    rotated_points[i].y =  orig_points[i].y;
    rotated_points[i].z =  orig_points[i].x * sin(radians(angle_deg)) + orig_points[i].z * cos(radians(angle_deg)) + z_offset;

    //project 3d points into 2d
    cube_pt[i].x  = 64 - rotated_points[i].x / rotated_points[i].z * cube_size;
    cube_pt[i].y  = 32 - rotated_points[i].y / rotated_points[i].z * cube_size;
  }
  
}
void Cube::inc_Angle(int inc){
  this->angle_deg += inc;
}
void Cube::render(void){
  drawFace(cube_pt[0],cube_pt[1],cube_pt[2],cube_pt[3]);
  drawFace(cube_pt[1],cube_pt[5],cube_pt[6],cube_pt[2]);
  drawFace(cube_pt[5],cube_pt[4],cube_pt[7],cube_pt[6]);
  drawFace(cube_pt[4],cube_pt[0],cube_pt[3],cube_pt[7]);
}
void Cube::Translate(pt3D_t vector){
  for (size_t i = 0; i < 8; i++)
  {
    orig_points[i].x += vector.x;
    orig_points[i].y += vector.y;
    orig_points[i].z += vector.z;
  }
  
  
}

#endif // __CUBE
