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
 * @copyright Copyright (C) 2015, 2016, 2017 Maxar (http://www.maxar.com/)
 */


// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <pp/Hdfs.h>
using namespace pp;

// Qt
#include <QDateTime>

// Standard
#include <sstream>
#include <string>

// Pretty Pipes
#include <pp/conf/Configuration.h>
#include <pp/Hdfs.h>
#include <pp/io/LineRecordWriter.h>

using namespace std;

class LineRecordWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(LineRecordWriterTest);
  CPPUNIT_TEST(testWrite);
  CPPUNIT_TEST_SUITE_END();

public:

  void testWrite()
  {
    Hdfs fs;
    string outDir = "test-output/LineRecordWriterTest/output/";
    if (fs.exists(outDir))
    {
      fs.deletePath(outDir, true);
    }

    {
      LineRecordWriter uut;

      uut.init(outDir, 0);
      string key("key");
      string value("value");
      uut.emitRecord(key.data(), key.size(), value.data(), value.size());
      key = "key2";
      value = "value2";
      uut.emitRecord(key.data(), key.size(), value.data(), value.size());
    }

    boost::shared_ptr<istream> is(fs.open("test-output/LineRecordWriterTest/output/part-00000.txt"));
    char buffer[1000] = { '\0' };
    is->read(buffer, 1000);
    string output(buffer);
    string base("key\tvalue\nkey2\tvalue2\n");
    CPPUNIT_ASSERT_EQUAL(base, output);
  }

};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LineRecordWriterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(LineRecordWriterTest, "glacial");
