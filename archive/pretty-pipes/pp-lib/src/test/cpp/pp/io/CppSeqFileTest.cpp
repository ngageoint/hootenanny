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

// Pretty Pipes
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/io/CppSeqFile.h>

using namespace std;

class Int
{
public:
  int v;

  virtual void readFields(char* byteArray, long)
  {
    v = *(int*)byteArray;
  }

  signed char* writeFields(size_t* len) const
  {
    signed char* result = new signed char[4];
    *(int*)result = v;
    *len = 4;
    return result;
  }
};

class CppSeqFileTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CppSeqFileTest);
  CPPUNIT_TEST(testWriteRead);
  CPPUNIT_TEST_SUITE_END();

public:

  void testWriteRead()
  {
    stringstream sout(stringstream::out);

    CppSeqFile::Writer writer(sout);

    int c = 0;
    Int i1, i2;
    for (int i = 0; i < 10; i++)
    {
      i1.v = c++;
      i2.v = c++;
      writer.append<Int, Int>(i1, i2);
    }
    writer.close();

    sout.flush();

    string s = sout.str();

    stringstream sin(s, stringstream::in);
    CppSeqFile::Reader reader(sin);

    int d = 0;
    while (reader.next<Int, Int>(i1, i2))
    {
      CPPUNIT_ASSERT_EQUAL(d++, i1.v);
      CPPUNIT_ASSERT_EQUAL(d++, i2.v);
    }
    CPPUNIT_ASSERT_EQUAL(c, d);
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(CppSeqFileTest);



