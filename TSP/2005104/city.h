#ifndef CITY_H
#define CITY_H
class City{
    public:
    int id;
    double x;
    double y;
    City(int id):id(id),x(0),y(0){}

    City(int id,double x,double y): id(id),x(x),y(y){}

    void setX(double x1){
        x=x1;
    }

    double getX(){
        return x;
    }

    void setY(double y1){
        y=y1;
    }

    double getY(){
        return y;
    }

    double distanceTo(const City &other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }
};
#endif // CITY_H
