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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/algorithms/FrechetDistance.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

#include <hoot/core/io/OsmReader.h>

#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/MapProjector.h>

#include "../TestUtils.h"

using namespace hoot;

namespace hoot
{

class FrechetDistanceTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(FrechetDistanceTest);
  CPPUNIT_TEST(simpleFrechet);
  CPPUNIT_TEST(tailFrechet);
  CPPUNIT_TEST(homologousTest);
  CPPUNIT_TEST(partialTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void simpleFrechet()
  {
    OsmMapPtr map = createTestMapSimple();
    WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w1")[0]);
    WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w2")[0]);

    FrechetDistance fd_simple(map, w1, w2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.00, fd_simple.distance(), 0.01);
  }

  void tailFrechet()
  {
    OsmMapPtr map = createTailMap();
    WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w1")[0]);
    WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w2")[0]);

    FrechetDistance fd(map, w1, w2);

    frechet_subline subline = fd.maxSubline(5);
    frechet_subline optimal_subline = createTailSubline();

    CPPUNIT_ASSERT_EQUAL(optimal_subline.size(), subline.size());

    for (frechet_subline::size_type i = 0; i < subline.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal_subline[i].first, subline[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal_subline[i].second, subline[i].second);
    }
  }

  frechet_subline createTailSubline()
  {
    frechet_subline optimal_subline;
    optimal_subline.push_back(vertex_match(0, 0));
    optimal_subline.push_back(vertex_match(1, 1));
    optimal_subline.push_back(vertex_match(2, 2));
    optimal_subline.push_back(vertex_match(3, 3));
    return optimal_subline;
  }

  OsmMapPtr createTailMap()
  {
    OsmMapPtr map = createEmptyMap();

    Coordinate c1[] = {
      Coordinate(10.0, 10.0), Coordinate(20.0, 20.0), Coordinate(30.0, 15.0), Coordinate(40.0, 20.0), Coordinate(50.0, 10.0), Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");

    Coordinate c2[] = {
      Coordinate(10.0, 12.0), Coordinate(20.0, 22.0), Coordinate(30.0, 17.0), Coordinate(40.0, 22.0), Coordinate(45.0, 32.0), Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c2, 5, "w2");

    return map;
  }

  OsmMapPtr createEmptyMap()
  {
    OsmMap::resetCounters();
    OsmMapPtr map(new OsmMap());
    OGREnvelope env;
    env.MinX = 0;
    env.MinY = 0;
    env.MaxX = 1;
    env.MaxY = 1;
    MapProjector::projectToOrthographic(map, env);
    return map;
  }

  OsmMapPtr createTestMapSimple()
  {
    OsmMapPtr map = createEmptyMap();

    Coordinate c1[] = {
      Coordinate(1.0, 2.0), Coordinate(21.0, 22.0), Coordinate(31.0, 32.0), Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");

    Coordinate c2[] = {
      Coordinate(1.0, 1.0), Coordinate(11.0, 11.0),  Coordinate(31.0, 31.0), Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c2, 5, "w2");

    return map;
  }

  void homologousTest()
  {
    /** Example taken from "A new merging process for data integration based on the discrete Fréchet distance"
     *  http://thomas.devogele.free.fr/articles/avant_2003_selection/DevogeleSDH2002.pdf
     *
     * Table 1 - Matrix of dE between (L1.i, L2.j) distance and ways
     */
    const int rows = 7;
    const int cols = 8;
    Meters values[rows][cols] = {
      {  0.86,  2.33,  5.55,  9.97, 10.67, 10.64,  9.52,  6.01 },
      {  2.52,  0.92,  4.18,  8.85,  9.62,  9.70,  8.78,  5.39 },
      {  3.45,  0.91,  3.10,  7.80,  8.60,  8.74,  7.93,  4.70 },
      {  6.97,  4.70,  1.49,  3.83,  4.68,  5.02,  4.75,  3.30 },
      { 11.54,  9.87,  6.60,  1.90,  1.17,  1.49,  2.97,  6.00 },
      {  9.53,  8.43,  5.68,  2.63,  2.33,  1.68,  0.71,  4.01 },
      {  5.72,  4.85,  3.09,  4.47,  4.94,  4.70,  3.40,  0.63 }
    };

    OsmMapPtr map = createTestMapHomologous();
    WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w1")[0]);
    WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w2")[0]);

    FrechetDistance fd(map, w1, w2);

    for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(values[r][c], fd._matrix[r][c], 0.01);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.17, fd.distance(), 0.01);

    frechet_subline subline = fd.maxSubline(2);
    frechet_subline optimal = createTestSublineHomologous();

    CPPUNIT_ASSERT_EQUAL(optimal.size(), subline.size());

    for (frechet_subline::size_type i = 0; i < subline.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal[i].first, subline[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal[i].second, subline[i].second);
    }
  }

  void partialTest()
  {
    /** Example taken from "A new merging process for data integration based on the discrete Fréchet distance"
     *  http://thomas.devogele.free.fr/articles/avant_2003_selection/DevogeleSDH2002.pdf
     *
     *  Table 2 - Matrix of dE between the partial homologous lines of figure 5
     */
    const int rows = 9;
    const int cols = 17;
    Meters values[rows][cols] = {
      {  1.60,  0.67,  1.78,  1.17,  0.72,  2.03,  3.62,  4.55,  6.14,  7.74,  8.55,  9.77, 11.40, 13.10, 12.80, 13.20, 12.70 },
      {  3.64,  2.62,  3.82,  2.97,  1.75,  0.11,  1.55,  2.58,  4.17,  5.77,  6.65,  7.89,  9.58, 11.20, 10.90, 11.30, 10.90 },
      {  5.73,  4.71,  5.91,  5.01,  3.80,  2.18,  0.73,  1.22,  2.44,  3.94,  4.91,  6.16,  7.78,  9.37,  9.16,  9.65,  9.35 },
      {  7.36,  6.35,  7.68,  6.82,  5.60,  3.95,  2.31,  1.51,  0.89,  2.10,  3.11,  4.34,  5.92,  7.50,  7.31,  7.81,  7.56 },
      {  9.08,  8.10,  9.49,  8.66,  7.44,  5.79,  4.14,  3.18,  1.63,  0.58,  1.50,  2.60,  4.08,  5.64,  5.47,  5.99,  5.82 },
      {  9.72,  8.77, 10.20,  9.44,  8.22,  6.58,  4.95,  3.91,  2.30,  0.71,  0.56,  1.58,  3.14,  4.74,  4.52,  5.02,  4.81 },
      { 11.30, 10.40, 11.80, 11.00,  9.86,  8.22,  6.58,  5.54,  3.94,  2.34,  1.63,  0.94,  1.62,  3.14,  3.00,  3.55,  3.52 },
      { 13.00, 12.10, 13.50, 12.70, 11.50,  9.93,  8.29,  7.26,  5.66,  4.06,  3.31,  2.26,  0.94,  1.58,  1.70,  2.30,  2.69 },
      { 14.70, 13.70, 15.20, 14.40, 13.20, 11.50,  9.92,  8.90,  7.30,  5.70,  4.93,  3.81,  2.16,  0.94,  1.57,  1.91,  2.75 }
    };

    OsmMapPtr map = createTestMapPartial();
    WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w1")[0]);
    WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w2")[0]);

    FrechetDistance fd(map, w1, w2);

    for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(values[r][c], fd._matrix[r][c], 0.1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.59, fd.distance(), 0.01);

    frechet_subline subline = fd.maxSubline(1.25);
    frechet_subline optimal = createTestSublinePartial();

    CPPUNIT_ASSERT_EQUAL(optimal.size(), subline.size());

    for (frechet_subline::size_type i = 0; i < subline.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal[i].first, subline[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal[i].second, subline[i].second);
    }
  }

  OsmMapPtr createTestMapHomologous()
  {
    OsmMapPtr map = createEmptyMap();

    Coordinate c1[] = {
      Coordinate(0, 0.7),    Coordinate(0.8, 2.5),  Coordinate(1.8, 3.2), Coordinate(5.9, 4.4),
      Coordinate(11.5, 3.5), Coordinate(10, 0.8),   Coordinate(6.2, 0.5), Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");

    Coordinate c2[] = {
      Coordinate(0.5, 0),    Coordinate(1.7, 2.3),  Coordinate(4.9, 3.3), Coordinate(9.6, 3.4),
      Coordinate(10.4, 3.1), Coordinate(10.5, 2.4), Coordinate(9.5, 1.3), Coordinate(6, 1.1),
      Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c2, 5, "w2");

    return map;
  }

  frechet_subline createTestSublineHomologous()
  {
    frechet_subline optimal_subline;
    optimal_subline.push_back(vertex_match(0, 0));
    optimal_subline.push_back(vertex_match(1, 1));
    optimal_subline.push_back(vertex_match(2, 1));
    optimal_subline.push_back(vertex_match(3, 2));
    optimal_subline.push_back(vertex_match(4, 3));
    optimal_subline.push_back(vertex_match(4, 4));
    optimal_subline.push_back(vertex_match(4, 5));
    optimal_subline.push_back(vertex_match(5, 6));
    optimal_subline.push_back(vertex_match(6, 7));
    return optimal_subline;
  }

  OsmMapPtr createTestMapPartial()
  {
    OsmMapPtr map = createEmptyMap();

    Coordinate c1[] = {
      Coordinate(4.5, 3.2), Coordinate(5.15, 5.2),  Coordinate(5.8, 7.2),   Coordinate(7.15, 8.5),
      Coordinate(8.5, 9.8), Coordinate(9.5, 10),    Coordinate(10.5, 11.3), Coordinate(11.5, 12.7),
      Coordinate(12.5, 14),
      Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c1, 5, "w1");

    Coordinate c2[] = {
      Coordinate(4.6, 1.6), Coordinate(4.8, 2.6),   Coordinate(3.4, 1.8),   Coordinate(3.4, 2.8),
      Coordinate(4.1, 3.8), Coordinate(5.05, 5.15), Coordinate(6, 6.5),     Coordinate(7, 7),
      Coordinate(8, 8.25),  Coordinate(9, 9.5),     Coordinate(10, 9.75),   Coordinate(11, 10.5),
      Coordinate(12, 11.9), Coordinate(13, 13.2),   Coordinate(13.2, 12.6), Coordinate(13.8, 12.6),
      Coordinate(14, 11.7),
      Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c2, 5, "w2");

    return map;
  }

  frechet_subline createTestSublinePartial()
  {
    frechet_subline optimal_subline;
    optimal_subline.push_back(vertex_match(0, 3));
    optimal_subline.push_back(vertex_match(0, 4));
    optimal_subline.push_back(vertex_match(1, 5));
    optimal_subline.push_back(vertex_match(2, 6));
    optimal_subline.push_back(vertex_match(2, 7));
    optimal_subline.push_back(vertex_match(3, 8));
    optimal_subline.push_back(vertex_match(4, 9));
    optimal_subline.push_back(vertex_match(5, 10));
    optimal_subline.push_back(vertex_match(6, 11));
    optimal_subline.push_back(vertex_match(7, 12));
    optimal_subline.push_back(vertex_match(8, 13));
    return optimal_subline;
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetDistanceTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetDistanceTest, "quick");

}

