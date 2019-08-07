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
 * @copyright Copyright (C) 2012, 2013, 2014, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/algorithms/changeset/ChangesetReplacementCreator.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>

namespace hoot
{

class ChangesetReplacementCreatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ChangesetReplacementCreatorTest);
  // We're already testing API DB inputs with command tests, so skipping those here.
//  CPPUNIT_TEST(runPolyLenientOsmTest);
//  CPPUNIT_TEST(runPolyStrictOsmTest);
//  CPPUNIT_TEST(runPoiStrictOsmTest);
//  CPPUNIT_TEST(runLinearLenientOsmTest);
//  CPPUNIT_TEST(runLinearStrictOsmTest);
//  CPPUNIT_TEST(runPolyLenientJsonTest);
//  CPPUNIT_TEST(runPolyStrictJsonTest);
//  CPPUNIT_TEST(runPoiStrictJsonTest);
//  CPPUNIT_TEST(runLinearLenientJsonTest);
//  CPPUNIT_TEST(runLinearStrictJsonTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ChangesetReplacementCreatorTest() :
  HootTestFixture(
    "test-files/algorithms/changeset/ChangesetReplacementCreatorTest/",
    "test-output/algorithms/changeset/ChangesetReplacementCreatorTest/"),
  _bounds(geos::geom::Envelope(1, 1, 1, 1))
  {
  }

  void runPolyLenientOsmTest()
  { 
    const QString testName = "runPolyLenientOsmTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.osm", inFileNameBase + "-in-sec.osm", _bounds,
      QString::fromStdString(BuildingCriterion::className()), true, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runPolyStrictOsmTest()
  {
    const QString testName = "runPolyStrictOsmTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.osm", inFileNameBase + "-in-sec.osm", _bounds,
      QString::fromStdString(BuildingCriterion::className()), false, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runPoiStrictOsmTest()
  {
    const QString testName = "runPoiStrictOsmTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.osm", inFileNameBase + "-in-sec.osm", _bounds,
      QString::fromStdString(PoiCriterion::className()), false, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runLinearLenientOsmTest()
  {
    const QString testName = "runLinearLenientOsmTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.osm", inFileNameBase + "-in-sec.osm", _bounds,
      QString::fromStdString(HighwayCriterion::className()), true, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runLinearStrictOsmTest()
  {
    const QString testName = "runLinearStrictOsmTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.osm", inFileNameBase + "-in-sec.osm", _bounds,
      QString::fromStdString(HighwayCriterion::className()), false, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runPolyLenientJsonTest()
  {
    const QString testName = "runPolyLenientJsonTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.json", inFileNameBase + "-in-sec.json", _bounds,
      QString::fromStdString(BuildingCriterion::className()), true, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runPolyStrictJsonTest()
  {
    const QString testName = "runPolyStrictJsonTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.json", inFileNameBase + "-in-sec.json", _bounds,
      QString::fromStdString(BuildingCriterion::className()), false, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runPoiStrictJsonTest()
  {
    const QString testName = "runPoiStrictJsonTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.json", inFileNameBase + "-in-sec.json", _bounds,
      QString::fromStdString(PoiCriterion::className()), false, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runLinearLenientJsonTest()
  {
    const QString testName = "runLinearLenientJsonTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.json", inFileNameBase + "-in-sec.json", _bounds,
      QString::fromStdString(HighwayCriterion::className()), true, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

  void runLinearStrictJsonTest()
  {
    const QString testName = "runLinearStrictJsonTest";
    const QString inFileNameBase = _inputPath + testName;
    const QString outFile = _outputPath + testName + "-out.osc";

    ChangesetReplacementCreator().create(
      inFileNameBase + "-in-ref.json", inFileNameBase + "-in-sec.json", _bounds,
      QString::fromStdString(HighwayCriterion::className()), false, outFile);

    HOOT_STR_EQUALS(
      FileUtils::readFully(inFileNameBase + "-out.osc"), FileUtils::readFully(outFile));
  }

private:

  geos::geom::Envelope _bounds;
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ChangesetReplacementCreatorTest, "slow");

}
