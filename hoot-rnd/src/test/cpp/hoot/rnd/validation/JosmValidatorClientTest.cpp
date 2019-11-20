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
#include <hoot/rnd/validation/JosmValidatorClient.h>

// JNI
#include <jni.h>

// Qt
#include <QDir>

namespace hoot
{

class JosmValidatorClientTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JosmValidatorClientTest);
  //CPPUNIT_TEST(runToyValidationTest);
  CPPUNIT_TEST(runListValidatorsTest);
  CPPUNIT_TEST_SUITE_END();

public:

  JosmValidatorClientTest() :
  HootTestFixture(UNUSED_PATH, UNUSED_PATH)
  {
    setResetType(ResetAll);
  }

  virtual void setUp()
  {
    conf().set("validation.josm.library.path", "/home/vagrant/hoot/hoot/tmp/me-josm.4.4.4.jar");
    conf().set("validation.hoot.josm.library.path", "/home/vagrant/hoot/tmp/hoot-josm.jar");
  }

  virtual void tearDown()
  {
  }

  void runToyValidationTest()
  {
    JosmValidatorClient uut;
    CPPUNIT_ASSERT_EQUAL(0L, uut.getBlankNodeIdTest());
  }

  void runListValidatorsTest()
  {
    JosmValidatorClient uut;
    LOG_VARW(uut.getAvailableValidators());
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JosmValidatorClientTest, "quick");

}
