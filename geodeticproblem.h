#ifndef GEODETICPROBLEM_H
#define GEODETICPROBLEM_H

#include <math.h>
#include <QPoint>

#define A_E 6371.1 //radius Earth
#define Degrees(x) (x * 57.29577951308232)	// radians -> degrees
#define Radians(x) (x / 57.29577951308232)	// degrees -> radians

class geodeticProblem
{
public:
    geodeticProblem();

    bool setData(double latB, double lonB, double angleRevers, double distance);

    void directGDGeod(double latA, double lonA, double angle, double distance, double largeSemiAxisEllipsoid, double eccentricityEllipsoidFirst, double eccentricityEllipsoidSecond, double &latB, double &lonB, double &angleRevers);

    void directGDPlane(double latA, double lonA, double angle, double distance, double &latB, double &lonB);

    void directGDSphera(double latA, double lonA, double angle, double distance, double &latB, double &lonB);

    void inversGDSphera(double lat[2], double lon[2], double &angle, double &distance);

    void inversGDPlane(double lat[2], double lon[2], double &angle, double &distance);

    void directGDPlaneH(double latA, double lonA, double height, double angle, double distance, double &latB, double &lonB);

private:

    double lat[2];
    double lon[2];
    double angle[2];
    double distance;
    double largeSemiAxisEllipsoid;
    double eccentricityEllipsoidFirst;
    double eccentricityEllipsoidSecond;

    /*
     * Преобразование сферических координат в вектор
     *
     * Аргументы исходные:
     *     y - {широта, долгота}
     *
     * Аргументы определяемые:
     *     x - вектор {x, y, z}
     */
    void spherToCart(double y[], double x[])
    {
      double p;

      p = cos(y[0]);
      x[2] = sin(y[0]);
      x[1] = p * sin(y[1]);
      x[0] = p * cos(y[1]);
    }

    /*
     * Вращение вокруг координатной оси
     *
     * Аргументы:
     *     x - входной/выходной 3-вектор
     *     a - угол вращения
     *     i - номер координатной оси (0..2)
     */
    void rotate(double x[], double a, int i)
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
    double cartToSpher(double x[], double y[])
    {
      double p;

      p = hypot(x[0], x[1]);
      y[1] = atan2(x[1], x[0]);
      y[0] = atan2(x[2], p);

      return hypot(p, x[2]);
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
    void SphereInvers(double pt1[], double pt2[], double &azi, double &dist)
    {
      double x[3], pt[2];

      spherToCart(pt2, x);			// сферические -> декартовы
      rotate(x, pt1[1], 2);			// первое вращение
      rotate(x, M_PI_2 - pt1[0], 1);	// второе вращение
      cartToSpher(x, pt);	     		// декартовы -> сферические
      azi = M_PI - pt[1];
      dist = M_PI_2 - pt[0];
    }

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
      spherToCart(pt, x);			// сферические -> декартовы
      rotate(x, pt1[0] - M_PI_2, 1);	// первое вращение
      rotate(x, -pt1[1], 2);		// второе вращение
      cartToSpher(x, pt2);	     		// декартовы -> сферические

      return;
    }
};

#endif // GEODETICPROBLEM_H
