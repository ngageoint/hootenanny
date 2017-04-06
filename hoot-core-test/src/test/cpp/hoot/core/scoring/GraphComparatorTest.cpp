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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */


// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/splitter/IntersectionSplitter.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/scoring/GraphComparator.h>
#include <hoot/core/util/OpenCv.h>
using namespace hoot;

// Tgs
#include <tgs/Statistics/Random.h>

// Boost
using namespace boost;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>

// Standard
#include <stdio.h>

class GraphComparatorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(GraphComparatorTest);
    CPPUNIT_TEST(runTest);
    //CPPUNIT_TEST(runDenverDistanceTest);
    //CPPUNIT_TEST(runKentuckyDistanceTest);
    CPPUNIT_TEST_SUITE_END();

public:

    void runTest()
    {
        OsmXmlReader reader;

        Tgs::Random::instance()->seed(0);

        boost::shared_ptr<OsmMap> map(new OsmMap());
        reader.read("test-files/ToyTestA.osm", map);

        boost::shared_ptr<OsmMap> map2(new OsmMap());
        reader.read("test-files/ToyTestB.osm", map2);

        const WayMap& w1 = map->getWays();
        for (WayMap::const_iterator it = w1.begin(); it != w1.end(); ++it)
        {
          boost::shared_ptr<Way> w = map->getWay(it->second->getId());
          w->setTag("highway", "road");
        }

        const WayMap& w2 = map2->getWays();
        for (WayMap::const_iterator it = w2.begin(); it != w2.end(); ++it)
        {
          boost::shared_ptr<Way> w = map2->getWay(it->second->getId());
          w->setTag("highway", "road");
        }

        GraphComparator uut(map, map2);
        uut.setIterations(3);
        uut.setPixelSize(10);
        uut.compareMaps();
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.973744272810634, uut.getMeanScore(), 0.00001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00177888445763033, uut.getConfidenceInterval(), 0.00001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.973924616144161, uut.getMedianScore(), 0.00001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(0.00187302021970233, uut.getStandardDeviation(), 0.00001);
    }

    void runDenverDistanceTest()
    {
        OsmXmlReader reader;

        Tgs::Random::instance()->seed(0);

//        int argc = 0;
//        char* argv[] = {""};
//        QCoreApplication a(argc, argv);
        LOG_WARN("Starting...");

        boost::shared_ptr<OsmMap> map(new OsmMap());
        reader.read("/home/jason.surratt/geoeye/src/hootenanny/tmp/denver-cleanup.osm", map);
        //reader.read("/home/jason.surratt/geoeye/src/hootenanny/test-files/jakarta-easy-osm.osm", map);
        IntersectionSplitter::splitIntersections(map);

        GraphComparator uut(map, map);
        uut.setIterations(3);
        uut.setPixelSize(10);

        vector<Coordinate> v;

        MapProjector::projectToPlanar(map);

        LOG_WARN("Computing...");
        //v.push_back(Coordinate(-104.77236, 39.71378));
        // Mississippi
        //v.push_back(MapProjector::reprojectFromWgs84(Coordinate(-104.77236, 39.71378), map->getSrs()));
        // Natural History Museum
        v.push_back(MapProjector::projectFromWgs84(Coordinate(-104.94057, 39.74748), map->getProjection()));
        // Kelly
        //v.push_back(MapProjector::reprojectFromWgs84(Coordinate(-105.03129, 39.74912), map->getSrs()));
        // jakarta easy
        //v.push_back(MapProjector::reprojectFromWgs84(Coordinate(106.7838233, -6.2014298), map->getSrs()));
        //v.push_back(Coordinate(106.7838233, -6.2014298));

        uut.drawCostDistance(map, v, "tmp/denver.png");
        LOG_WARN("Done.");
    }

    void runKentuckyDistanceTest()
    {
        OsmXmlReader reader;

        Tgs::Random::instance()->seed(0);

//        int argc = 0;
//        char* argv[] = {""};
//        QCoreApplication a(argc, argv);
        LOG_WARN("Starting...");

        boost::shared_ptr<OsmMap> map(new OsmMap());
        reader.read("/home/jason.surratt/tmp/MikesHouse.osm", map);
        IntersectionSplitter::splitIntersections(map);

        GraphComparator uut(map, map);
        uut.setIterations(3);
        uut.setPixelSize(10);

        vector<Coordinate> v;

        MapProjector::projectToPlanar(map);

        LOG_WARN("Computing...");
        //v.push_back(Coordinate(-104.77236, 39.71378));
        // Mississippi
        //v.push_back(MapProjector::reprojectFromWgs84(Coordinate(-104.77236, 39.71378), map->getSrs()));
        // Natural History Museum
        v.push_back(MapProjector::projectFromWgs84(Coordinate(-85.514581,38.326101), map->getProjection()));
        // Kelly
        //v.push_back(MapProjector::reprojectFromWgs84(Coordinate(-105.03129, 39.74912), map->getSrs()));
        // jakarta easy
        //v.push_back(MapProjector::reprojectFromWgs84(Coordinate(106.7838233, -6.2014298), map->getSrs()));
        //v.push_back(Coordinate(106.7838233, -6.2014298));

        uut.drawCostDistance(map, v, "tmp/MikesHouse.png");
        LOG_WARN("Done.");
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(GraphComparatorTest);



