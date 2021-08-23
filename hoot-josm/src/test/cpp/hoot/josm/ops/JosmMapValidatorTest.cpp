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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>

#include <hoot/josm/ops/JosmMapValidator.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class JosmMapValidatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JosmMapValidatorTest);
  CPPUNIT_TEST(runGetAvailableValidatorDetailTest);
  CPPUNIT_TEST(runEmptyValidatorsTest);
  CPPUNIT_TEST(runValidateTest);
  CPPUNIT_TEST(runValidateFileTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JosmMapValidatorTest() :
  HootTestFixture(
    "test-files/ops/JosmMapValidatorTest", "test-output/ops/JosmMapValidatorTest")
  {
    setResetType(ResetBasic);
  }

  void runGetAvailableValidatorDetailTest()
  {
    JosmMapValidator uut;
    uut.setConfiguration(conf());
    const QMap<QString, QString> validators = uut.getValidatorDetail();
    LOG_VART(validators.keys());
    LOG_VART(validators.values());
    CPPUNIT_ASSERT(validators.size() > 0);
    CPPUNIT_ASSERT(validators.contains("DuplicatedWayNodes"));
    // We replace the inner class separator, '$', with '.' for readability, so check that.
    CPPUNIT_ASSERT(validators.contains("CrossingWays.Boundaries"));
  }

  void runEmptyValidatorsTest()
  {   
    JosmMapValidator uut;
    QString exceptionMsg;
    try
    {
      uut.getValidatorDetail();
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    HOOT_STR_EQUALS("No validators configured.", exceptionMsg);
  }

  void runValidateTest()
  {
    const QString testName = "runValidateTest";
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm");
    LOG_VARD(map->size());

    JosmMapValidator uut;
    uut.setMaxElementsForMapString(INT_MAX);
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidators(validators);
    LOG_INFO(uut.getInitStatusMessage());
    uut.apply(map);
    LOG_INFO(uut.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumFailingValidators());
    HOOT_STR_EQUALS(
      "Found 4 validation errors in 45 features with JOSM.\n"
      "Duplicated way nodes errors: 1\n"
      "Unclosed Ways errors: 2\n"
      "Untagged, empty and one node ways errors: 1\n"
      "Total failing JOSM validators: 0",
      uut.getSummary());

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }

  void runValidateFileTest()
  {
    const QString testName = "runValidateFileTest";
    OsmMapPtr map = std::make_shared<OsmMap>();
    OsmMapReaderFactory::read(map, "test-files/ops/JosmMapCleanerTest/runCleanTest-in.osm");
    LOG_VARD(map->size());

    JosmMapValidator uut;
    uut.setMaxElementsForMapString(1);  // force temp map file I/O
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidators(validators);
    LOG_INFO(uut.getInitStatusMessage());
    uut.apply(map);
    LOG_INFO(uut.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumFailingValidators());
    HOOT_STR_EQUALS(
      "Found 4 validation errors in 45 features with JOSM.\n"
      "Duplicated way nodes errors: 1\n"
      "Unclosed Ways errors: 2\n"
      "Untagged, empty and one node ways errors: 1\n"
      "Total failing JOSM validators: 0",
      uut.getSummary());

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/runValidateTest-out.osm", _outputPath + "/" + outTestFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JosmMapValidatorTest, "slow");

}
