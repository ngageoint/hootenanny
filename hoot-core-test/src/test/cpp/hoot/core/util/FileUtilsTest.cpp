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
 * @copyright Copyright (C) 2013, 2014, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>

//  Qt
#include <QFileInfo>

namespace hoot
{

class FileUtilsTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(FileUtilsTest);
  CPPUNIT_TEST(runFilesTest);
  CPPUNIT_TEST(runLogFormatTest);
  CPPUNIT_TEST_SUITE_END();

public:

  FileUtilsTest()
    : HootTestFixture(UNUSED_PATH,
                      "test-output/util/FileUtilsTest/")
  {
  }

  void runFilesTest()
  {
    QString test_directory = _outputPath + "test-directory/";
    CPPUNIT_ASSERT(FileUtils::makeDir(test_directory));
    QFileInfo test_directory_exists(test_directory);
    CPPUNIT_ASSERT(test_directory_exists.exists() && test_directory_exists.isDir());

    QString output_file = test_directory + "test-file.txt";

    QStringList file_lines;
    file_lines.append("1");
    file_lines.append("2");
    file_lines.append("3");
    file_lines.append("4");
    file_lines.append("5");

    QString file_content = file_lines.join("\n");

    FileUtils::writeFully(output_file, file_content);
    QString read_content = FileUtils::readFully(output_file);
    HOOT_STR_EQUALS(file_content, read_content);

    CPPUNIT_ASSERT_EQUAL(5L, FileUtils::getNumberOfLinesInFile(output_file));

    CPPUNIT_ASSERT(FileUtils::dirContainsFileWithExtension(test_directory, "txt"));
    CPPUNIT_ASSERT(!FileUtils::dirContainsFileWithExtension(test_directory, "osm"));

    QStringList lines = FileUtils::readFileToList(output_file);
    QString joined_lines = lines.join("\n");

    HOOT_STR_EQUALS(file_content, joined_lines);

    FileUtils::removeDir(test_directory);
    QFileInfo test_directory_gone(test_directory);
    CPPUNIT_ASSERT(!test_directory_gone.exists());
  }

  void runLogFormatTest()
  {
    QStringList input;
    input.append("$HOOT_HOME/test-files/ToyTestA.osm");
    input.append("$HOOT_HOME/test-files/cmd/glacial/NonDestructiveTest/Expected.osm");
    input.append("https://test:test@hootenanny.com/api/");
    input.append("https://192.168.0.1/api/");
    input.append("https://hootenanny.com:8080/api/");

    QStringList output;
    output.append("$HOOT_HOME/test-files/ToyTestA.osm");
    output.append("$HOOT_HOME/test-files/cmd/glacial/NonDestructiveTest/Expected.osm");
    output.append("https://hootenanny.com/api/");
    output.append("https://<host-ip>/api/");
    output.append("https://hootenanny.com/api/");

    HOOT_STR_EQUALS(output[0], FileUtils::toLogFormat(input[0]));
    HOOT_STR_EQUALS(output[1].right(12), FileUtils::toLogFormat(input[1], 12));
    HOOT_STR_EQUALS(output[2], FileUtils::toLogFormat(input[2]));
    HOOT_STR_EQUALS(output[3], FileUtils::toLogFormat(input[3]));
    HOOT_STR_EQUALS(output[4], FileUtils::toLogFormat(input[4]));

    HOOT_STR_EQUALS(output.join(", "), FileUtils::toLogFormat(input));
    HOOT_STR_EQUALS(output.join(", ").right(128), FileUtils::toLogFormat(input, 128));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FileUtilsTest, "quick");

}

