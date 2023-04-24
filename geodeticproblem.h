#ifndef GEODETICPROBLEM_H
#define GEODETICPROBLEM_H

#include <math.h>
#include <QPoint>

#define A_E 6371.1 //radius Earth
#define Degrees(x) (x * (180 / M_PI))	// radians -> degrees
#define Radians(x) (x / (180 / M_PI))	// degrees -> radians

class geodeticProblem
{
public:
    geodeticProblem();

    bool setData(double latB, double lonB, double angleRevers, double distance);

    void directGDGeod(double latA, double lonA, double angle, double distance, double largeSemiAxisEllipsoid, double eccentricityEllipsoidFirst, double eccentricityEllipsoidSecond, double &latB, double &lonB, double &angleRevers);

    void directGDPlane(double latA, double lonA, double angle, double distance, double &latB, double &lonB);

    void directGDPlaneH(double latA, double lonA, double AH, double angleDir, double angleOrent, double distance, double &latB, double &lonB, double &BH);

    void directGDSphera(double latA, double lonA, double angle, double distance, double &latB, double &lonB);

    void inversGDSphera(double lat[2], double lon[2], double &angle, double &distance);

    void inversGDPlane(double lat[2], double lon[2], double &angle, double &distance);

    void inversGDPlaneH(double lat[2], double lon[2], double height[2], double &angleDir, double &distance, double &angleOrent);

private:

    double lat[2];
    double lon[2];
    double angle[2];
    double distance;
    double largeSemiAxisEllipsoid;
    double eccentricityEllipsoidFirst;
    double eccentricityEllipsoidSecond;

    /*
     * Решение прямой геодезической задачи
     *
     * Аргументы исходные:
     *     pt1  - {широта, долгота} точки Q1
     *     azi  - азимут начального направления
     *     dist - расстояние (сферическое)
     *
     * Аргументы определяемые:
     *     pt2  - {широта, долгота} точки Q2
     */
    void SphereDirect(double pt1[], double azi, double dist, double pt2[])
    {
      double pt[2], x[3];

      pt[0] = M_PI_2 - dist;
      pt[1] = M_PI - azi;
      SpherToCart(pt, x);			// сферические -> декартовы
      Rotate(x, pt1[0] - M_PI_2, 1);	// первое вращение
      Rotate(x, -pt1[1], 2);		// второе вращение
      CartToSpher(x, pt2);	     		// декартовы -> сферические

      return;
    }

    /*
     * Решение обратной геодезической задачи
     *
     * Аргументы исходные:
     *     pt1  - {широта, долгота} точки Q1
     *     pt2  - {широта, долгота} точки Q2
     *
     * Аргументы определяемые:
     *     azi  - азимут начального направления
     *     dist - расстояние (сферическое)
     */
    void SphereInverse(double pt1[], double pt2[], double *azi, double *dist)
    {
      double x[3], pt[2];

      SpherToCart(pt2, x);			// сферические -> декартовы
      Rotate(x, pt1[1], 2);			// первое вращение
      Rotate(x, M_PI_2 - pt1[0], 1);	// второе вращение
      CartToSpher(x, pt);	     		// декартовы -> сферические
      *azi = M_PI - pt[1];
      *dist = M_PI_2 - pt[0];

      return;
    }

    /*
     * Преобразование сферических координат в вектор
     *
     * Аргументы исходные:
     *     y - {широта, долгота}
     *
     * Аргументы определяемые:
     *     x - вектор {x, y, z}
     */
    void SpherToCart(double y[], double x[])
    {
      double p;

      p = cos(y[0]);
      x[2] = sin(y[0]);
      x[1] = p * sin(y[1]);
      x[0] = p * cos(y[1]);

      return;
    }

    /*
     * Вращение вокруг координатной оси
     *
     * Аргументы:
     *     x - входной/выходной 3-вектор
     *     a - угол вращения
     *     i - номер координатной оси (0..2)
     */
    void Rotate(double x[], double a, int i)
    {
      double c, s, xj;
      int j, k;

      j = (i + 1) % 3;
      k = (i - 1) % 3;
      c = cos(a);
      s = sin(a);
      xj = x[j] * c + x[k] * s;
      x[k] = -x[j] * s + x[k] * c;
      x[j] = xj;

      return;
    }

    /*
     * Преобразование вектора в сферические координаты
     *
     * Аргументы исходные:
     *     x - {x, y, z}
     *
     * Аргументы определяемые:
     *     y - {широта, долгота}
     *
     * Возвращает:
     *     длину вектора
     */
    double CartToSpher(double x[], double y[])
    {
      double p;

      p = hypot(x[0], x[1]);
      y[1] = atan2(x[1], x[0]);
      y[0] = atan2(x[2], p);

      return hypot(p, x[2]);
    }
};

#endif // GEODETICPROBLEM_H
