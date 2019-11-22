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

namespace hoot
{

class JosmValidatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JosmValidatorTest);
  CPPUNIT_TEST(runGetAvailableValidatorsTest);
  //CPPUNIT_TEST(runValidateTest);
  //CPPUNIT_TEST(runValidateAndFixTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JosmValidatorTest() :
  HootTestFixture(UNUSED_PATH, UNUSED_PATH)
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

  void runValidateTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "TODO");

    JosmValidator uut(false);
    uut.setValidatorsToUse(QStringList("TODO"));
    uut.apply(map);

    // TODO: map assertions
  }

  void runValidateAndFixTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(map, "TODO");

    JosmValidator uut(true);
    uut.setValidatorsToUse(QStringList("TODO"));
    uut.apply(map);

    // TODO: map assertions
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JosmValidatorTest, "quick");

}
