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
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/ops/MapCropper.h>

#include <tgs/Statistics/Random.h>

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

class RubberSheetTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RubberSheetTest);
  CPPUNIT_TEST(runSimpleTest);
  CPPUNIT_TEST(runIoTest);
  CPPUNIT_TEST(runCalculateTiePointDistancesTest);
  CPPUNIT_TEST(runCalculateTiePointDistancesNotEnoughTiePointsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runIoTest()
  {
    TestUtils::resetEnvironment();
    QByteArray arr1;
    QByteArray arr2;
    {
      OsmXmlReader reader;
      OsmMap::resetCounters();
      boost::shared_ptr<OsmMap> map(new OsmMap());
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/DcGisRoads.osm", map);
      reader.setDefaultStatus(Status::Unknown2);
      reader.read("test-files/DcTigerRoads.osm", map);

      MapCropper(Envelope(-77.0554,-77.0441,38.8833,38.8933)).apply(map);

      MapCleaner().apply(map);
      RubberSheet uut;
      uut.setReference(false);
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
      boost::shared_ptr<OsmMap> map(new OsmMap());
      reader.setDefaultStatus(Status::Unknown1);
      reader.read("test-files/DcGisRoads.osm", map);
      reader.setDefaultStatus(Status::Unknown2);
      reader.read("test-files/DcTigerRoads.osm", map);

      MapCropper(Envelope(-77.0554,-77.0441,38.8833,38.8933)).apply(map);

      MapCleaner().apply(map);
      RubberSheet uut;

      QBuffer buf1(&arr1);
      QBuffer buf2(&arr2);
      buf1.open(QIODevice::ReadOnly);
      buf2.open(QIODevice::ReadOnly);
      uut.readTransform1to2(buf1);
      uut.readTransform2to1(buf2);

      uut.applyTransform(map);

      MapProjector::projectToWgs84(map);

      QDir().mkdir("test-output/conflate/");
      OsmXmlWriter writer;
      // for testing we don't need a high precision.
      writer.setPrecision(7);
      writer.write(map, "test-output/conflate/RubberSheetIo.osm");

      HOOT_FILE_EQUALS("test-files/conflate/RubberSheetIo.osm",
                       "test-output/conflate/RubberSheetIo.osm");
    }
  }

  void runSimpleTest()
  {
    TestUtils::resetEnvironment();

    OsmXmlReader reader;
    OsmMap::resetCounters();
    boost::shared_ptr<OsmMap> map(new OsmMap());
    reader.setDefaultStatus(Status::Unknown1);
    reader.read("test-files/DcGisRoads.osm", map);
    reader.setDefaultStatus(Status::Unknown2);
    reader.read("test-files/DcTigerRoads.osm", map);

    MapCropper(Envelope(-77.0554,-77.0441,38.8833,38.8933)).apply(map);

    MapCleaner().apply(map);
    RubberSheet uut;
    uut.setReference(false);
    uut.apply(map);

    MapProjector::projectToWgs84(map);

    QDir().mkdir("test-output/conflate/");
    OsmXmlWriter writer;
    writer.write(map, "test-output/conflate/RubberSheetSimple.osm");

    HOOT_FILE_EQUALS("test-files/conflate/RubberSheetSimple.osm",
                     "test-output/conflate/RubberSheetSimple.osm");
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

  void runCalculateTiePointDistancesNotEnoughTiePointsTest()
  {
    RubberSheet rubberSheet;
    vector<RubberSheet::Tie> ties;
    rubberSheet._ties = ties;

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

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RubberSheetTest, "slow");

}
