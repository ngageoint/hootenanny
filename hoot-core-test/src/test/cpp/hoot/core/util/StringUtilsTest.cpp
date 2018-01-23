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

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

#include "../TestUtils.h"

namespace hoot
{

class StringUtilsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(StringUtilsTest);
  CPPUNIT_TEST(runHasAlphabeticCharTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runHasAlphabeticCharTest()
  {
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter("test"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("234"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("(*&%<?"));
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter("Şiḩḩī"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("_____"));
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter("-かまくらふしきあん"));
    CPPUNIT_ASSERT(StringUtils::hasAlphabeticCharacter(" か しきあん"));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter(""));
    CPPUNIT_ASSERT(!StringUtils::hasAlphabeticCharacter("  "));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StringUtilsTest, "quick");

}

