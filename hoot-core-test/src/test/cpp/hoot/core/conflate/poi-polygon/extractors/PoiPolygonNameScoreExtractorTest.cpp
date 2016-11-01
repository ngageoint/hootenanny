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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/visitors/FindWaysVisitor.h>
#include <hoot/core/conflate/poi-polygon/extractors/PoiPolygonNameScoreExtractor.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

#include "../../../TestUtils.h"

namespace hoot
{

class PoiPolygonNameScoreExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PoiPolygonNameScoreExtractorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    /*OsmReader reader;
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(
      "test-files/conflate/extractor/SampledAngleHistogramExtractorTest/Haiti_CNIGS_Rivers_REF1-cropped.osm",
      map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(
      "test-files/conflate/extractor/SampledAngleHistogramExtractorTest/Haiti_osm_waterway_ss_REF2-cropped.osm",
      map);
    MapProjector::projectToPlanar(map);

    SampledAngleHistogramExtractor angleHistogramExtractor;
    angleHistogramExtractor.setHeadingDelta(ConfigOptions().getWayMatcherHeadingDelta());
    angleHistogramExtractor.setSampleDistance(ConfigOptions().getWayAngleSampleDistance());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.040583,
      angleHistogramExtractor.extract(
        *map,
        map->getWay(FindWaysVisitor::findWaysByTag(map, "REF1", "001f4b")[0]),
        map->getWay(FindWaysVisitor::findWaysByTag(map, "REF2", "001f4b")[0])),
      1e-6)*/;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PoiPolygonNameScoreExtractorTest, "quick");

}
