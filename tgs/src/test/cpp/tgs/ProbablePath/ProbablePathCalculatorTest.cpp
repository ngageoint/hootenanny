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
 * @copyright Copyright (C) 2015, 2017, 2018, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Standard
#include <limits>
#include <vector>
using namespace std;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

//  TGS
#include "../PluginFactory.h"
#include <tgs/ProbablePath/ProbablePathCalculator.h>

namespace Tgs
{

class ProbablePathCalculatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ProbablePathCalculatorTest);
  CPPUNIT_TEST( test1 );
  CPPUNIT_TEST( testImpassible );
  CPPUNIT_TEST( testUpdateCostSurface );
  CPPUNIT_TEST_SUITE_END();

public:

  void test1()
  {
    ProbablePathCalculator uut;

    vector<float> friction;
    int w = 50, h = 50;
    friction.reserve(w * h);
    for (int row = 0; row < h; row++)
    {
      for (int col = 0; col < w; col++)
      {
        friction.push_back(1);
      }
    }

    uut.setFriction(w, h, friction);

    std::vector<PpPoint> source;
    // lower left
    source.push_back(PpPoint(2, 40));
    std::vector<PpPoint> destination;
    // upper right
    destination.push_back(PpPoint(14, 34));
    destination.push_back(PpPoint(2, 12));
    uut.setSources(source);
    uut.setDestinations(destination);
    std::vector<int> result;
    uut.setRandomNoise(0.0);
    uut.calculateProbablePaths(1);
    uut.getRouteCounts();
    vector<float> costSurface = uut.getCostSurface();

    uut.setRandomPatches(0.1f, 10);
    uut.calculateProbablePaths(5);
    result = uut.getRouteCounts();

    CPPUNIT_ASSERT_EQUAL(friction.size(), result.size());

    int rMax = std::numeric_limits<int>::min(), rMin = std::numeric_limits<int>::max();
    float fMax = -1e10, fMin = 1e10;
    float cMax = -1e10, cMin = 1e10;
    for (unsigned int i = 0; i < result.size(); i++)
    {
      rMax = std::max(rMax, result[i]);
      rMin = std::min(rMin, result[i]);
      fMax = std::max(fMax, friction[i]);
      fMin = std::min(fMin, friction[i]);
      cMax = std::max(cMax, costSurface[i]);
      cMin = std::min(cMin, costSurface[i]);
    }
//     qDebug() << fMin << fMax;
//     qDebug() << rMin << rMax;
//     QImage img(loader.getWidth(), loader.getHeight(), QImage::Format_RGB32);
// 
//     int* bits = (int*)img.bits();
//     for (unsigned int i = 0; i < result.size(); i++)
//     {
//       int r = std::min((int)(result[i] / rMax * 255.0), 255);
//       int f = 255 - friction[i] / fMax * 255.0;
//       int c = std::min((int)(costSurface[i] / cMax * 255.0), 255);
// 
//       if (result[i] < 0)
//       {
//         bits[i] = qRgb(0, 255, 0);
//       }
//       else if (friction[i] < 0)
//       {
//         bits[i] = qRgb(255, 0, 0);
//       }
//       else
//       {
//         int rr = 0;
//         if (r > 0)
//         {
//           rr = 100;
//         }
//         bits[i] = qRgb(rr, r, c);
//       }
//     }
//     img.save("ProbablePathCalculatorTest.png", "PNG");
  }

  void testImpassible()
  {
    int w = 5, h = 5;
    vector<float> friction;
    friction.resize(w * h, 1);
    // make the four pixels in the upper left impassible.
    //friction[0 + 0 * w] = -1;
    friction[1 + 0 * w] = -1;
    friction[0 + 1 * w] = -1;
    friction[1 + 1 * w] = -1;

    ProbablePathCalculator uut;
    uut.setFriction(w, h, friction);

    std::vector<PpPoint> source;
    // lower left
    source.push_back(PpPoint(0, 0));
    std::vector<PpPoint> destination;
    // upper right
    destination.push_back(PpPoint(4, 4));
    uut.setSources(source);
    uut.setDestinations(destination);
    std::vector<int> result;
    uut.setRandomNoise(0.0);
    uut.calculateProbablePaths(1);
    uut.getRouteCounts();
    uut.calculateProbablePaths(1);
    uut.getRouteCounts();
  }

  void testUpdateCostSurface()
  {
    int w = 5, h = 5;
    vector<float> friction;
    friction.resize(w * h, 1);
    Image<float> cost;
    cost.resize(w, h, -1);
    cost.pixel(1, 0) = 3;

    std::string tmp = "[[4, 3, 4, 5, 6]\n"
      "[4.41421, 4, 4.41421, 5.41421, 6.41421]\n"
      "[5.41421, 5, 5.41421, 5.82843, 6.82843]\n"
      "[6.41421, 6, 6.41421, 6.82843, 7.24264]\n"
      "[7.41421, 7, 7.41421, 7.82843, 8.24264]\n"
      "]";

    {
      ProbablePathCalculator uut;
      uut.setFriction(w, h, friction);
      uut.setRandomNoise(0.0);
      cost = uut.updateCostSurface(cost);
      stringstream strm;
      strm << cost;
      CPPUNIT_ASSERT_EQUAL(tmp, strm.str());
    }

    {
      cost.resize(w, h, 10);
      cost.pixel(1, 0) = 3;

      ProbablePathCalculator uut;
      uut.setFriction(w, h, friction);
      uut.setRandomNoise(0.0);
      uut.updateCostSurface(cost);
      stringstream strm;
      strm << cost;
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ProbablePathCalculatorTest, PluginFactory::testName());

}
