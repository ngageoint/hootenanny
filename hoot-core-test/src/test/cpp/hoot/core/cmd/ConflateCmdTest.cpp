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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/cmd/ConflateCmd.h>
#include <hoot/core/util/ConfigOptions.h>

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

namespace hoot
{

class ConflateCmdTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(ConflateCmdTest);
  CPPUNIT_TEST(runOpFilterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  ConflateCmdTest()
  {
    setResetType(ResetAll);
  }

  void runOpFilterTest()
  {
    QStringList matchCreators;
    QStringList conflatePreOps;
    QStringList conflatePostOps;
    QStringList mapCleanerTransforms;
    ConflateCmd uut;
    uut.setFilterOps(true);

    // Test 1

    matchCreators.clear();
    matchCreators.append("hoot::BuildingMatchCreator");
    conf().set(ConfigOptions::getMatchCreatorsKey(), matchCreators);
    conflatePreOps.clear();
    conflatePreOps.append("");
    conf().set(ConfigOptions::getConflatePreOpsKey(), conflatePreOps);
    conflatePostOps.clear();
    conflatePostOps.append("");
    conf().set(ConfigOptions::getConflatePostOpsKey(), conflatePostOps);
    mapCleanerTransforms.clear();
    mapCleanerTransforms.append("");
    conf().set(ConfigOptions::getMapCleanerTransformsKey(), mapCleanerTransforms);

    uut._removeSuperfluousOps();

    conflatePreOps = conf().getList(ConfigOptions::getConflatePreOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, conflatePreOps.size());
    CPPUNIT_ASSERT(conflatePostOps.contains(""));
    conflatePostOps = conf().getList(ConfigOptions::getConflatePostOpsKey());
    CPPUNIT_ASSERT_EQUAL(1, conflatePostOps.size());
    CPPUNIT_ASSERT(conflatePostOps.contains(""));
    mapCleanerTransforms = conf().getList(ConfigOptions::getMapCleanerTransformsKey());
    CPPUNIT_ASSERT_EQUAL(1, mapCleanerTransforms.size());
    CPPUNIT_ASSERT(mapCleanerTransforms.contains(""));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConflateCmdTest, "quick");

}
