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
#include <hoot/core/ops/JosmMapValidator.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

class JosmMapValidatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JosmMapValidatorTest);
  CPPUNIT_TEST(runValidateTest);
  //CPPUNIT_TEST(runValidateMapUnmodifiedTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JosmMapValidatorTest() :
  HootTestFixture(
    // note: using JosmMapCleanerTest's dir for input
    "test-files/ops/JosmMapCleanerTest", "test-output/ops/JosmMapValidatorTest")
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

  void runValidateTest()
  {
    const QString testName = "runValidateTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/runCleanTest-in.osm");
    LOG_VARD(map->size());

    JosmMapValidator uut;
    uut.setModifyMap(false);
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }

  void runValidateMapUnmodifiedTest()
  {
    const QString testName = "runValidateMapUnmodifiedTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/runCleanTest-in.osm");
    LOG_VARD(map->size());

    JosmMapValidator uut;
    uut.setModifyMap(false);
    QStringList validators;
    validators.append("UntaggedWay");   // triggers "One node way"
    validators.append("UnclosedWays");
    validators.append("DuplicatedWayNodes");
    uut.setJosmValidatorsInclude(validators);
    uut.apply(map);

    CPPUNIT_ASSERT_EQUAL(45, uut.getNumElementsProcessed());
    CPPUNIT_ASSERT_EQUAL(4, uut.getNumValidationErrors());

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JosmMapValidatorTest, "slow");

}
