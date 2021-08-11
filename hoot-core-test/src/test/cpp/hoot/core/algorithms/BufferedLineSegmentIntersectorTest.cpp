/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014, 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/BufferedLineSegmentIntersector.h>
#include <hoot/core/util/Log.h>

// Tgs
#include <tgs/Statistics/Random.h>
#include <tgs/System/Time.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class BufferedLineSegmentIntersectorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(BufferedLineSegmentIntersectorTest);
  CPPUNIT_TEST(runCircleTest);
  CPPUNIT_TEST(runRandomTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  BufferedLineSegmentIntersectorTest() = default;

  bool geosIntersect(const LineSegment& ls1, Meters buffer, const LineSegment& ls2,
                     LineSegment& result)
  {
    std::shared_ptr<Geometry> g1(ls1.toGeometry(*GeometryFactory::getDefaultInstance())->clone());
    std::shared_ptr<Geometry> g2(ls2.toGeometry(*GeometryFactory::getDefaultInstance())->clone());
    std::shared_ptr<Geometry> g(g1->buffer(buffer, 40));

    std::shared_ptr<Geometry> i(g->intersection(g2.get()));

    if (i->isEmpty())
    {
      result.p0 = Coordinate::getNull();
      return false;
    }
    else if (i->getGeometryTypeId() == GEOS_POINT)
    {
      Point* p = dynamic_cast<Point*>(i.get());
      result.p0 = *(p->getCoordinate());
    }
    else if (i->getGeometryTypeId() == GEOS_LINESTRING)
    {
      LineString* ls = dynamic_cast<LineString*>(i.get());
      assert(ls->getNumPoints() == 2);
      //  getPointN creates a new Point object that needs to be deleted
      std::shared_ptr<Point> p0(ls->getPointN(0));
      std::shared_ptr<Point> p1(ls->getPointN(1));
      result.p0 = *p0->getCoordinate();
      result.p1 = *p1->getCoordinate();
    }
    else
    {
      throw HootException();
    }
    return true;
  }

  void runCircleTest()
  {
    {
      LineSegment ls(-2, 0.5, 2, 0.5);
      Coordinate origin(0, 0);

      Coordinate p1, p2;
      BufferedLineSegmentIntersector uut;
      uut.circleIntersection(origin, 1, ls, p1, p2);

      HOOT_STR_EQUALS("0.866025 0.5, -0.866025 0.5", p1 << ", " << p2);
    }

    {
      LineSegment ls(-1, 2.5, 3, 2.5);
      Coordinate origin(1, 2);

      Coordinate p1, p2;
      BufferedLineSegmentIntersector uut;
      uut.circleIntersection(origin, 1, ls, p1, p2);

      HOOT_STR_EQUALS("1.86603 2.5, 0.133975 2.5", p1 << ", " << p2);
    }

    // tangent at the top of the circle
    {
      LineSegment ls(-1, 3, 3, 3);
      Coordinate origin(1, 2);

      Coordinate p1, p2;
      BufferedLineSegmentIntersector uut;
      uut.circleIntersection(origin, 1, ls, p1, p2);

      HOOT_STR_EQUALS("1 3, nan nan", p1 << ", " << p2);
    }

    // arbitrary line that was causing problems. Intersections are outside the line segment.
    {
      LineSegment ls(0.648952, 2.00472, 0.783395, 4.02088);
      Coordinate origin(0.686158, 1.21443);

      Coordinate p1, p2;
      BufferedLineSegmentIntersector uut;
      uut.circleIntersection(origin, 0.424808, ls, p1, p2);

      HOOT_STR_EQUALS("nan nan, nan nan", p1 << ", " << p2);
    }
  }

  void runRandomTest()
  {
    BufferedLineSegmentIntersector uut;

    double scale = 5;

    double uutTime = 0.0;
    double geosTime = 0.0;
    // the geos intersection is an approximation so we need a fairly large epsilon
    double epsilon = 1e-2;
    size_t count = 1000;
    for (size_t i = 0; i < count; i++)
    {
      double r = Tgs::Random::instance()->generateUniform() * 3.0;
      LineSegment ls1(Tgs::Random::instance()->generateUniform() * scale,
                      Tgs::Random::instance()->generateUniform() * scale,
                      Tgs::Random::instance()->generateUniform() * scale,
                      Tgs::Random::instance()->generateUniform() * scale);
      LineSegment ls2(Tgs::Random::instance()->generateUniform() * scale,
                      Tgs::Random::instance()->generateUniform() * scale,
                      Tgs::Random::instance()->generateUniform() * scale,
                      Tgs::Random::instance()->generateUniform() * scale);

      LineSegment lsGeos;
      double start = Tgs::Time::getTime();
      bool resultGeos = geosIntersect(ls1, r, ls2, lsGeos);
      double end = Tgs::Time::getTime();
      geosTime += end - start;

      LineSegment lsUut;
      start = Tgs::Time::getTime();
      bool resultUut = uut.intersect(ls1, r, ls2, lsUut);
      end = Tgs::Time::getTime();
      uutTime += end - start;

      // if the line is very close to the buffer then we can't expect consistent results.
      bool touchy = fabs(ls1.distance(ls2) - r) <= epsilon;
      CPPUNIT_ASSERT_EQUAL(resultGeos || touchy, resultUut || touchy);
      if (resultGeos && !touchy)
      {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lsGeos.p0.distance(lsUut.p0), epsilon);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, lsGeos.p1.distance(lsUut.p1), epsilon);
      }
    }

    LOG_INFO("GEOS Mean Time: " << (1000.0 * geosTime) / (double)count << "ms");
    LOG_INFO("UUT Mean Time: " << (1000.0 * uutTime) / (double)count << "ms");
    LOG_INFO("Ratio: " << geosTime / uutTime);
  }

  void runTest()
  {
    BufferedLineSegmentIntersector uut;

    {
      LineSegment ls1(0, 0, 1, 1);
      LineSegment ls2(0, 0, 1, 1);
      LineSegment result;
      HOOT_STR_EQUALS(true, uut.intersect(ls1, 1, ls2, result));
      HOOT_STR_EQUALS("LINESEGMENT(0 0,1 1)",
                      result);
    }

    {
      LineSegment ls1(0, 1, 1, 0);
      LineSegment ls2(0, 0, 1, 1);
      LineSegment result;
      HOOT_STR_EQUALS(true, uut.intersect(ls1, .14142135623730950488016887242097, ls2, result));
      HOOT_STR_EQUALS("LINESEGMENT(0.4 0.4,0.6 0.6)",
                      result);
    }

    {
      LineSegment ls1(2, 0, 3, 2);
      LineSegment ls2(2, 0, 4, 4);
      LineSegment result;
      HOOT_STR_EQUALS(true, uut.intersect(ls1, .1, ls2, result));
      HOOT_STR_EQUALS("LINESEGMENT(2 0,3.04472 2.08944)", result);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(BufferedLineSegmentIntersectorTest, "quick");

}
