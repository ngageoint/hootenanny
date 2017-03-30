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
#include <pp/Hdfs.h>
#include <pp/mapreduce/Job.h>
using namespace pp;

// Qt
#include <QDir>

#include <hoot/core/TestUtils.h>
#include <hoot/hadoop/convert/WriteOsmSqlStatementsDriver.h>

#include "../MapReduceTestFixture.h"

namespace hoot
{

class WriteOsmSqlStatementsDriverTest : public MapReduceTestFixture
{
  CPPUNIT_TEST_SUITE(WriteOsmSqlStatementsDriverTest);
  CPPUNIT_TEST(testJob);
  CPPUNIT_TEST_SUITE_END();

public:

  void testJob()
  {
    const string outDir = "test-output/hadoop/convert/WriteOsmSqlStatementsDriverTest";
    const QString outFile = QString::fromStdString(outDir) + "/output.sql";
    if (QFile::exists(outFile))
    {
      QFile::remove(outFile);
    }
    Hdfs fs;
    if (fs.exists(outDir))
    {
      fs.deletePath(outDir, true);
    }
    QDir().mkpath(QString::fromStdString(outDir));
    fs.copyFromLocal(
      "test-files/conflate/unified/AllDataTypesA.osm.pbf"/*"test-files/DcGisRoads.pbf"*/,
      outDir + "/input.osm.pbf");

    WriteOsmSqlStatementsDriver().write(QString::fromStdString(outDir) + "/input.osm.pbf", outFile);

    //TODO: verify output
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(hoot::WriteOsmSqlStatementsDriverTest, "glacial");

