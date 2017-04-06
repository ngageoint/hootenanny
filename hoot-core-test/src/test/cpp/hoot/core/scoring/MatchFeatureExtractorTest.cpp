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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/polygon/BuildingMatchCreator.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/manipulators/WayMerger.h>
#include <hoot/core/scoring/MatchFeatureExtractor.h>

// Tgs
#include <tgs/Statistics/Random.h>

// Qt
#include <QDir>
#include <QFile>

#include "../TestUtils.h"

namespace hoot
{

class MatchFeatureExtractorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MatchFeatureExtractorTest);
  CPPUNIT_TEST(runBuildingTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::resetEnvironment();
  }

  boost::shared_ptr<OsmMap> load(QString s1, QString s2)
  {
    OsmXmlReader reader;

    Tgs::Random::instance()->seed(0);
    OsmMap::resetCounters();

    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(s1, map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(s2, map);

    // make the results consistent.
    MapProjector::projectToAeac(map);
    MapCleaner().apply(map);

    return map;
  }

  void runBuildingTest()
  {
    // This test is primarily useful as an input to Weka for training models.
    MatchFeatureExtractor uut;
    uut.addMatchCreator(boost::shared_ptr<MatchCreator>(new BuildingMatchCreator()));
    uut.processMap(load("test-files/conflate/extractor/BuildingsA.osm",
      "test-files/conflate/extractor/BuildingsB.osm"));

    LOG_TRACE(uut.getResults().toStdString());

    QDir().mkpath("test-output/conflate/extractor/MatchFeatureExtractorTest/");
    QFile fp("test-output/conflate/extractor/MatchFeatureExtractorTest/Buildings.arff");
    CPPUNIT_ASSERT(fp.open(QIODevice::WriteOnly | QIODevice::Text));
    QByteArray arr = uut.getResults().toUtf8();
    fp.write(arr);
    fp.close();

    // check for consistency with previous versions.
    HOOT_FILE_EQUALS(
      "test-files/conflate/extractor/MatchFeatureExtractorTest/Buildings.arff",
      "test-output/conflate/extractor/MatchFeatureExtractorTest/Buildings.arff");
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MatchFeatureExtractorTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ManipulatorFeatureExtractorTest, "current");

}

