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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/JosmMapCleaner.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class JosmMapCleanerTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JosmMapCleanerTest);
  CPPUNIT_TEST(runCleanNoErrorsTest);
  CPPUNIT_TEST(runCleanTest);
  CPPUNIT_TEST(runCleanNoDetailTagsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JosmMapCleanerTest() :
  HootTestFixture("test-files/ops/JosmMapCleanerTest", "test-output/ops/JosmMapCleanerTest")
  {
  }

  void runCleanNoErrorsTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm");
    LOG_VARD(map->size());

    JosmMapCleaner uut;
    uut.setLogMissingCertAsWarning(false);
    uut.setAddDetailTags(true);
    uut.setJosmValidatorsInclude(QStringList("UntaggedWay"));
    LOG_INFO(uut.getInitStatusMessage());
    uut.apply(map);
    LOG_INFO(uut.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(40, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumGroupsOfElementsCleaned());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumElementsDeleted());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumFailingValidators());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumFailedCleaningOperations());
  }

  void runCleanTest()
  {
    const QString testName = "runCleanTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/" + testName + "-in.osm");
    LOG_VARD(map->size());

    JosmMapCleaner uut;
    uut.setLogMissingCertAsWarning(false);
    uut.setAddDetailTags(true);
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumGroupsOfElementsCleaned());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumElementsDeleted());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumFailedCleaningOperations());

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }

  void runCleanNoDetailTagsTest()
  {
    const QString testName = "runCleanNoDebugTagsTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/runCleanTest-in.osm");
    LOG_VARD(map->size());

    JosmMapCleaner uut;
    uut.setAddDetailTags(false);
    uut.setLogMissingCertAsWarning(false);
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumGroupsOfElementsCleaned());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumElementsDeleted());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumFailedCleaningOperations());

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JosmMapCleanerTest, "quick");

}
