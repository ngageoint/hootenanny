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
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/Log.h>
using namespace hoot;


// Boost
using namespace boost;

// Qt
#include <QDebug>

#include "../TestUtils.h"

namespace hoot
{

class OsmXmlReaderTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(OsmXmlReaderTest);
    CPPUNIT_TEST(runTest);
    CPPUNIT_TEST(runUseIdTest);
    CPPUNIT_TEST(runUseStatusTest);
    CPPUNIT_TEST(runUncompressTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runTest()
    {
        OsmXmlReader uut;

        OsmMapPtr map(new OsmMap());
        uut.read("test-files/ToyTestA.osm", map);

        CPPUNIT_ASSERT_EQUAL(36, (int)map->getNodes().size());
        CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());
    }

    void runUseIdTest()
    {
        OsmXmlReader uut;

        OsmMap::resetCounters();

        OsmMapPtr map(new OsmMap());
        uut.setUseDataSourceIds(true);
        uut.read("test-files/ToyTestA.osm", map);

        CPPUNIT_ASSERT_EQUAL(36,(int)map->getNodes().size());
        CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());

        long min = 1e9;
        long max = -1e9;
        for (NodeMap::const_iterator it = map->getNodes().begin();
             it != map->getNodes().end(); ++it)
        {
          const ConstNodePtr& n = it->second;
          min = std::min(min, n->getId());
          max = std::max(max, n->getId());
        }

        CPPUNIT_ASSERT_EQUAL(-1669793l, min);
        CPPUNIT_ASSERT_EQUAL(-1669723l, max);

        CPPUNIT_ASSERT(map->containsWay(-1669801));
        CPPUNIT_ASSERT(map->containsWay(-1669799));
        CPPUNIT_ASSERT(map->containsWay(-1669797));
        CPPUNIT_ASSERT(map->containsWay(-1669795));
    }

    void runUseStatusTest()
    {
        OsmXmlReader uut;

        OsmMap::resetCounters();

        OsmMapPtr map(new OsmMap());
        uut.setUseDataSourceIds(true);
        uut.setUseStatusFromFile(true);
        uut.setDefaultStatus(Status::Invalid);
        uut.read("test-files/io/OsmXmlReaderUseStatusTest.osm", map);

        CPPUNIT_ASSERT_EQUAL(104, (int)map->getNodes().size());
        CPPUNIT_ASSERT_EQUAL(17, (int)map->getWays().size());

        HOOT_STR_EQUALS(Status::Unknown1, map->getWay(-12)->getStatus().getEnum());
        HOOT_STR_EQUALS(Status::Conflated, map->getWay(-13)->getStatus().getEnum());
        HOOT_STR_EQUALS(Status::Unknown2, map->getWay(-51)->getStatus().getEnum());
        HOOT_STR_EQUALS(Status::Conflated + 1, map->getWay(-14)->getStatus().getEnum());
        HOOT_STR_EQUALS(Status::Conflated + 2, map->getWay(-15)->getStatus().getEnum());

        HOOT_STR_EQUALS(0, map->getWay(-12)->getStatus().getInput());
        HOOT_STR_EQUALS(1, map->getWay(-51)->getStatus().getInput());
        HOOT_STR_EQUALS(2, map->getWay(-14)->getStatus().getInput());
        HOOT_STR_EQUALS(3, map->getWay(-15)->getStatus().getInput());

        HOOT_STR_EQUALS("Unknown1", map->getWay(-12)->getStatus().toString());
        HOOT_STR_EQUALS("Unknown2", map->getWay(-51)->getStatus().toString());
        HOOT_STR_EQUALS("Input003", map->getWay(-14)->getStatus().toString());
        HOOT_STR_EQUALS("Input004", map->getWay(-15)->getStatus().toString());
    }

    void runUncompressTest()
    {
      const std::string cmd("gzip -c test-files/ToyTestA.osm > test-output/ToyTestA_compressed.osm.gz");
      LOG_DEBUG("Running compress command: " << cmd);

      int retVal;
      if ((retVal = std::system(cmd.c_str())) != 0)
      {
        QString error = QString("Error %1 returned from compress command: %2").arg(retVal).
          arg(QString::fromStdString(cmd));
        throw HootException(error);
      }

      OsmXmlReader uut;
      OsmMap::resetCounters();
      OsmMapPtr map(new OsmMap());
      uut.setUseDataSourceIds(true);

      // Excercise the code
      uut.read("test-output/ToyTestA_compressed.osm.gz", map);

      // Checka a few things
      CPPUNIT_ASSERT_EQUAL(36,(int)map->getNodes().size());
      CPPUNIT_ASSERT_EQUAL(4, (int)map->getWays().size());

      QFile f("test-output/ToyTestA_compressed.osm.gz");
      CPPUNIT_ASSERT(f.exists());
      CPPUNIT_ASSERT(f.remove());
    }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmXmlReaderTest, "quick");

}
