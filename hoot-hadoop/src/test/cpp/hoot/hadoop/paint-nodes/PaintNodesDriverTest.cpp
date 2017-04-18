/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

// Pretty Pipes
#include <pp/io/CppSeqFileRecordWriter.h>
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
using namespace pp;

// Qt
#include <QDir>

// Standard
#include <iostream>
#include <stdlib.h>

#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/TestUtils.h>
#include <hoot/hadoop/pbf/PbfInputFormat.h>
#include <hoot/hadoop/pbf/PbfRecordReader.h>
#include <hoot/hadoop/paint-nodes/PaintNodesDriver.h>

#include "../MapReduceTestFixture.h"

namespace hoot
{

class PaintNodesDriverTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(PaintNodesDriverTest);
  CPPUNIT_TEST(testJob);
  CPPUNIT_TEST_SUITE_END();

public:

  void testJob()
  {
    LOG_INFO("Starting.");
    string outDir = "test-output/hadoop/PaintNodesDriverTest/";

    Hdfs fs;
    if (fs.exists(outDir))
    {
      fs.deletePath(outDir, true);
    }
    fs.copyFromLocal("test-files/io/SmallSplits.pbf", outDir + "input/SmallSplits.pbf");

    Envelope e(-180, 180, -90, 90);
    double pixelSize = 0.1;

    {
      PaintNodesDriver uut;
      const cv::Mat& m = uut.calculateDensity(e, pixelSize, QString::fromStdString(outDir) + "input");

      CPPUNIT_ASSERT_EQUAL(3601, m.cols);
      CPPUNIT_ASSERT_EQUAL(1801, m.rows);
      //CPPUNIT_ASSERT_EQUAL(9, m.ptr<int>(1288)[750]);
      //CPPUNIT_ASSERT_EQUAL(27, m.ptr<int>(1288)[751]);
      long sum = 0;
      for (int py = 0; py < m.rows; py++)
      {
        const int32_t* row1 = m.ptr<int32_t>(py);

        for (int px = 0; px < m.cols; px++)
        {
          sum += row1[px];
          if (row1[px] > 0)
          {
            LOG_INFO("px: " << px << " py: " << py << " v: " << row1[px]);
          }
        }
      }
      CPPUNIT_ASSERT_EQUAL(36l, sum);
    }

    // does it work when cached?
    {
      PaintNodesDriver uut;
      const cv::Mat& m = uut.calculateDensity(e, pixelSize, QString::fromStdString(outDir) + "input");

      CPPUNIT_ASSERT_EQUAL(3601, m.cols);
      CPPUNIT_ASSERT_EQUAL(1801, m.rows);
      //CPPUNIT_ASSERT_EQUAL(9, m.ptr<int>(1288)[750]);
      //CPPUNIT_ASSERT_EQUAL(27, m.ptr<int>(1288)[751]);
      long sum = 0;
      for (int py = 0; py < m.rows; py++)
      {
        const int32_t* row1 = m.ptr<int32_t>(py);

        for (int px = 0; px < m.cols; px++)
        {
          sum += row1[px];
          if (row1[px] > 0)
          {
            LOG_INFO("px: " << px << " py: " << py << " v: " << row1[px]);
          }
        }
      }
      CPPUNIT_ASSERT_EQUAL(36l, sum);
    }
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::PaintNodesDriverTest, "glacial");

