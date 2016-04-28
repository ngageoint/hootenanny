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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/OsmApiDbAwareHootApiDbWriter.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/OsmUtils.h>

// Standard
#include <functional>

// TGS
#include <tgs/BigContainers/BigMap.h>
#include <tgs/System/Time.h>

#include "../TestUtils.h"
#include "ServicesDbTestUtils.h"

namespace hoot
{

class OsmApiDbAwareHootApiDbWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiDbAwareHootApiDbWriterTest);
  //CPPUNIT_TEST(runTest);
  CPPUNIT_TEST_SUITE_END();

public:

  static QString userEmail() { return "OsmApiDbAwareHootApiDbWriterTest@hoottestcpp.org"; }

  void setUp()
  {
    ServicesDbTestUtils::deleteUser(userEmail());
    HootApiDb db;
    db.open(ServicesDbTestUtils::getDbModifyUrl());
    db.getOrCreateUser(userEmail(), "OsmApiDbAwareHootApiDbWriterTest");
  }

  void tearDown()
  {
    ServicesDbTestUtils::deleteUser(userEmail());
  }

  void runTest()
  {
    //TODO:
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiDbAwareHootApiDbWriterTest, "slow");

}
