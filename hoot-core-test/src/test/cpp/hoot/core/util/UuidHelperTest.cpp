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
 * @copyright Copyright (C) 2013, 2014, 2015, 2017, 2018 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/UuidHelper.h>

namespace hoot
{

class UuidHelperTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(UuidHelperTest);
  CPPUNIT_TEST(uuid5Test);
  CPPUNIT_TEST_SUITE_END();

public:

  void uuid5Test()
  {
      QUuid r = UuidHelper::createUuid5("foo", QUuid("{6ba7b812-9dad-11d1-80b4-00c04fd430c8}"));
      HOOT_STR_EQUALS("{bca95adb-b5f1-564f-96a7-6355c52d1fa7}", r.toString());
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::UuidHelperTest, "quick");

