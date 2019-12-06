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
  CPPUNIT_TEST(runGetAvailableValidatorsTest);
  CPPUNIT_TEST(runValidatorInclusionTest);
  CPPUNIT_TEST(runValidatorExclusionTest);
  CPPUNIT_TEST(runEmptyValidatorsTest);
  CPPUNIT_TEST(runCleanNoErrorsTest);
  CPPUNIT_TEST(runCleanTest);
  CPPUNIT_TEST(runCleanNoDebugTagsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JosmMapCleanerTest() :
  HootTestFixture("test-files/ops/JosmMapCleanerTest", "test-output/ops/JosmMapCleanerTest")
  {
    //setResetType(ResetAll);
  }

  virtual void setUp()
  {
    // TODO: not being passed in correctly yet
    //conf().set("jni.class.path", QStringList("/home/vagrant/hoot/tmp/hoot-josm.jar"));
  }

  virtual void tearDown()
  {
  }

  void runGetAvailableValidatorsTest()
  {
    JosmMapCleaner uut;
    const QMap<QString, QString> validators = uut.getAvailableValidators();
    LOG_VART(validators.keys());
    LOG_VART(validators.values());
    // TODO: This won't hold up across version changes...come up with something better.
    CPPUNIT_ASSERT_EQUAL(51, validators.size());
  }

  void runValidatorInclusionTest()
  {
    JosmMapCleaner uut;
    QStringList validators;
    validators.append("UntaggedWay");
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);
    uut._initJosmValidatorsList();

    const QStringList validatorsUsed = uut.getJosmValidatorsUsed();
    LOG_VARD(validatorsUsed);
    CPPUNIT_ASSERT_EQUAL(3, validatorsUsed.size());
    const QString validatorsNamespace = uut.getValidatorsJosmNamespace();
    CPPUNIT_ASSERT(validatorsUsed.contains(validatorsNamespace + ".UntaggedWay"));
    CPPUNIT_ASSERT(validatorsUsed.contains(validatorsNamespace + ".UnclosedWays"));
    CPPUNIT_ASSERT(validatorsUsed.contains(validatorsNamespace + ".DuplicatedWayNodes"));
  }

  void runValidatorExclusionTest()
  {
    JosmMapCleaner uut;

    QStringList validators;
    validators.append("UntaggedWay");
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);

    validators.clear();
    validators.append("UntaggedWay");
    uut.setJosmValidatorsExclude(validators);

    uut._initJosmValidatorsList();

    const QStringList validatorsUsed = uut.getJosmValidatorsUsed();
    CPPUNIT_ASSERT_EQUAL(2, validatorsUsed.size());
    const QString validatorsNamespace = uut.getValidatorsJosmNamespace();
    CPPUNIT_ASSERT(validatorsUsed.contains(validatorsNamespace + ".UnclosedWays"));
    CPPUNIT_ASSERT(validatorsUsed.contains(validatorsNamespace + ".DuplicatedWayNodes"));
  }

  void runEmptyValidatorsTest()
  {
    JosmMapCleaner uut;
    uut._initJosmValidatorsList();
    // TODO: This won't hold up across version changes...come up with something better.
    CPPUNIT_ASSERT_EQUAL(51, uut.getJosmValidatorsUsed().size());
  }

  void runCleanNoErrorsTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm");
    LOG_VARD(map->size());

    JosmMapCleaner uut;
    uut.setAddDebugTags(true);
    uut.setJosmValidatorsInclude(QStringList("UntaggedWay"));
    LOG_INFO(uut.getInitStatusMessage());
    uut.apply(map);
    LOG_INFO(uut.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(40, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumGroupsOfElementsCleaned());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumElementsDeleted());
  }

  void runCleanTest()
  {
    const QString testName = "runCleanTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/" + testName + "-in.osm");
    LOG_VARD(map->size());

    JosmMapCleaner uut;
    uut.setAddDebugTags(true);
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);
    uut.apply(map);

    // TODO: verify summary out

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumGroupsOfElementsCleaned());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumElementsDeleted());

    // TODO: fix output

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }

  void runCleanNoDebugTagsTest()
  {
    const QString testName = "runCleanNoDebugTagsTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/runCleanTest-in.osm");
    LOG_VARD(map->size());

    JosmMapCleaner uut;
    uut.setAddDebugTags(false);
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);
    uut.apply(map);

    // TODO: verify summary out

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumGroupsOfElementsCleaned());
    CPPUNIT_ASSERT_EQUAL(2, uut.getNumElementsDeleted());

    // TODO: fix output

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JosmMapCleanerTest, "slow");

}
