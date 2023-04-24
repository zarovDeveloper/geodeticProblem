#include "geodeticproblem.h"
using namespace std;

geodeticProblem::geodeticProblem()
{

}


//direct geodesic problem in Plane, PlaneH, Sphere(need work), Geod (need work)

void geodeticProblem::directGDPlane(double latA, double lonA, double angle, double distance, double &latB, double &lonB) //directGD on Plane
{
    double deltaX = distance * cos(Radians(angle));
    double deltaY = distance * sin(Radians(angle));

    latB = latA + deltaX; //lat point B
    lonB = lonA + deltaY; //lon point B
}

void geodeticProblem::directGDPlaneH(double latA, double lonA, double AH, double angleDir, double angleOrent, double distance, double &latB, double &lonB, double &BH) //directGD on Plane with height
{
    double deltaX = distance * cos(Radians(angleDir));
    double deltaY = distance * sin(Radians(angleDir));

    latB = latA + deltaX; //lat point B
    lonB = lonA + deltaY; //lon point B

    BH = AH + distance * sin(Radians(angleOrent)); //height point B
}

void geodeticProblem::directGDSphera(double latA, double lonA, double angle, double distance, double &latB, double &lonB)
{
    double pt1[2], pt2[2];

    pt1[0] = Radians(latA);
    pt1[1] = Radians(lonA);

    SphereDirect(pt1, Radians(angle), distance / A_E, pt2);

    latB = Degrees(pt2[0]);
    lonB = Degrees(pt2[1]);
    angle = Degrees(angle);
}

void geodeticProblem::directGDGeod(double latA, double lonA, double angl, double s, double LSAE, double EEF, double EES, double &latB, double &lonB, double &angleRevers)
{
    //need work
}


//invers geodesic problem in Plane, Sphere, Geod


void geodeticProblem::inversGDPlane(double *lat, double *lon, double &angle, double &distance) //inverseGD on Plane
{
    double deltaX = lat[1] - lat[0];
    double deltaY = lon[1] - lon[0];

    angle = Degrees(atan(deltaY / deltaX)); //directional angle

    distance = sqrt(deltaX*deltaX + deltaY*deltaY); //distance between point A and point B

    //coordinate increments
    if (deltaX < 0 and deltaY > 0) angle -= 180;
    else if (deltaX < 0 and deltaY < 0) angle += 180;
    else if (deltaX > 0 and deltaY < 0) return;
}

void geodeticProblem::inversGDPlaneH(double *lat, double *lon, double *height, double &angleDir, double &distance, double &angleOrent)
{
    double deltaX = lat[1] - lat[0];
    double deltaY = lon[1] - lon[0];

    angleDir = Degrees(atan(deltaY / deltaX)); //directional angle

    distance = sqrt(deltaX*deltaX + deltaY*deltaY); //distance between point A and point B

    //coordinate increments
    if (deltaX < 0 and deltaY > 0) angleDir -= 180;
    else if (deltaX < 0 and deltaY < 0) angleDir += 180;
    else if (deltaX > 0 and deltaY < 0) return;

    //angle between point A and point B
    angleOrent = Degrees(asin((height[1] - height[0]) / distance));
}

void geodeticProblem::inversGDSphera(double *lat, double *lon, double &angle, double &distance)
{
    double pt1[2], pt2[2];

    pt1[0] = Radians(lat[0]);
    pt1[1] = Radians(lon[0]);
    pt2[0] = Radians(lat[1]);
    pt2[1] = Radians(lon[1]);

    SphereInverse(pt2, pt1, &angle, &distance);		// Решение обратной задачи
    SphereInverse(pt1, pt2, &angle, &distance);		// Вычисление обратного азимута

    distance *= A_E;
    angle = Degrees(angle);
}
