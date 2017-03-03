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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <iostream>
#include <stdlib.h>

class HdfsTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(HdfsTest);
  CPPUNIT_TEST(testWrite);
  CPPUNIT_TEST(testReadAll);
  CPPUNIT_TEST(testReadSeek);
  CPPUNIT_TEST_SUITE_END();

public:

  const static int count = 1000000;

  void testWrite()
  {
    Hdfs hdfs("default", 0);

    if (hdfs.exists("sequence.txt"))
    {
      hdfs.remove("sequence.txt");
    }

    auto_ptr<ostream> osPtr(hdfs.create("sequence.txt"));
    ostream& os = *osPtr;

    char buf[12];
    for (int i = 0; i < count; i++)
    {
      snprintf(buf, 12, "%8d\n", i);
      os << buf;
    }
    os.flush();

    CPPUNIT_ASSERT_EQUAL(true, hdfs.exists("sequence.txt"));
  }

  void testReadAll()
  {
    Hdfs hdfs("default", 0);

    auto_ptr<istream> isPtr(hdfs.open("sequence.txt"));
    istream& is = *isPtr;

    string bufIn, bufExpected;
    bufIn.resize(12);
    bufExpected.resize(12);
    for (int i = 0; i < 12; i++)
    {
      bufIn[i] = 0;
    }
    for (int i = 0; i < count; i++)
    {
      snprintf((char*)bufExpected.data(), 12, "%8d\n", i);
      is.read((char*)bufIn.data(), 9);
      CPPUNIT_ASSERT_EQUAL(9l, is.gcount());
      CPPUNIT_ASSERT_EQUAL(false, is.eof());
      CPPUNIT_ASSERT_EQUAL(bufExpected, bufIn);
    }

    // attempt to read again, this should cause us to hit the EOF.
    is.read((char*)bufIn.data(), 1);
    CPPUNIT_ASSERT_EQUAL(0l, is.gcount());
    CPPUNIT_ASSERT_EQUAL(true, is.eof());
  }

  void testReadSeek()
  {
    Hdfs hdfs("default", 0);

    auto_ptr<istream> isPtr(hdfs.open("sequence.txt"));
    istream& is = *isPtr;

    is.seekg(0, ios_base::end);
    CPPUNIT_ASSERT_EQUAL(count * 9, (int)is.tellg());
    is.seekg(0, ios_base::beg);
    CPPUNIT_ASSERT_EQUAL(0, (int)is.tellg());

    string bufIn, bufExpected;
    bufIn.resize(12);
    bufExpected.resize(12);
    for (int i = 0; i < 12; i++)
    {
      bufIn[i] = 0;
    }

    for (int i = 0; i < 1000; i++)
    {
      int l = rand() % count;
      snprintf((char*)bufExpected.data(), 12, "%8d\n", l);
      is.seekg(l * 9, ios_base::beg);
      CPPUNIT_ASSERT_EQUAL(l * 9, (int)is.tellg());
      is.read((char*)bufIn.data(), 9);
      CPPUNIT_ASSERT_EQUAL(9l, is.gcount());
      CPPUNIT_ASSERT_EQUAL(false, is.eof());
      CPPUNIT_ASSERT_EQUAL(bufExpected, bufIn);
    }
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(HdfsTest, "glacial");

