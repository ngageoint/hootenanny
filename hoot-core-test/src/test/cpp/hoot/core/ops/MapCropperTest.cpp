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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/MapReprojector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/ObjectInputStream.h>
#include <hoot/core/io/ObjectOutputStream.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// geos
#include <geos/io/WKTReader.h>
#include <geos/geom/Point.h>

// Qt
#include <QDebug>

// TGS
#include <tgs/Statistics/Random.h>

#include "../TestUtils.h"

namespace hoot
{
using namespace Tgs;

class MapCropperTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MapCropperTest);
  CPPUNIT_TEST(runGeometryTest);
  CPPUNIT_TEST(runSerializationTest);
  CPPUNIT_TEST(runConfigurationTest);
  CPPUNIT_TEST_SUITE_END();

public:

  shared_ptr<OsmMap> genPoints(int seed)
  {
    srand(seed);
    shared_ptr<OsmMap> result(new OsmMap());

    for (int i = 0; i < 1000; i++)
    {
      double x = Random::generateUniform() * 360 - 180;
      double y = Random::generateUniform() * 180 - 90;

      shared_ptr<Node> n(new Node(Status::Invalid, result->createNextNodeId(), x, y, 10));
      result->addNode(n);
    }

    return result;
  }

  void runGeometryTest()
  {
    shared_ptr<OsmMap> map = genPoints(0);

    shared_ptr<Geometry> g(geos::io::WKTReader().read(
      "POLYGON ((-50 0, 0 50, 50 0, 0 -50, 0 0, -50 0))"));

    int insideCount = 0;
    const NodeMap& nm = map->getNodeMap();
    for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
    {
      Coordinate c = it->second->toCoordinate();
      auto_ptr<Point> p(GeometryFactory::getDefaultInstance()->createPoint(c));
      if (g->intersects(p.get()))
      {
        insideCount++;
      }
    }

    {
      MapCropper uut(g, false);
      uut.apply(map);

      CPPUNIT_ASSERT_EQUAL(insideCount, (int)map->getNodeMap().size());
    }

    {
      shared_ptr<OsmMap> map = genPoints(0);

      MapCropper uut(g, true);
      uut.apply(map);
      CPPUNIT_ASSERT_EQUAL(1000 - insideCount, (int)map->getNodeMap().size());
    }
  }

  void runSerializationTest()
  {
    shared_ptr<Geometry> g(geos::io::WKTReader().read(
      "POLYGON ((-50 0, 0 50, 50 0, 0 -50, 0 0, -50 0))"));

    MapCropper pre(g, false);
    shared_ptr<OsmMap> mapPre = genPoints(0);
    pre.apply(mapPre);

    stringstream ss;
    {
      ObjectOutputStream oos(ss);
      oos.writeObject(pre);
    }

    stringstream ss2(ss.str());
    ObjectInputStream ois(ss2);
    auto_ptr<OsmMapOperation> post(ois.readObject<OsmMapOperation>());
    shared_ptr<OsmMap> mapPost = genPoints(0);
    post->apply(mapPost);

    // do we get the same result before/after serialization.
    CPPUNIT_ASSERT_EQUAL(mapPre->getNodeMap().size(), mapPost->getNodeMap().size());
  }

  void runConfigurationTest()
  {
    MapCropper cropper;
    Settings settings = conf();

    settings.set("crop.bounds", "12.462,41.891,12.477,41.898");
    cropper.setConfiguration(settings);
    HOOT_STR_EQUALS("Env[12.462:12.477,41.891:41.898]", cropper._envelope.toString());

    settings.clear();
    settings.set("crop.bounds", "-12.462,41.891,12.477,41.898");
    cropper.setConfiguration(settings);
    HOOT_STR_EQUALS("Env[-12.462:12.477,41.891:41.898]", cropper._envelope.toString());

    settings.clear();
    settings.set("crop.bounds", "12,41.891,13,41.898");
    cropper.setConfiguration(settings);
    HOOT_STR_EQUALS("Env[12:13,41.891:41.898]", cropper._envelope.toString());

    settings.clear();
    settings.set("crop.bounds", "12,41.891,13.,42");
    cropper.setConfiguration(settings);
    HOOT_STR_EQUALS("Env[12:13,41.891:42]", cropper._envelope.toString());

    cropper._envelope = Envelope();

    settings.clear();
    settings.set("crop.bounds", "12.462,41.891,-12.477,41.898");
    QString exceptionMsg("");
    try
    {
      cropper.setConfiguration(settings);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid bounds passed to map cropper"));
    HOOT_STR_EQUALS("Env[0:-1,0:-1]", cropper._envelope.toString());

    settings.clear();
    settings.set("crop.bounds", "12.462,41.891,12.477,-41.898");
    exceptionMsg = "";
    try
    {
      cropper.setConfiguration(settings);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid bounds passed to map cropper"));
    HOOT_STR_EQUALS("Env[0:-1,0:-1]", cropper._envelope.toString());

    settings.clear();
    settings.set("crop.bounds", "41.891,12.477,41.898");
    exceptionMsg = "";
    try
    {
      cropper.setConfiguration(settings);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid bounds passed to map cropper"));
    HOOT_STR_EQUALS("Env[0:-1,0:-1]", cropper._envelope.toString());

    settings.clear();
    settings.set("crop.bounds", "12.462,asdf,12.477,41.898");
    exceptionMsg = "";
    try
    {
      cropper.setConfiguration(settings);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid bounds passed to map cropper"));
    HOOT_STR_EQUALS("Env[0:-1,0:-1]", cropper._envelope.toString());

    settings.clear();
    settings.set("crop.bounds", "12.462,,12.477,41.898");
    exceptionMsg = "";
    try
    {
      cropper.setConfiguration(settings);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid bounds passed to map cropper"));
    HOOT_STR_EQUALS("Env[0:-1,0:-1]", cropper._envelope.toString());
  }

};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCropperTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCropperTest, "current");


