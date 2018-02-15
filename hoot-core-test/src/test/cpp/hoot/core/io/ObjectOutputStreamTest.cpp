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
 * @copyright Copyright (C) 2013, 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/ObjectOutputStream.h>

using namespace hoot;

// Standard
#include <sstream>
using namespace std;

#include "../TestUtils.h"

namespace hoot
{

class TestClass2 : public Serializable
{
public:
  static std::string className() { return "TestClass"; }

  virtual string getClassName() const { return className(); }

  virtual void readObject(QDataStream& /*stream*/) {}

  virtual void writeObject(QDataStream& stream) const
  {
    stream << a;
    stream << b;
  }

  int a;
  int b;
};

class ObjectOutputStreamTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ObjectOutputStreamTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    stringstream ss;
    ObjectOutputStream uut(ss);

    TestClass2 t;
    t.a = 42;
    t.b = -1;
    uut.writeObject(t);
    uut.flush();

    //TestUtils::dumpString(ss.str());

    const unsigned char data[] = {
        0,   0,   0,  18,   0,  84,   0, 101,   0, 115,   0, 116,   0,  67,   0, 108,   0,  97,
        0, 115,   0, 115,   0,   0,   0,  42, 255, 255, 255, 255};
    size_t dataSize = 30;

    CPPUNIT_ASSERT_EQUAL(dataSize, ss.str().size());
    for (size_t i = 0; i < dataSize; i++)
    {
      CPPUNIT_ASSERT_EQUAL((unsigned int)data[i], (unsigned int)(unsigned char)ss.str().at(i));
    }
  }

  void _dumpString(const string& str)
  {
    cout << "const unsigned char data[] = {";
    for (size_t i = 0; i < str.size(); i++)
    {
      if (i != 0)
      {
        cout << ", ";
      }
      if (i % 18 == 0)
      {
        cout << endl << "  ";
      }
      //printf("%3d%c", (unsigned char)str.at(i),
      //  str.at(i) >= 32 && str.at(i) <= 125 ? (char)(str.at(i)) : '#');
      printf("%3u", (unsigned char)str.at(i));
    }
    cout << "};" << endl;
    cout << "size_t dataSize = " << str.size() << ";" << endl;
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ObjectOutputStreamTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ObjectOutputStreamTest, "quick");

}
