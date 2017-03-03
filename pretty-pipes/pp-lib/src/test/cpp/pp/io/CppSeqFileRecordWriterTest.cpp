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
#include <sstream>
#include <string>

// Pretty Pipes
#include <pp/conf/Configuration.h>
#include <pp/DataInputStream.h>
#include <pp/DataOutputStream.h>
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
#include <pp/io/CppSeqFile.h>
#include <pp/io/CppSeqFileRecordReader.h>
#include <pp/io/CppSeqFileRecordWriter.h>
#include <pp/io/FileInputFormat.h>

class DummyBlock2
{
public:
  int v;

  virtual void readFields(char* byteArray, long)
  {
    int* a = (int*)byteArray;
    v = a[0];
  }

  signed char* writeFields(size_t* len) const
  {
    *len = sizeof(int);
    int* result = new int[1];
    result[0] = v;
    return (signed char*)result;
  }
};

class CppSeqFileRecordWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CppSeqFileRecordWriterTest);
  CPPUNIT_TEST(testWriteRead);
  CPPUNIT_TEST_SUITE_END();

public:
  vector<bool> _foundKey, _foundValue;

  void writeTestData(string fn, int offset)
  {
    Hdfs fs;

    auto_ptr<ostream> out(fs.create(fn));
    CppSeqFile::Writer writer(*out);

    int count = 100;
    DummyBlock2 i1, i2;
    int c = offset;
    for (int i = 0; i < count; i++)
    {
      i1.v = c++;
      i2.v = c++;
      writer.append<DummyBlock2, DummyBlock2>(i1, i2);
    }
    writer.close();
    out.reset();
  }

  void testWriteRead()
  {
    Hdfs fs;

    _foundKey.resize(400);
    _foundValue.resize(400);
    for (size_t i = 0; i < _foundKey.size(); i++)
    {
      _foundKey[i] = false;
      _foundValue[i] = false;
    }
    string inDir = "test-output/CppSeqFileRecordWriterTest/input/";
    string outDir = "test-output/CppSeqFileRecordWriterTest/output/";
    if (fs.exists(inDir))
    {
      fs.deletePath(inDir, true);
    }
    fs.mkdirs(inDir);
    writeTestData(inDir + "in1.csq", 0);
    writeTestData(inDir + "in2.csq", 200);

    // create a job
    Job job;

    // set the name
    job.setName("CppSeqFileRecordWriterTest");

    // set the input/output
    job.setInput(inDir);
    job.setOutput(outDir);

    job.setNumReduceTask(2);

    job.setInputFormatClass(FileInputFormat::className());
    job.setRecordReaderClass(CppSeqFileRecordReader::className());
    job.setRecordWriterClass(CppSeqFileRecordWriter::className());

    // Adds all libraries in this directory to the job.
    job.addLibraryDir(getenv("HADOOP_HOME") + string("/c++/Linux-amd64-64/lib/"));
    job.addLibraryDir(getenv("PRETTY_PIPES_HOME") + string("/lib/"));

    // This library will be used to provide mapper/reducer classes and anything else referenced
    // by the factory.
    job.addPlugin(getenv("PRETTY_PIPES_HOME") + string("/lib/libPrettyPipes.so"));

    // run the job.
    job.setJobTracker("local");
    job.run();

    FileInputFormat fif;
    Configuration conf;
    conf.set("mapred.input.dir", outDir);
    fif.setConfiguration(conf);

    CPPUNIT_ASSERT_EQUAL(2, fif.getSplitCount());
    verifyOutput(fif, 0);
    verifyOutput(fif, 1);

    for (size_t i = 0; i < _foundKey.size(); i++)
    {
      if (i % 2 == 0)
      {
        CPPUNIT_ASSERT_EQUAL(true, (bool)_foundKey[i]);
        CPPUNIT_ASSERT_EQUAL(false, (bool)_foundValue[i]);
      }
      else
      {
        CPPUNIT_ASSERT_EQUAL(false, (bool)_foundKey[i]);
        CPPUNIT_ASSERT_EQUAL(true, (bool)_foundValue[i]);
      }
    }
  }

  void verifyOutput(FileInputFormat& fif, int split)
  {
    CppSeqFileRecordReader reader;
    string splitBytes;
    fif.getSplit(split).writeFields(splitBytes);
    stringstream ss(stringstream::out);
    DataOutputStream dos(ss);
    dos.writeUTF("");
    dos.writeUTF(QString::fromStdString(FileInputSplit::className()));
    dos.writeInt(splitBytes.size());
    dos.write(splitBytes.data(), splitBytes.size());
    ss.flush();
    reader.readInputSplit(ss.str());

    string key, value;
    DummyBlock2 k, v;
    while (reader.next(key, value))
    {
      CPPUNIT_ASSERT_EQUAL(4, (int)key.size());
      CPPUNIT_ASSERT_EQUAL(4, (int)value.size());
      k.readFields((char*)key.data(), key.size());
      v.readFields((char*)value.data(), value.size());
      CPPUNIT_ASSERT(k.v >= 0 && k.v <= (int)_foundKey.size());
      CPPUNIT_ASSERT(v.v >= 0 && v.v <= (int)_foundKey.size());
      _foundKey[k.v] = true;
      _foundValue[v.v] = true;
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CppSeqFileRecordWriterTest, "glacial");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CppSeqFileRecordWriterTest, "current");
