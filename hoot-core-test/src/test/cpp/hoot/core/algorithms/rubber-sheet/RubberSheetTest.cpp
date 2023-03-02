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
 * @copyright Copyright (C) 2013-2023 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheet.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheetApplier.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheetCreator.h>
#include <hoot/core/algorithms/rubber-sheet/RubberSheetDeriver.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <tgs/Statistics/Random.h>

// Qt
#include <QBuffer>
#include <QByteArray>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class RubberSheetTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RubberSheetTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runIoTest);
  CPPUNIT_TEST(runCalculateTiePointDistancesTest);
  CPPUNIT_TEST(runCalculateTiePointDistancesNotEnoughTiePointsTest1);
  CPPUNIT_TEST(runCalculateTiePointDistancesNotEnoughTiePointsTest2);
  CPPUNIT_TEST(runFilterTest1);
  CPPUNIT_TEST(runFilterTest2);
  CPPUNIT_TEST(runJsonTiePointsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RubberSheetTest() :
  HootTestFixture("test-files/algorithms/rubber-sheet/", "test-output/algorithms/rubber-sheet/")
  {
    setResetType(ResetEnvironment);
  }

  void runSimpleTest()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcGisRoads.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/DcTigerRoads.osm", map);

    MapCropper cropper;
    cropper.setBounds(Envelope(-77.0554,-77.0441,38.8833,38.8933));
    cropper.apply(map);

    MapCleaner().apply(map);
    RubberSheet uut;
    uut.setReference(false);
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    OsmXmlWriter writer;
    writer.write(map, _outputPath + "RubberSheetSimple.osm");

    HOOT_FILE_EQUALS(_inputPath + "RubberSheetSimple.osm", _outputPath + "RubberSheetSimple.osm");
  }

  void runIoTest()
  {
    QByteArray arr1;
    QByteArray arr2;
    {
      OsmXmlReader reader;
      OsmMap::resetCounters();
      OsmMapPtr map = std::make_shared<OsmMap>();
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/DcGisRoads.osm", map);
      reader.setDefaultStatus(Status::Unknown2);
      reader.read("test-files/DcTigerRoads.osm", map);

      MapCropper cropper;
      cropper.setBounds(Envelope(-77.0554,-77.0441,38.8833,38.8933));
      cropper.apply(map);

      MapCleaner().apply(map);
      RubberSheet uut;
      uut.setReference(false);
      uut.setMinimumTies(10);
      uut.calculateTransform(map);

      QBuffer buf1(&arr1);
      QBuffer buf2(&arr2);
      buf1.open(QIODevice::WriteOnly);
      buf2.open(QIODevice::WriteOnly);
      uut.writeTransform1to2(buf1);
      uut.writeTransform2to1(buf2);
      buf1.close();
      buf2.close();
    }

    {
      OsmXmlReader reader;
      OsmMap::resetCounters();
      OsmMapPtr map = std::make_shared<OsmMap>();
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/DcGisRoads.osm", map);
      reader.setDefaultStatus(Status::Unknown2);
      reader.read("test-files/DcTigerRoads.osm", map);

      MapCropper cropper;
      cropper.setBounds(Envelope(-77.0554,-77.0441,38.8833,38.8933));
      cropper.apply(map);

      MapCleaner().apply(map);
      RubberSheet uut;
      uut.setReference(false);
      uut.setMinimumTies(10);

      QBuffer buf1(&arr1);
      QBuffer buf2(&arr2);
      buf1.open(QIODevice::ReadOnly);
      buf2.open(QIODevice::ReadOnly);
      uut.readTransform1to2(buf1);
      uut.readTransform2to1(buf2);

      uut.applyTransform(map);

      MapProjector::projectToWgs84(map);

      OsmXmlWriter writer;
      // for testing we don't need a high precision.
      writer.setPrecision(7);
      writer.write(map, _outputPath + "RubberSheetIo.osm");

      HOOT_FILE_EQUALS(_inputPath + "RubberSheetIo.osm",
                       _outputPath + "RubberSheetIo.osm");
    }
  }

  void runCalculateTiePointDistancesTest()
  {
    RubberSheet rubberSheet;
    vector<RubberSheet::Tie> ties;
    RubberSheet::Tie tie1;
    tie1.c1 = Coordinate(11426.757023323107, -1875.2719014191346);
    tie1.c2 = Coordinate(11417.464191739389, -1877.1562578742921);
    ties.push_back(tie1);
    RubberSheet::Tie tie2;
    tie2.c1 = Coordinate(5210.5676019934281, -3336.1151452254558);
    tie2.c2 = Coordinate(5203.7921899235298, -3345.3789840725226);
    ties.push_back(tie2);

    rubberSheet._ties = ties;
    vector<double> tiePointDistances = rubberSheet.calculateTiePointDistances();
    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.481958, tiePointDistances.at(0), 1e-6);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(11.477148, tiePointDistances.at(1), 1e-6);
  }

  void runCalculateTiePointDistancesNotEnoughTiePointsTest1()
  {
    RubberSheet rubberSheet;

    QString exceptionMsg("");
    try
    {
      rubberSheet.calculateTiePointDistances();
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT_EQUAL(
      QString("No tie points have been generated.").toStdString(), exceptionMsg.toStdString());
  }

  void runCalculateTiePointDistancesNotEnoughTiePointsTest2()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcGisRoads.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/DcTigerRoads.osm", map);

    MapCropper cropper;
    cropper.setBounds(Envelope(-77.0554,-77.0441,38.8833,38.8933));
    cropper.apply(map);

    MapCleaner().apply(map);
    RubberSheet uut;
    uut.setReference(false);
    uut.setFailWhenMinimumTiePointsNotFound(true);
    uut.setMinimumTies(999);

    QString exceptionMsg("");
    try
    {
      uut.apply(map);
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(
      exceptionMsg.contains("Error rubbersheeting due to not finding enough tie points"));
  }

  void runFilterTest1()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/cmd/glacial/RelationMergeTest/input1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/cmd/glacial/RelationMergeTest/input2.osm", map);

    RubberSheet uut;
    uut.setReference(true);
    uut.setDebug(true);
    uut.setCriteria(QStringList("RiverCriterion"), map);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter().write(map, _outputPath + "runFilterTest1-out.osm");

    CPPUNIT_ASSERT_EQUAL(5419L, uut.getNumFeaturesProcessed());
    CPPUNIT_ASSERT_EQUAL(141L, uut.getNumFeaturesAffected());
    HOOT_FILE_EQUALS(_inputPath + "runFilterTest1-out.osm", _outputPath + "runFilterTest1-out.osm");
  }

  void runFilterTest2()
  {
    OsmXmlReader reader;
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/cmd/glacial/RelationMergeTest/input1.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/cmd/glacial/RelationMergeTest/input2.osm", map);

    RubberSheet uut;
    uut.setReference(true);
    uut.setDebug(true);
    QStringList criteria;
    criteria.append("RiverCriterion");
    criteria.append("HighwayCriterion");
    uut.setCriteria(criteria, map);
    uut.apply(map);

    MapProjector::projectToWgs84(map);
    OsmXmlWriter().write(map, _outputPath + "runFilterTest2-out.osm");

    CPPUNIT_ASSERT_EQUAL(5419L, uut.getNumFeaturesProcessed());
    CPPUNIT_ASSERT_EQUAL(4026L, uut.getNumFeaturesAffected());
    HOOT_FILE_EQUALS(_inputPath + "runFilterTest2-out.osm", _outputPath + "runFilterTest2-out.osm");
  }

  void runJsonTiePointsTest()
  {
    RubberSheetCreator().create(_inputPath + "manual_tie_points.json",
                                _inputPath + "manual_tie_points_original.osm",
                                _inputPath + "manual_tie_points_offset.osm",
                                _outputPath + "manual_tie_points_transform.rs", "",
                                true);
    RubberSheetApplier().apply(_outputPath + "manual_tie_points_transform.rs",
                               _inputPath + "manual_tie_points_offset.osm",
                               _outputPath + "manual_tie_points_output.osm");

    HOOT_FILE_EQUALS(_inputPath + "manual_tie_points_expected.osm", _outputPath + "manual_tie_points_output.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RubberSheetTest, "slow");

}
