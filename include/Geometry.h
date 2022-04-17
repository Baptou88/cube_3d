#if !defined(__GEOMETRY)
#define __GEOMETRY

struct pt2D_t {
  float x ;
  float y ;
};
struct pt3D_t {
  float x ;
  float y ;
  float z ;
};

class Geometry
{
private:
public:
    Geometry(/* args */);
    ~Geometry();
    void inc_Angle(int inc);
    
protected:
    float angle_deg = 30;

};

Geometry::Geometry(/* args */)
{
}

Geometry::~Geometry()
{
}
void Geometry::inc_Angle(int inc){
  this->angle_deg += inc;
}


#endif // __GEOMETRY
