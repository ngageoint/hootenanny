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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */


// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <pp/Hdfs.h>
using namespace pp;

// Standard
#include <sstream>
#include <string>

#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>

using namespace std;

class DataInputStreamTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DataInputStreamTest);
  CPPUNIT_TEST(testWriteRead);
  CPPUNIT_TEST_SUITE_END();

public:

  void testWriteRead()
  {
    stringstream sout(stringstream::out);

    DataOutputStream dos(sout);
    dos.writeInt(42);
    dos.writeInt(111000111);
    dos.writeLong(0x5555FFFFAAAA1111);
    dos.writeLong(0x123456789ABCDEF0);
    dos.writeString("Hello World.");
    dos.writeInt(0xDEADBEEF);
    dos.writeString("Good Bye");

    sout.flush();

    string s = sout.str();

    stringstream sin(s, stringstream::in);
    DataInputStream dis(sin);

    CPPUNIT_ASSERT_EQUAL(42, dis.readInt());
    CPPUNIT_ASSERT_EQUAL(111000111, dis.readInt());
    CPPUNIT_ASSERT_EQUAL(0x5555FFFFAAAA1111, dis.readLong());
    CPPUNIT_ASSERT_EQUAL(0x123456789ABCDEF0, dis.readLong());
    CPPUNIT_ASSERT_EQUAL(string("Hello World."), dis.readString());
    CPPUNIT_ASSERT_EQUAL((int)0xDEADBEEF, dis.readInt());
    CPPUNIT_ASSERT_EQUAL(string("Good Bye"), dis.readString());
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(DataInputStreamTest);


