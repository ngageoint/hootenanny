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
#include <hoot/core/MapProjector.h>
#include <hoot/core/conflate/SearchRadiusCalculator.h>
#include <hoot/core/io/OsmReader.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/conflate/MapCleaner.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QBuffer>
#include <QByteArray>

#include "../TestUtils.h"

namespace hoot
{

class SearchRadiusCalculatorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SearchRadiusCalculatorTest);
  CPPUNIT_TEST(runCalcResultTest);
  CPPUNIT_TEST(runBadPreOpTest);
  CPPUNIT_TEST(runNotEnoughTiePointsTest);
  CPPUNIT_TEST(runPreviouslyConflatedDataTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runCalcResultTest()
  {
    OsmReader reader;
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(
      "test-files/conflate/SearchRadiusCalculatorTest/Haiti_CNIGS_Rivers_REF1-cropped-2.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(
      "test-files/conflate/SearchRadiusCalculatorTest/Haiti_osm_waterway_ss_REF2-cropped-2.osm", map);

    MapCleaner().apply(map);

    Settings testSettings = conf();
    testSettings.set("rubber.sheet.ref", "true");
    testSettings.set("rubber.sheet.minimum.ties", "5");
    SearchRadiusCalculator searchRadiusCalculator;
    searchRadiusCalculator.setConfiguration(testSettings);

    searchRadiusCalculator.apply(map);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(34.334710, any_cast<double>(searchRadiusCalculator.getResult()), 1e-6);
  }

  void runBadPreOpTest()
  {
    Settings testSettings = conf();
    testSettings.set("unify.pre.ops", "hoot::SetTagVisitor;hoot::RubberSheet");
    SearchRadiusCalculator searchRadiusCalculator;

    QString exceptionMsg("");
    try
    {
      //The existence of the rubber sheet op having been previously applied is the first thing
      //checked for, so it doesn't matter that we're passing an empty map in here.
      searchRadiusCalculator.setConfiguration(testSettings);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("Rubber sheeting cannot be used when automatically calculating search radius.").toStdString(),
      exceptionMsg.toStdString());
  }

  void runNotEnoughTiePointsTest()
  {
    DisableLog dl;

    vector<double> tiePointDistances;
    tiePointDistances.push_back(11.0);

    Settings testSettings = conf();
    testSettings.set("rubber.sheet.ref", "true");
    testSettings.set("rubber.sheet.minimum.ties", "5");
    SearchRadiusCalculator searchRadiusCalculator;
    searchRadiusCalculator.setConfiguration(testSettings);

    searchRadiusCalculator._calculateSearchRadius(tiePointDistances);
    //if the search radius calculator can't find enough tie points, it will use the specified
    //default search radius
    CPPUNIT_ASSERT_EQUAL(
      ConfigOptions().getCircularErrorDefaultValue(), searchRadiusCalculator._result);
  }

  void runPreviouslyConflatedDataTest()
  {
    OsmReader reader;
    OsmMap::resetCounters();
    shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read(
      "test-files/conflate/SearchRadiusCalculatorTest/Haiti_CNIGS_Rivers_REF1-cropped-2.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read(
      "test-files/conflate/SearchRadiusCalculatorTest/Haiti_osm_waterway_ss_REF2-cropped-2.osm", map);

    MapCleaner().apply(map);

    //If any data in the dataset has already been conflated (or is invalid), the operation
    //shouldn't fail.  The data should just be skipped.
    map->getWay(map->findWays("REF1", "001952")[0])->setStatus(Status::Conflated);
    map->getWay(map->findWays("REF2", "001f4b")[0])->setStatus(Status::Invalid);

    Settings testSettings = conf();
    testSettings.set("rubber.sheet.ref", "true");
    testSettings.set("rubber.sheet.minimum.ties", "5");
    SearchRadiusCalculator searchRadiusCalculator;
    searchRadiusCalculator.setConfiguration(testSettings);

    searchRadiusCalculator.apply(map);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(32.675054, any_cast<double>(searchRadiusCalculator.getResult()), 1e-6);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SearchRadiusCalculatorTest, "quick");

}
