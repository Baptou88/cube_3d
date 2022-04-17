#include <Arduino.h>
#include <Geometry.h>
//#include <heltec.h>
#include <SSD1306Wire.h>

#if !defined(__TRIANGLE)
#define __TRIANGLE

extern SSD1306Wire *display;
class Triangle: public Geometry
{
private:
  pt2D_t cube_pt[4];
  pt3D_t orig_points[4] = {
  {-1,-1,1},
  {1,-1,1},
  {0,1,1},
  {0,0,-1}
};

float  z_offset = -4;
float cube_size = 70;
pt3D_t rotated_points [4];
public:
  Triangle(/* args */);
  ~Triangle();
  float shoelace(pt2D_t a,pt2D_t b,pt2D_t c);
  void hiddenCorner(pt2D_t a,pt2D_t b);
  void drawFace(pt2D_t a,pt2D_t b,pt2D_t c);
  void calcul(void);
 void inc_Angle(int inc);
  void render();
  void Translate(pt3D_t vector);
};

Triangle::Triangle(/* args */)
{
}

Triangle::~Triangle()
{
}
float Triangle::shoelace(pt2D_t a,pt2D_t b,pt2D_t c){
  return {
    (a.x*b.y - a.y*b.x) + 
    (b.x*c.y - b.y*c.x) + 
    (c.x*a.y - c.y*a.x) 
  };
}
void Triangle::hiddenCorner(pt2D_t a,pt2D_t b){
   int diviser = 3;
  for (size_t i = 0; i < diviser; i++)
  {
    display->setPixel((i*(b.x - a.x)/diviser)+a.x,(i*(b.y - a.y)/diviser)+a.y);
  }
  
}
void Triangle::drawFace(pt2D_t a,pt2D_t b,pt2D_t c){
  
  if (shoelace(a,b,c) >=0)
  {
    display->drawLine(a.x,a.y,b.x,b.y);
    display->drawLine(b.x,b.y,c.x,c.y);
    display->drawLine(c.x,c.y,a.x,a.y);
    

  } else
  {
    display->setPixel(a.x,a.y);
    display->setPixel(b.x,b.y);
    display->setPixel(c.x,c.y);
    

    hiddenCorner(a,b);
    hiddenCorner(b,c);
    hiddenCorner(c,a);
    
  }
  
  
}
void Triangle::calcul(void){
  for (size_t i = 0; i < 4; i++)
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

void Triangle::render(void){
  drawFace(cube_pt[0],cube_pt[1],cube_pt[2]);
  drawFace(cube_pt[3],cube_pt[1],cube_pt[0]);
  drawFace(cube_pt[3],cube_pt[0],cube_pt[2]);
  drawFace(cube_pt[2],cube_pt[1],cube_pt[3]);

}
void Triangle::Translate(pt3D_t vector){
  for (size_t i = 0; i < 4; i++)
  {
    orig_points[i].x += vector.x;
    orig_points[i].y += vector.y;
    orig_points[i].z += vector.z;
  }
  
  
}

void Triangle::inc_Angle(int inc){
  this->angle_deg += inc;
}



#endif // __TRIANGLE
