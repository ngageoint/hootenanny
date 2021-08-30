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
 * @copyright Copyright (C) 2015, 2017, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <iostream>
using namespace std;

// TGS
#include <tgs/DelaunayTriangulation/DelaunayTriangulation.h>
#include <tgs/TgsException.h>

namespace Tgs
{

class DelaunayTriangulationTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DelaunayTriangulationTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST(edgesTest);
  CPPUNIT_TEST(facesTest);
  CPPUNIT_TEST(findFaceTest);
  CPPUNIT_TEST_EXCEPTION(smallPointsTest, Exception);
  CPPUNIT_TEST_SUITE_END();

public:

  void edgesTest()
  {
    DelaunayTriangulation uut;
    uut.insert(0, 0);
    uut.insert(1, 0);
    uut.insert(1, 1);
    uut.insert(0.7, 0.3);
    int count = 0;
    for (EdgeIterator ei = uut.getEdgeIterator();  ei != uut.getEdgeEnd(); ++ei)
    {
      count++;
    }
    CPPUNIT_ASSERT_EQUAL(12, count);
  }

  void facesTest()
  {
    DelaunayTriangulation uut;
    uut.insert(0, 0);
    uut.insert(1, 0);
    uut.insert(1, 1);
    uut.insert(0.7, 0.3);
    int count = 0;
    for (FaceIterator fi = uut.getFaceIterator(); fi != uut.getFaceEnd(); ++fi)
    {
      count++;
    }
    CPPUNIT_ASSERT_EQUAL(4, count);
  }

  void findFaceTest()
  {
    DelaunayTriangulation uut;
    uut.insert(-10, -10);
    uut.insert(0, 10);
    uut.insert(10, -10);
    uut.insert(-1, 2);
    uut.insert(0.7, 0.3);
    Face f = uut.findContainingFace(0.5, 0.1);
    CPPUNIT_ASSERT_EQUAL(string("Face: -10,-10 0.7,0.3 10,-10"), f.toString());
  }

  void simpleTest()
  {
    DelaunayTriangulation uut;
    uut.insert(0, 0);
    uut.insert(1, 0);
    uut.insert(1, 1);
    Edge e = uut.getStartingEdge();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, e.getDestinationX(), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, e.getDestinationY(), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, e.getOriginX(), 0.01);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, e.getOriginY(), 0.01);
  }

  void smallPointsTest()
  {
    // should throw an exception when there are less than 3 points.
    DelaunayTriangulation uut;
    uut.insert(0, 0);
    uut.insert(1, 0);
    uut.getStartingEdge();
  }
};

}


//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::DelaunayTriangulationTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::DelaunayTriangulationTest, "quick");

