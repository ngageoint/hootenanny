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
 * @copyright Copyright (C) 2015, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Standard
#include <iostream>
#include <math.h>
using namespace std;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// tgs
#include <tgs/System/SystemInfo.h>
#include "../PluginFactory.h"

#define ASSERT_DBL_EQL(expected, actual) CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, 1e-4)

namespace Tgs
{
  class SystemInfoTest : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(SystemInfoTest);
    CPPUNIT_TEST(test1);
    CPPUNIT_TEST_SUITE_END();

  public:

    void test1()
    {
      long vm, rss;
      SystemInfo::_getCurrentProcessMemoryUsage(vm, rss);
      CPPUNIT_ASSERT(vm > 0);
      CPPUNIT_ASSERT(rss > 0);

      CPPUNIT_ASSERT(SystemInfo::getTotalSystemVirtualMemory() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getSystemVirtualMemoryUsed() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getCurrentProcessVirtualMemoryUsage() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getVirtualMemoryAvailable() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getPercentageOfVirtualMemoryUsed() > 0);

      CPPUNIT_ASSERT(SystemInfo::getTotalSystemPhysicalMemory() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getSystemPhysicalMemoryUsed() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getCurrentProcessPhysicalMemoryUsage() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getPhysicalMemoryAvailable() >= 1);
      CPPUNIT_ASSERT(SystemInfo::getPercentageOfPhysicalMemoryUsed() > 0);

      CPPUNIT_ASSERT(SystemInfo::getNumberOfProcessors() >= 1);
    }
  };
}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::SystemInfoTest, "quick");
