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
 * @copyright Copyright (C) 2012, 2013, 2014, 2015, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/conflate/NodeReplacements.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
using namespace hoot;

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class NodeReplacementsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(NodeReplacementsTest);
  CPPUNIT_TEST(runSimplifyTest);
  CPPUNIT_TEST(runIoTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/conflate");
  }

  void runIoTest()
  {
    NodeReplacements uut;

    HashMap<long, long>& m = uut.getReplacements();
    m[0] = 1;
    m[1] = 2;
    m[2] = 3;

    uut.write("test-output/conflate/Node.replacements");

    NodeReplacements uut2;
    uut2.read("test-output/conflate/Node.replacements");

    CPPUNIT_ASSERT_EQUAL(std::string("0 : 3\n1 : 3\n2 : 3\n"), uut2.toString().toStdString());
  }

  void runSimplifyTest()
  {
    NodeReplacements uut;

    HashMap<long, long>& m = uut.getReplacements();
    m[0] = 1;
    m[1] = 2;
    m[2] = 3;

    uut.simplify();

    CPPUNIT_ASSERT_EQUAL(std::string("0 : 3\n1 : 3\n2 : 3\n"), uut.toString().toStdString());

    // should find the circular reference.
    m[3] = 0;
    CPPUNIT_ASSERT_THROW(uut.simplify(), HootException);
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(NodeReplacementsTest, "quick");

}
