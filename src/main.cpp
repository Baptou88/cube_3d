#include <Arduino.h>
#include <heltec.h>

SSD1306Wire *display = Heltec.display;

struct pt2D_t {
  float x ;
  float y ;
};
struct pt3D_t {
  float x ;
  float y ;
  float z ;
};

pt2D_t cube_pt[8];
// pt2D_t cube_pt[] = {
//   {37.6867235218178,12.7373447043636},
//   {73.7274204169332,5.42419319435559},
//   {73.7274204169332,58.5758068056444},
//   {37.6867235218178,51.2626552956365},
//   {59.2251850602793,18.9549629879441},
//   {85.9013334604115,15.9671111534705},
//   {85.9013334604115,48.0328888465295},
//   {59.2251850602793,45.0450370120559},
// };

float angle_deg = 30;
float  z_offset = -4;
float cube_size = 70;

pt3D_t orig_points[] = {
  {-1,-1,1},
  {1,	-1,1},
  {1,	1,1},
  {-1,1,1},
  {-1,-1,-1},
  {1,	-1,-1},
  {1,	1,-1},
  {-1,1,-1}
};
pt3D_t rotated_points [8];
  
// pt2D_t cube_pt[] = 
//   {
//     {64-28,32-28},  //1
//     {64+28,32-28},  //2
//     {64+28,32+28},  //3
//     {64-28,32+28},  //4

//     {64-16,32-16},  //5
//     {64+16,32-16},  //6
//     {64+16,32+16},  //7
//     {64-16,32+16}   //8
//   };
float shoelace(pt2D_t a,pt2D_t b,pt2D_t c,pt2D_t d){
  return {
    (a.x*b.y - a.y*b.x) + 
    (b.x*c.y - b.y*c.x) + 
    (c.x*d.y - c.y*d.x) + 
    (d.x*a.y - d.y*a.x)
  };
}
void calcul(void){
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
void setup() {
  // put your setup code here, to run once:

  Heltec.begin(true,true,true,true,868E6);

}

void loop() {
  // put your main code here, to run repeatedly:

  calcul();

  display->clear();

    //face A
    if (shoelace(cube_pt[0],cube_pt[1],cube_pt[2],cube_pt[3])>=0)
    {
      display->drawLine(cube_pt[0].x,cube_pt[0].y,cube_pt[1].x,cube_pt[1].y);
      display->drawLine(cube_pt[1].x,cube_pt[1].y,cube_pt[2].x,cube_pt[2].y);
      display->drawLine(cube_pt[2].x,cube_pt[2].y,cube_pt[3].x,cube_pt[3].y);
      display->drawLine(cube_pt[3].x,cube_pt[3].y,cube_pt[0].x,cube_pt[0].y);

    }
  

  display->drawLine(cube_pt[4].x,cube_pt[4].y,cube_pt[5].x,cube_pt[5].y);
  display->drawLine(cube_pt[5].x,cube_pt[5].y,cube_pt[6].x,cube_pt[6].y);
  display->drawLine(cube_pt[6].x,cube_pt[6].y,cube_pt[7].x,cube_pt[7].y);
  display->drawLine(cube_pt[7].x,cube_pt[7].y,cube_pt[4].x,cube_pt[4].y);

  display->drawLine(cube_pt[0].x,cube_pt[0].y,cube_pt[4].x,cube_pt[4].y);
  display->drawLine(cube_pt[1].x,cube_pt[1].y,cube_pt[5].x,cube_pt[5].y);
  display->drawLine(cube_pt[2].x,cube_pt[2].y,cube_pt[6].x,cube_pt[6].y);
  display->drawLine(cube_pt[3].x,cube_pt[3].y,cube_pt[7].x,cube_pt[7].y);

  display->display();

  angle_deg += 1;

  delay(50);
}