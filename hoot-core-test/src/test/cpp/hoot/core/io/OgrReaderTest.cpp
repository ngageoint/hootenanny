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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Progress.h>
using namespace hoot;


// Boost
using namespace boost;

#include "../TestUtils.h"

// Qt
#include <QDebug>

namespace hoot
{

class OgrReaderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(OgrReaderTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST(runBoundingBoxTest);
    CPPUNIT_TEST(runJavaScriptTranslateTest);
    CPPUNIT_TEST(runPythonTranslateTest);
    CPPUNIT_TEST(runStreamHasMoreElementsTest);
    CPPUNIT_TEST(runStreamReadNextElementTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runBasicTest()
    {
      OgrReader uut;

      Progress progress("runBasicTest");
      boost::shared_ptr<OsmMap> map(new OsmMap());
      uut.read("test-files/jakarta_raya_coastline.shp", "", map, progress);

      CPPUNIT_ASSERT_EQUAL(604, (int)map->getNodes().size());
      CPPUNIT_ASSERT_EQUAL(6, (int)map->getWays().size());
    }

    void runBoundingBoxTest()
    {
      OgrReader uut;

      OGREnvelope env;
      env.MinX = -1;
      env.MaxX = 1;
      env.MinY = 85;
      env.MaxY = 86;

      Coordinate c1(-1, 84);
      Coordinate c2(1, 84);

      {
        boost::shared_ptr<OGRSpatialReference> ortho1 = MapProjector::createOrthographic(env);

        Settings s;
        // 15512.4m wide at the top
        // 19381.6m wide at the bottom
        // 111385.6m tall
        s.set(ConfigOptions::getOgrReaderBoundingBoxLatlngKey(), "-1,85,1,86");
        // resulting bounding box is
        // 19403.28m wide
        // 111385.6m tall
        HOOT_STR_EQUALS("Env[-9701.64:9701.64,-55659:55726.6]",
          uut.getBoundingBoxFromConfig(s, ortho1.get())->toString());
      }

      {
        Settings s;
        // 15512.4m wide
        // 111385.6m tall
        s.set(ConfigOptions::getOgrReaderBoundingBoxKey(), "-7756.2,-55692.8,7756.2,55692.8");
        HOOT_STR_EQUALS("Env[-7756.2:7756.2,-55692.8:55692.8]",
          uut.getBoundingBoxFromConfig(s, 0)->toString());
      }
    }

    void runJavaScriptTranslateTest()
    {
      OgrReader uut;

      Progress progress("runBasicTest");
      boost::shared_ptr<OsmMap> map(new OsmMap());
      uut.setTranslationFile("translations/cloudmade.js");
      uut.read("test-files/jakarta_raya_coastline.shp", "", map, progress);

      CPPUNIT_ASSERT_EQUAL(604, (int)map->getNodes().size());
      CPPUNIT_ASSERT_EQUAL(6, (int)map->getWays().size());

      int shoreline = 0;
      int water = 0;
      for (WayMap::const_iterator it = map->getWays().begin(); it != map->getWays().end(); ++it)
      {
        boost::shared_ptr<Way> w = it->second;
        if (w->getTags()["natural"] == "shoreline")
        {
          shoreline++;
        }
        if (w->getTags()["natural"] == "water")
        {
          water++;
        }
      }
      CPPUNIT_ASSERT_EQUAL(5, shoreline);
      CPPUNIT_ASSERT_EQUAL(1, water);
    }

    void runPythonTranslateTest()
    {
      OgrReader uut;

      Progress progress("runBasicTest");
      boost::shared_ptr<OsmMap> map(new OsmMap());
      uut.setTranslationFile("cloudmade");
      uut.read("test-files/jakarta_raya_coastline.shp", "", map, progress);

      CPPUNIT_ASSERT_EQUAL(604, (int)map->getNodes().size());
      CPPUNIT_ASSERT_EQUAL(6, (int)map->getWays().size());

      int shoreline = 0;
      int water = 0;
      for (WayMap::const_iterator it = map->getWays().begin(); it != map->getWays().end(); ++it)
      {
        boost::shared_ptr<Way> w = it->second;
        if (w->getTags()["natural"] == "shoreline")
        {
          shoreline++;
        }
        if (w->getTags()["natural"] == "water")
        {
          water++;
        }
      }
      CPPUNIT_ASSERT_EQUAL(5, shoreline);
      CPPUNIT_ASSERT_EQUAL(1, water);
    }

    void runStreamHasMoreElementsTest(
        void )
    {
      OgrReader reader1;

      // If we haven't opened a file, it best not be ready to read
      CPPUNIT_ASSERT_EQUAL(reader1.hasMoreElements(), false);

      // Try to open invalid file
      OgrReader reader2(QString("test-files/totalgarbage.osm.pbf"));
      CPPUNIT_ASSERT_EQUAL(reader2.hasMoreElements(), false);

      // Open valid file
      OgrReader reader3(QString("test-files/jakarta_raya_coastline.shp"));
      CPPUNIT_ASSERT_EQUAL(reader3.hasMoreElements(), true);

      // Close file and check again
      reader3.close();
      CPPUNIT_ASSERT_EQUAL(reader3.hasMoreElements(), false);
    }

    void runStreamReadNextElementTest(
        void )
    {
      OgrReader reader(QString("test-files/jakarta_raya_coastline.shp"));

      // Iterate through all items
      int numberOfElements(0);
      while ( reader.hasMoreElements() == true )
      {
        ElementPtr tempElement = reader.readNextElement();
        numberOfElements++;
        LOG_VART(tempElement->toString());
      }

      CPPUNIT_ASSERT_EQUAL(610, numberOfElements);
    }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OgrReaderTest, "quick");

}
