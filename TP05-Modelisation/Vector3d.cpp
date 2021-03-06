#include "stdafx.h"
#include "vector3d.h"
#ifndef dabs
    #define dabs(x) x>0?x:-x
#endif

Vector3D::Vector3D(const Vector2D &vector):xp(vector.x()), yp(vector.y()), zp(0.0){}

Vector3D::Vector3D(const Vector2D &vector, double zpos):xp(vector.x()), yp(vector.y()), zp(zpos){}


Vector3D Vector3D::normal(const Vector3D &v1, const Vector3D &v2)
{
    return (v1^v2).normalized();
}

Vector3D Vector3D::normal(const Vector3D &v1, const Vector3D &v2, const Vector3D &v3)
{
    Vector3D a=v2-v1;
    Vector3D b=v3-v1;
    return (a^b).normalized();
}

double Vector3D::distanceToPlane(const Vector3D &plane, const Vector3D &normal) const
{
    double d=-(plane*normal);
    double num=dabs((*this)*(normal)+d);
    return num/normal.length();

}

double Vector3D::distanceToPlane(const Vector3D &plane1, const Vector3D &plane2, const Vector3D &plane3) const
{
    Vector3D normal = Vector3D::normal(plane1,plane2,plane3);
    return distanceToPlane(plane1, normal);
}

double Vector3D::distanceToLine(const Vector3D &point, const Vector3D &direction) const
{
   double t= (point*direction-(*this)*direction)/(direction*direction);
   return distanceToPoint(point+t*direction);
}
