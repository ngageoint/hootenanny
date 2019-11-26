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
#include <hoot/rnd/ops/JosmValidator.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

static const QString JOSM_TESTS_NAMESPACE = "org.openstreetmap.josm.data.validation.tests";

class JosmValidatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JosmValidatorTest);
  CPPUNIT_TEST(runGetAvailableValidatorsTest);
  CPPUNIT_TEST(runValidateNoErrorsTest);
  CPPUNIT_TEST(runValidateWithErrorsTest);
  //CPPUNIT_TEST(runValidateAndFixTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JosmValidatorTest() :
  HootTestFixture("test-files/rnd/ops/JosmValidatorTest", "test-output/rnd/ops/JosmValidatorTest")
  {
    setResetType(ResetAll);
  }

  virtual void setUp()
  {
    // TODO: not being passed in correctly yet
    conf().set("validation.josm.library.path", "/home/vagrant/hoot/hoot/tmp/me-josm.4.4.4.jar");
    conf().set("validation.hoot.josm.library.path", "/home/vagrant/hoot/tmp/hoot-josm.jar");
  }

  virtual void tearDown()
  {
  }

  void runGetAvailableValidatorsTest()
  {
    JosmValidator uut;
    const QMap<QString, QString> validators = uut.getAvailableValidators();
    LOG_VART(validators.keys());
    LOG_VART(validators.values());
    CPPUNIT_ASSERT_EQUAL(51, validators.size());
  }

  void runValidateNoErrorsTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "test-files/ToyTestA.osm");
    LOG_VARD(map->size());

    JosmValidator uut(false);
    // TODO: add another validator
    uut.setValidatorsToUse(QStringList(JOSM_TESTS_NAMESPACE + ".UntaggedWay"));
    LOG_INFO(uut.getInitStatusMessage());
    uut.apply(map);
    LOG_INFO(uut.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(40, uut.getNumElementsValidated());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumElementsFixed());
  }

  void runValidateWithErrorsTest()
  {
    const QString testName = "runValidateWithErrorsTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/" + testName + "-in.osm", true, true);
    LOG_VARD(map->size());

    // The next to last way has both no tags and only one node, so it should definitely cause a
    // validation error in UntaggedWay, and it will be marked with a hoot validation error tag.

    JosmValidator uut(false);
    // TODO: add another validator
    uut.setValidatorsToUse(QStringList(JOSM_TESTS_NAMESPACE + ".UntaggedWay"));
    LOG_INFO(uut.getInitStatusMessage());
    uut.apply(map);
    LOG_INFO(uut.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(40, uut.getNumElementsValidated());
    CPPUNIT_ASSERT_EQUAL(1, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(0, uut.getNumElementsFixed());

    const QString outTestFileName =  testName + "-out.osm";
    // TODO: setting debug to false should actually suppress the validation tag...
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }

  void runValidateAndFixTest()
  {
    const QString testName = "runValidateAndFixTest";
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, _inputPath + "/runValidateWithErrorsTest-in.osm");
    LOG_VARD(map->size());

    JosmValidator uut(true);
    // TODO: replace UntaggedWay and then add another validator
    uut.setValidatorsToUse(QStringList(JOSM_TESTS_NAMESPACE + ".UntaggedWay"));
    LOG_INFO(uut.getInitStatusMessage());
    uut.apply(map);
    LOG_INFO(uut.getCompletedStatusMessage());

    CPPUNIT_ASSERT_EQUAL(40, uut.getNumElementsValidated());
    CPPUNIT_ASSERT_EQUAL(1, uut.getNumValidationErrors());
    CPPUNIT_ASSERT_EQUAL(1, uut.getNumElementsFixed());

    const QString outTestFileName =  testName + "-out.osm";
    OsmMapWriterFactory::write(map, _outputPath + "/" + outTestFileName, false, false);
    HOOT_FILE_EQUALS(_inputPath + "/" + outTestFileName, _outputPath + "/" + outTestFileName);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JosmValidatorTest, "quick");

}
