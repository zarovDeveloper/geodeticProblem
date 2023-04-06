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
}

void geodeticProblem::directGDGeod(double latA, double lonA, double angl, double s, double LSAE, double EEF, double EES, double &latB, double &lonB, double &angleRevers)
{
    lat[0] = latA;
    lon[0] = lonA;
    angle[0] = angl;

    distance = s;
    largeSemiAxisEllipsoid = LSAE;
    eccentricityEllipsoidFirst = EEF;
    eccentricityEllipsoidSecond = EES;

    double u[2];
    double m[2];
    double k;
    double alpha, beta, gamma, sigma;
    double kf[2];
    double ro = 206264.80624709; //the number of angular seconds in radians


    u[0] = atan(sqrt(1 - eccentricityEllipsoidFirst) * tan(lat[0]));

    m[0] = atan(tan(u[0]) / cos(angle[0]));
    m[1] = asin(sin(angle[0]) * cos(u[0]));

    k = sqrt(eccentricityEllipsoidSecond * cos(m[1]) * cos(m[1]));

    double r[3];

    r[0] = 1 + k * k / 4 - (3 * pow(k, 4)) / 64;
    r[1] = pow(k, 2) / 4 - pow(k, 4) / 16;
    r[2] = pow(k, 4) / 128;

    alpha = ro / (largeSemiAxisEllipsoid * r[0]);
    beta = r[1] / r[0];
    gamma = r[2] / r[0];

    kf[0] = (0.5 + eccentricityEllipsoidFirst / 8 - eccentricityEllipsoidFirst / 16 * cos(m[1]) * cos(m[1])) * eccentricityEllipsoidFirst;
    kf[1] = (cos(m[1]) * cos(m[1]) * ro * pow(eccentricityEllipsoidFirst, 2)) / 16;

    double temp[4];

    temp[0] = (alpha * distance) / 3600 * M_PI / 180;
    temp[1] = temp[0] + beta * sin(temp[0]) * cos(2 * m[0] + temp[0]);
    temp[2] = temp[0] + beta * sin(temp[1]) * cos(2 * m[0] + temp[1]) + gamma * sin(2 * temp[1]) * cos(4 * m[0] + 2 * temp[1]);
    temp[3] = temp[0] + beta * sin(temp[2]) * cos(2 * m[0] + temp[2]) + gamma * sin(2 * temp[2]) * cos(4 * m[0] + 2 * temp[2]);

    while(fabs(temp[2] - temp[3]) > 0.001){
        temp[2] = temp[3];
        temp[3] = temp[0] + beta * sin(temp[2]) * cos(2 * m[0] + temp[2]) + gamma * sin(2 * temp[2]) * cos(4 * m[0] + 2 * temp[2]);
    }
    sigma = temp[3];

    u[1] = asin(cos(m[1]) * sin(m[0] + sigma));
    angle[1] = asin(sin(m[1]) / cos(u[1]));


    if(angle[1] > 0)
        angle[1] += M_PI;
    if(angle[1] < 0)
        angle[1] = fabs(angle[1]);

    double omega;

    omega = asin((sin(sigma) * sin(angle[0])) / cos(u[1]));

    double deltaLon;

    deltaLon = omega - sin(m[1]) * (kf[0] * sigma + kf[1] * sin(sigma) * cos(2 * m[0] + sigma));

    lon[1] = lon[0] + deltaLon;
    lat[1] = fabs(atan(tan(u[1]) / sqrt(1 - eccentricityEllipsoidFirst)));

    lat[1] = lat[1] * 180 / M_PI;
    lon[1] = lon[1] * 180 / M_PI;
    angle[1] = angle[1] * 180 / M_PI;

    latB = lat[1];
    lonB = lon[1];
    angleRevers = angle[1];
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

  SphereInvers(pt2, pt1, angle, distance); //solve invers GD

  angle = Degrees(angle);
  distance *= A_E;
}
