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
#include <hoot/core/io/ObjectInputStream.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Factory.h>

using namespace hoot;

// Standard
#include <sstream>
using namespace std;

#include "../TestUtils.h"

namespace hoot
{

class TestClass : public Serializable
{
public:
  static std::string className() { return "TestClass"; }

  virtual string getClassName() const { return className(); }

  virtual void readObject(QDataStream& stream)
  {
    stream >> a;
    stream >> b;
  }

  virtual void writeObject(QDataStream& ) const
  {
  }

  int a;
  int b;
};

HOOT_FACTORY_REGISTER_BASE(TestClass)

class ObjectInputStreamTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ObjectInputStreamTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    const unsigned char data[] = {
        0,   0,   0,  18,   0,  84,   0, 101,   0, 115,   0, 116,   0,  67,   0, 108,   0,  97,
        0, 115,   0, 115,   0,   0,   0,  42, 255, 255, 255, 255};
    size_t dataSize = 30;

    string s;
    s.resize(dataSize);
    memcpy((char*)s.data(), data, dataSize);
    stringstream ss(s, stringstream::in);

    ObjectInputStream uut(ss);

    TestClass* tc = uut.readObject<TestClass>();

    CPPUNIT_ASSERT_EQUAL(42, tc->a);
    CPPUNIT_ASSERT_EQUAL(-1, tc->b);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ObjectInputStreamTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ObjectInputStreamTest, "quick");

}
