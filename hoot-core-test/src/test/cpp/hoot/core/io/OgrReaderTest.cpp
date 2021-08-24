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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Progress.h>

using namespace geos::geom;

namespace hoot
{

class OgrReaderTest : public HootTestFixture
{
    CPPUNIT_TEST_SUITE(OgrReaderTest);
    CPPUNIT_TEST(runBasicTest);
    CPPUNIT_TEST(runBoundingBoxTest);
    CPPUNIT_TEST(runJavaScriptTranslateTest);
    CPPUNIT_TEST(runPythonTranslateTest);
    CPPUNIT_TEST(runStreamHasMoreElementsTest);
    CPPUNIT_TEST(runStreamReadNextElementTest);
    CPPUNIT_TEST(runOgrZipTest);
    CPPUNIT_TEST_SUITE_END();

public:

    OgrReaderTest() : HootTestFixture("test-files/", UNUSED_PATH)
    {
    }

    void runBasicTest()
    {
      OgrReader uut;

      OsmMapPtr map = std::make_shared<OsmMap>();
      uut.read(_inputPath + "jakarta_raya_coastline.shp", "", map);

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
        std::shared_ptr<OGRSpatialReference> ortho1 = MapProjector::createOrthographic(env);

        Settings s;
        // 15512.4m wide at the top
        // 19381.6m wide at the bottom
        // 111385.6m tall
        s.set(ConfigOptions::getBoundsKey(), "-1,85,1,86");
        s.set(ConfigOptions::getOgrReaderBoundingBoxLatlngKey(), "true");
        // resulting bounding box is
        // 19403.28m wide
        // 111385.6m tall
        HOOT_STR_EQUALS("Env[-9734.03:9734.03,-55842.4:55911]",
          uut.getBoundingBoxFromConfig(s, ortho1.get())->toString());
      }

      {
        Settings s;
        // 15512.4m wide
        // 111385.6m tall
        s.set(ConfigOptions::getBoundsKey(), "-7756.2,-55692.8,7756.2,55692.8");
        HOOT_STR_EQUALS("Env[-7756.2:7756.2,-55692.8:55692.8]",
          uut.getBoundingBoxFromConfig(s, 0)->toString());
      }
    }

    void runJavaScriptTranslateTest()
    {
      OgrReader uut;

      OsmMapPtr map = std::make_shared<OsmMap>();
      uut.setSchemaTranslationScript("translations/cloudmade.js");
      uut.read(_inputPath + "jakarta_raya_coastline.shp", "", map);

      CPPUNIT_ASSERT_EQUAL(604, (int)map->getNodes().size());
      CPPUNIT_ASSERT_EQUAL(6, (int)map->getWays().size());

      int shoreline = 0;
      int water = 0;
      for (WayMap::const_iterator it = map->getWays().begin(); it != map->getWays().end(); ++it)
      {
        WayPtr w = it->second;
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

      OsmMapPtr map = std::make_shared<OsmMap>();
      uut.setSchemaTranslationScript("cloudmade");
      uut.read(_inputPath + "jakarta_raya_coastline.shp", "", map);

      CPPUNIT_ASSERT_EQUAL(604, (int)map->getNodes().size());
      CPPUNIT_ASSERT_EQUAL(6, (int)map->getWays().size());

      int shoreline = 0;
      int water = 0;
      for (WayMap::const_iterator it = map->getWays().begin(); it != map->getWays().end(); ++it)
      {
        WayPtr w = it->second;
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

    void runStreamHasMoreElementsTest()
    {
      OgrReader reader1;

      // If we haven't opened a file, it best not be ready to read.
      CPPUNIT_ASSERT_EQUAL(reader1.hasMoreElements(), false);

      // Try to open an invalid file.
      OgrReader reader2(_inputPath + "totalgarbage.osm.pbf");
      CPPUNIT_ASSERT_EQUAL(reader2.hasMoreElements(), false);

      // Open a valid file.
      OgrReader reader3(_inputPath + "jakarta_raya_coastline.shp");
      CPPUNIT_ASSERT_EQUAL(reader3.hasMoreElements(), true);

      // Close the file and check again.
      reader3.close();
      CPPUNIT_ASSERT_EQUAL(reader3.hasMoreElements(), false);
    }

    void runStreamReadNextElementTest()
    {
      OgrReader reader(_inputPath + "jakarta_raya_coastline.shp");

      // Iterate through all items.
      int numberOfElements(0);
      while (reader.hasMoreElements() == true)
      {
        ElementPtr tempElement = reader.readNextElement();
        numberOfElements++;
        LOG_VART(tempElement->toString());
      }

      CPPUNIT_ASSERT_EQUAL(610, numberOfElements);
    }

    void runOgrZipTest()
    {
      QStringList files_extensions({".zip", ".tar", ".tar.gz"});
      QStringList files_prefixes({"/vsizip/", "/vsitar/", "/vsitar/"});

      for (int ext = 0; ext < files_extensions.length(); ++ext)
      {
        QString file = _inputPath + "MGCPv3" + files_extensions[ext];
        QStringList files = OgrUtilities::getInstance().getValidFilesInContainer(file);
        //  The MGCPv3 container files contain 9 Shapefiles
        CPPUNIT_ASSERT_EQUAL(9, files.length());
        //  Make sure all of them begin will the correct prefix
        for (int f = 0; f < files.length(); ++f)
          CPPUNIT_ASSERT(files[f].startsWith(files_prefixes[ext]));
        //  Check the files in alphabetical order
        CPPUNIT_ASSERT(files[0].endsWith("MGCPv3" + files_extensions[ext] + "/AAL015.shp"));
        CPPUNIT_ASSERT(files[1].endsWith("MGCPv3" + files_extensions[ext] + "/AAL020.shp"));
        CPPUNIT_ASSERT(files[2].endsWith("MGCPv3" + files_extensions[ext] + "/LAN010.shp"));
        CPPUNIT_ASSERT(files[3].endsWith("MGCPv3" + files_extensions[ext] + "/LAP010.shp"));
        CPPUNIT_ASSERT(files[4].endsWith("MGCPv3" + files_extensions[ext] + "/LAP030.shp"));
        CPPUNIT_ASSERT(files[5].endsWith("MGCPv3" + files_extensions[ext] + "/LAP050.shp"));
        CPPUNIT_ASSERT(files[6].endsWith("MGCPv3" + files_extensions[ext] + "/LAQ040.shp"));
        CPPUNIT_ASSERT(files[7].endsWith("MGCPv3" + files_extensions[ext] + "/LBH140.shp"));
        CPPUNIT_ASSERT(files[8].endsWith("MGCPv3" + files_extensions[ext] + "/PAL015.shp"));
      }
    }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OgrReaderTest, "quick");

}
