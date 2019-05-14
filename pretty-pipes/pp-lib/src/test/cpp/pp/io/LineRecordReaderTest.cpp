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
#include <sstream>
#include <string>

// Pretty Pipes
#include <pp/conf/Configuration.h>
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Hdfs.h>
#include <pp/io/LineRecordReader.h>
#include <pp/io/FileInputFormat.h>

using namespace pp;
using namespace std;

class LineRecordReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(LineRecordReaderTest);
  CPPUNIT_TEST(testRead);
  CPPUNIT_TEST_SUITE_END();

public:

  void testRead()
  {
    string fn = "test-output/pp/io/LineRecordReaderTest.txt";

    Hdfs hdfs("default", 0);

    if (hdfs.exists(fn))
    {
      hdfs.remove(fn);
    }

    boost::shared_ptr<ostream> osPtr(hdfs.create(fn));
    ostream& os = *osPtr;

    int count = 5000;
    long size = 0;
    char buf[12];
    for (int i = 0; i < count; i++)
    {
      snprintf(buf, 12, "%d\n", i);
      size += strlen(buf);
      os << buf;
    }
    os.flush();
    osPtr.reset();

    string keyStr;
    string value;
    int i = 0;
    long start = 0;
    long length = 500;

    while (start < size)
    {
      FileInputSplit* s = new FileInputSplit;
      s->setLength(std::min(size - start, length));
      s->setPath(fn);
      s->setStart(start);

      LineRecordReader reader;
      reader.initialize(s);

      while (reader.next(keyStr, value))
      {
        snprintf(buf, 12, "%d", i++);
        CPPUNIT_ASSERT_EQUAL(string(buf), value);
      }
      start += length;
    }
    CPPUNIT_ASSERT_EQUAL(count, i);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LineRecordReaderTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LineRecordReaderTest, "glacial");

