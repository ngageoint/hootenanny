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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/rnd/conflate/frechet/FrechetDistance.h>
#include <hoot/core/visitors/FindWaysVisitor.h>

#include <hoot/core/io/OsmXmlReader.h>

#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/MapProjector.h>

#include <hoot-core-test/src/test/cpp/hoot/core/TestUtils.h>

namespace hoot
{

class FrechetDistanceTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(FrechetDistanceTest);
  CPPUNIT_TEST(simpleFrechet);
  CPPUNIT_TEST(tailFrechet);
  CPPUNIT_TEST(homologousTest);
  CPPUNIT_TEST(partialTest);
  CPPUNIT_TEST(splitTest);
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

    CPPUNIT_ASSERT_EQUAL(optimal_subline.first, subline.first);

    CPPUNIT_ASSERT_EQUAL(optimal_subline.second.size(), subline.second.size());

    for (subline_entry::size_type i = 0; i < subline.second.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal_subline.second[i].first, subline.second[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal_subline.second[i].second, subline.second[i].second);
    }

    vector<frechet_subline> sublines = fd.matchingSublines(5);

    CPPUNIT_ASSERT_EQUAL((int)sublines.size(), 1);

    CPPUNIT_ASSERT_EQUAL(optimal_subline.first, sublines[0].first);

    CPPUNIT_ASSERT_EQUAL(optimal_subline.second.size(), sublines[0].second.size());

    for (subline_entry::size_type i = 0; i < sublines[0].second.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal_subline.second[i].first, sublines[0].second[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal_subline.second[i].second, sublines[0].second[i].second);
    }

  }

  frechet_subline createTailSubline()
  {
    subline_entry optimal_subline;
    optimal_subline.push_back(vertex_match(0, 0));
    optimal_subline.push_back(vertex_match(1, 1));
    optimal_subline.push_back(vertex_match(2, 2));
    optimal_subline.push_back(vertex_match(3, 3));
    return frechet_subline(4.0, optimal_subline);
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
    // Example taken from "A new merging process for data integration based on the discrete Fréchet distance"
    //  http://thomas.devogele.free.fr/articles/avant_2003_selection/DevogeleSDH2002.pdf
    //
    // Table 1 - Matrix of dE between (L1.i, L2.j) distance and ways
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

    CPPUNIT_ASSERT_DOUBLES_EQUAL(optimal.first, subline.first, 0.0001);

    CPPUNIT_ASSERT_EQUAL(optimal.second.size(), subline.second.size());

    for (subline_entry::size_type i = 0; i < subline.second.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal.second[i].first, subline.second[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal.second[i].second, subline.second[i].second);
    }
  }

  void partialTest()
  {
    // Example taken from "A new merging process for data integration based on the discrete Fréchet distance"
    //  http://thomas.devogele.free.fr/articles/avant_2003_selection/DevogeleSDH2002.pdf
    //
    //  Table 2 - Matrix of dE between the partial homologous lines of figure 5

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

    CPPUNIT_ASSERT_EQUAL(optimal.first, subline.first);
    CPPUNIT_ASSERT_EQUAL(optimal.second.size(), subline.second.size());

    for (subline_entry::size_type i = 0; i < subline.second.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal.second[i].first, subline.second[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal.second[i].second, subline.second[i].second);
    }

    vector<frechet_subline> matches = fd.matchingSublines(1.25);

    CPPUNIT_ASSERT_EQUAL(1, (int)matches.size());

    CPPUNIT_ASSERT_EQUAL(optimal.first, matches[0].first);
    CPPUNIT_ASSERT_EQUAL(optimal.second.size(), matches[0].second.size());

    for (subline_entry::size_type i = 0; i < matches[0].second.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(optimal.second[i].first, matches[0].second[i].first);
      CPPUNIT_ASSERT_EQUAL(optimal.second[i].second, matches[0].second[i].second);
    }
  }

  void splitTest()
  {
    const int rows = 5;
    const int cols = 6;
    Meters values[rows][cols] = {
      {  3.00, 10.44, 42.72, 20.22, 30.14, 40.11 },
      { 10.44,  3.00, 40.31, 10.44, 20.22, 30.14 },
      { 20.22, 10.44, 40.31,  3.00, 10.44, 20.22 },
      { 30.14, 20.22, 42.72, 10.44,  3.00, 10.44 },
      { 40.11, 30.14, 47.16, 20.22, 10.44,  3.00 }

    };

    OsmMapPtr map = createTestSplitMap();
    WayPtr w1 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w1")[0]);
    WayPtr w2 = map->getWay(FindWaysVisitor::findWaysByTag(map, "note", "w2")[0]);

    FrechetDistance fd(map, w1, w2);

    for (int r = 0; r < rows; r++)
      for (int c = 0; c < cols; c++)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(values[r][c], fd._matrix[r][c], 0.1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(40.00, fd.distance(), 0.01);

    vector<frechet_subline> matches = fd.matchingSublines(4);
    vector<frechet_subline> manual = createTestSplitMatches();

    CPPUNIT_ASSERT_EQUAL(manual.size(), matches.size());

    for (vector<frechet_subline>::size_type i = 0; i < matches.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(manual[i].first, matches[i].first);
      CPPUNIT_ASSERT_EQUAL(manual[i].second.size(), matches[i].second.size());

      for (subline_entry::size_type j = 0; j < matches[i].second.size(); j++)
      {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(manual[i].second[j].first, matches[i].second[j].first, 0.01);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(manual[i].second[j].second, matches[i].second[j].second, 0.01);
      }
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
    subline_entry optimal_subline;
    optimal_subline.push_back(vertex_match(0, 0));
    optimal_subline.push_back(vertex_match(1, 1));
    optimal_subline.push_back(vertex_match(2, 1));
    optimal_subline.push_back(vertex_match(3, 2));
    optimal_subline.push_back(vertex_match(4, 3));
    optimal_subline.push_back(vertex_match(4, 4));
    optimal_subline.push_back(vertex_match(4, 5));
    optimal_subline.push_back(vertex_match(5, 6));
    optimal_subline.push_back(vertex_match(6, 7));
    return frechet_subline(9.0, optimal_subline);
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
    subline_entry optimal_subline;
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
    return frechet_subline(11.0, optimal_subline);
  }

  OsmMapPtr createTestSplitMap()
  {
    OsmMapPtr map = createEmptyMap();

    Coordinate c1[] = {
      Coordinate(0.0, 0.0), Coordinate(10.0, 0.0), Coordinate(20.0, 0.0), Coordinate(30.0, 0.0), Coordinate(40.0, 0.0),
      Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c1, 3, "w1");

    Coordinate c2[] = {
      Coordinate(0.0, 3.0), Coordinate(10.0, 3.0), Coordinate(15.0, 40.0), Coordinate(20.0, 3.0), Coordinate(30.0, 3.0), Coordinate(40.0, 3.0),
      Coordinate::getNull() };
    TestUtils::createWay(map, Status::Unknown1, c2, 3, "w2");

    return map;
  }

  vector<frechet_subline> createTestSplitMatches()
  {
    vector<frechet_subline> matches;
    subline_entry optimal_subline;
    optimal_subline.push_back(vertex_match(2, 3));
    optimal_subline.push_back(vertex_match(3, 4));
    optimal_subline.push_back(vertex_match(4, 5));
    matches.push_back(frechet_subline(3.0, optimal_subline));
    subline_entry secondary_subline;
    secondary_subline.push_back(vertex_match(0, 0));
    secondary_subline.push_back(vertex_match(1, 1));
    matches.push_back(frechet_subline(2.0, secondary_subline));
    return matches;
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetDistanceTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FrechetDistanceTest, "quick");

}

