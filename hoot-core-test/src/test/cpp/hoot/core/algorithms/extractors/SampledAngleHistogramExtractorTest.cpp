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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/extractors/SampledAngleHistogramExtractor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class SampledAngleHistogramExtractorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(SampledAngleHistogramExtractorTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  SampledAngleHistogramExtractorTest()
    : HootTestFixture("test-files/algorithms/extractors/SampledAngleHistogramExtractorTest/",
                      UNUSED_PATH)
  {
    setResetType(ResetBasic);
  }

  void runTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(_inputPath + "Haiti_CNIGS_Rivers_REF1-cropped.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(_inputPath + "Haiti_osm_waterway_ss_REF2-cropped.osm", map);
    MapProjector::projectToPlanar(map);

    SampledAngleHistogramExtractor angleHistogramExtractor;
    angleHistogramExtractor.setHeadingDelta(ConfigOptions().getWayMatcherHeadingDelta());
    angleHistogramExtractor.setSampleDistance(ConfigOptions().getWayAngleSampleDistance());
    CPPUNIT_ASSERT_DOUBLES_EQUAL(
      0.040583,
      angleHistogramExtractor.extract(
        *map,
        map->getWay(
          ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref1(), "001f4b")[0]),
        map->getWay(
          ElementIdsVisitor::findElementsByTag(map, ElementType::Way, MetadataTags::Ref2(), "001f4b")[0])),
      1e-6);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SampledAngleHistogramExtractorTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SampledAngleHistogramExtractorTest, "quick");

}
