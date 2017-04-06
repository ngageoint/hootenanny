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
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/ObjectInputStream.h>
#include <hoot/core/io/ObjectOutputStream.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/ElementConverter.h>
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
#include <geos/geom/Envelope.h>
#include <geos/geom/Polygon.h>

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
  CPPUNIT_TEST(runMultiPolygonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  boost::shared_ptr<OsmMap> genPoints(int seed)
  {
    Tgs::Random::instance()->seed(seed);
    boost::shared_ptr<OsmMap> result(new OsmMap());

    for (int i = 0; i < 1000; i++)
    {
      double x = Random::instance()->generateUniform() * 360 - 180;
      double y = Random::instance()->generateUniform() * 180 - 90;

      boost::shared_ptr<Node> n(new Node(Status::Invalid, result->createNextNodeId(), x, y, 10));
      result->addNode(n);
    }

    return result;
  }

  void runGeometryTest()
  {
    boost::shared_ptr<OsmMap> map = genPoints(0);

    boost::shared_ptr<Geometry> g(geos::io::WKTReader().read(
      "POLYGON ((-50 0, 0 50, 50 0, 0 -50, 0 0, -50 0))"));

    int insideCount = 0;
    const NodeMap& nm = map->getNodes();
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

      CPPUNIT_ASSERT_EQUAL(insideCount, (int)map->getNodes().size());
    }

    {
      boost::shared_ptr<OsmMap> map = genPoints(0);

      MapCropper uut(g, true);
      uut.apply(map);
      CPPUNIT_ASSERT_EQUAL(1000 - insideCount, (int)map->getNodes().size());
    }
  }

  void runSerializationTest()
  {
    boost::shared_ptr<Geometry> g(geos::io::WKTReader().read(
      "POLYGON ((-50 0, 0 50, 50 0, 0 -50, 0 0, -50 0))"));

    MapCropper pre(g, false);
    boost::shared_ptr<OsmMap> mapPre = genPoints(0);
    pre.apply(mapPre);

    stringstream ss;
    {
      ObjectOutputStream oos(ss);
      oos.writeObject(pre);
    }

    stringstream ss2(ss.str());
    ObjectInputStream ois(ss2);
    auto_ptr<OsmMapOperation> post(ois.readObject<OsmMapOperation>());
    boost::shared_ptr<OsmMap> mapPost = genPoints(0);
    post->apply(mapPost);

    // do we get the same result before/after serialization.
    CPPUNIT_ASSERT_EQUAL(mapPre->getNodes().size(), mapPost->getNodes().size());
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

  void runMultiPolygonTest()
  {
    boost::shared_ptr<OsmMap> map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/MultipolygonTest.osm",true);

    Envelope env(0.30127,0.345,0.213,0.28154);

    MapCropper::crop(map, env);

    //compare relations
    const RelationMap relations = map->getRelations();
    HOOT_STR_EQUALS(1, relations.size());
    QString relationStr = "relation(-1592); type: multipolygon; members:   Entry: role: outer, eid: Way:-1556;   Entry: role: inner, eid: Way:-1552; ; tags: landuse = farmland; status: invalid; version: 0; visible: 1; circular error: 15";
    for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); it++)
    {
      const boost::shared_ptr<Relation>& r = it->second;
      HOOT_STR_EQUALS(relationStr, r->toString().replace("\n","; "));
    }

    //compare ways
    int count = 0;
    const WayMap ways = map->getWays();
    HOOT_STR_EQUALS(2, ways.size());
    for (WayMap::const_iterator it = ways.begin(); it != ways.end(); it++)
    {
      const boost::shared_ptr<Way>& w = it->second;
      boost::shared_ptr<Polygon> pl = ElementConverter(map).convertToPolygon(w);
      const Envelope& e = *(pl->getEnvelopeInternal());
      double area = pl->getArea();
      if (count == 0)
      {
        HOOT_STR_EQUALS("Env[0.303878:0.336159,0.220255:0.270199]", e.toString());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00150737, area, 0.00001);
      }
      else
      {
        HOOT_STR_EQUALS("Env[0.314996:0.328946,0.231514:0.263127]", e.toString());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.000401258, area, 0.00001);
      }
      count++;
    }
  }

};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCropperTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MapCropperTest, "current");


