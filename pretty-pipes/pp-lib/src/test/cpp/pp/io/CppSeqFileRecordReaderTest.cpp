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

// Qt
#include <QDateTime>

// Standard
#include <math.h>
#include <sstream>
#include <string>

// Pretty Pipes
#include <pp/conf/Configuration.h>
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Hdfs.h>
#include <pp/io/CppSeqFile.h>
#include <pp/io/CppSeqFileRecordReader.h>
#include <pp/io/FileInputFormat.h>


class DummyBlock
{
public:
  int v;
  static const int count = 1000;

  virtual void readFields(char* byteArray, long)
  {
    int* a = (int*)byteArray;
    v = a[0];

    for (int i = 1; i < count; i++)
    {
      if (v + i != a[i])
      {
        throw std::exception();
      }
    }
  }

  signed char* writeFields(size_t* len) const
  {
    *len = sizeof(int) * count;
    int* result = new int[count];
    result[0] = v;
    for (int i = 1; i < count; i++)
    {
      result[i] = v + i;
    }
    return (signed char*)result;
  }
};

class CppSeqFileRecordReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CppSeqFileRecordReaderTest);
  CPPUNIT_TEST(testWriteRead);
  CPPUNIT_TEST_SUITE_END();

public:

  void testWriteRead()
  {
    Hdfs fs;

    auto_ptr<ostream> out(fs.create("tmp/dum.csq"));

    CppSeqFile::Writer writer(*out);

    int count = 10000;
    DummyBlock i1, i2;
    int c = 0;
    for (int i = 0; i < count; i++)
    {
      i1.v = c++;
      i2.v = c++;
      writer.append<DummyBlock, DummyBlock>(i1, i2);
    }
    writer.close();
    out.reset();

    auto_ptr<istream> in(fs.open("tmp/dum.csq"));
    CppSeqFile::Reader reader(*in);

    int d = 0;
    while (reader.next<DummyBlock, DummyBlock>(i1, i2))
    {
      CPPUNIT_ASSERT_EQUAL(d++, i1.v);
      CPPUNIT_ASSERT_EQUAL(d++, i2.v);
    }
    CPPUNIT_ASSERT_EQUAL(c, d);

    FileInputFormat fif;
    Configuration conf;
    conf.set("mapred.input.dir", "tmp/dum.csq");

    FileStatus status = fs.getFileStatus("tmp/dum.csq");
    CPPUNIT_ASSERT_EQUAL(80280020l, status.getLen());
    conf.set("mapred.input.dir", "tmp/dum.csq");
    fif.setConfiguration(conf);
    CPPUNIT_ASSERT_EQUAL((int)ceil((double)status.getLen() / (double)status.getBlockSize()),
      fif.getSplitCount());

    int e = 0;
    for (int i = 0; i < fif.getSplitCount(); i++)
    {
      CppSeqFileRecordReader uut;
      string splitBytes;
      fif.getSplit(i).writeFields(splitBytes);
      stringstream ss(stringstream::out);
      DataOutputStream dos(ss);
      dos.writeUTF("");
      dos.writeUTF(QString::fromStdString(FileInputSplit::className()));
      dos.writeInt(splitBytes.size());
      dos.write(splitBytes.data(), splitBytes.size());
      ss.flush();
      uut.readInputSplit(ss.str());

      string key, value;
      while (uut.next(key, value))
      {
        e++;
      }
    }

    CPPUNIT_ASSERT_EQUAL(count, e);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CppSeqFileRecordReaderTest, "glacial");


