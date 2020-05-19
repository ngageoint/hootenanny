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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/DataConverter.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QFileInfo>

namespace hoot
{

class DataConverterTest : public HootTestFixture
{
  // just testing input validation here, as the command line tests get the rest
  CPPUNIT_TEST_SUITE(DataConverterTest);
  CPPUNIT_TEST(runEmptyInputsTest);
  CPPUNIT_TEST(runEmptyOutputTest);
  CPPUNIT_TEST(runColumnsNotOsmToShpTest1);
  CPPUNIT_TEST(runColumnsNotOsmToShpTest2);
  CPPUNIT_TEST(runBothTranslationAndColumnsTest);
  CPPUNIT_TEST(runFeatureLimitNonOgrInputsTest);
  CPPUNIT_TEST(runTranslationFileDoesntExistTest);
  CPPUNIT_TEST(runInvalidTranslationFileFormatTest);
  CPPUNIT_TEST(runUrlOutputTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runEmptyInputsTest()
  {
    QString exceptionMsg("");
    try
    {
      QStringList inputs;
      DataConverter().convert(inputs, "test.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(exceptionMsg.contains("No input(s) specified"));
  }

  void runEmptyOutputTest()
  {
    QString exceptionMsg("");
    try
    {
      QStringList inputs;
      inputs.append("test.shp");
      DataConverter().convert(inputs, "");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(exceptionMsg.contains("No output specified"));
  }

  void runColumnsNotOsmToShpTest1()
  {
    QString exceptionMsg("");
    try
    {
      DataConverter converter;
      QStringList cols;
      cols.append("Test1");
      cols.append("Test2");
      converter.setShapeFileColumns(cols);
      QStringList inputs;
      inputs.append("test1.shp");
      converter.convert(inputs, "test2.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(
      exceptionMsg.contains(
        "Columns may only be specified when converting to the shape file format"));
  }

  void runColumnsNotOsmToShpTest2()
  {
    QString exceptionMsg("");
    try
    {
      DataConverter converter;
      QStringList cols;
      cols.append("Test1");
      cols.append("Test2");
      converter.setShapeFileColumns(cols);

      QStringList inputs;
      inputs.append("test1.osm");
      converter.convert(inputs, "test2.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(
      exceptionMsg.contains(
        "Columns may only be specified when converting to the shape file format"));
  }

  void runBothTranslationAndColumnsTest()
  {
    QString exceptionMsg("");
    try
    {
      DataConverter converter;
      // any file with a supported translation file that's known to exist
      converter.setTranslation("translations/GeoNames.js");
      QStringList cols;
      cols.append("Test1");
      cols.append("Test2");
      converter.setShapeFileColumns(cols);
      QStringList inputs;
      inputs.append("test1.shp");
      converter.convert(inputs, "test2.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);

    CPPUNIT_ASSERT(exceptionMsg.contains("Cannot specify both a translation and export columns"));
  }

  void runFeatureLimitNonOgrInputsTest()
  {
    QString exceptionMsg("");
    try
    {
      DataConverter converter;
      converter.setOgrFeatureReadLimit(2);
      QStringList inputs;
      inputs.append("test1.osm");
      converter.convert(inputs, "test2.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(
      exceptionMsg.contains("Read limit may only be specified when converting OGR inputs"));
  }

  void runTranslationFileDoesntExistTest()
  {
    QString exceptionMsg("");
    try
    {
      DataConverter converter;
      converter.setTranslation("MyTranslation.js");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);

    CPPUNIT_ASSERT(exceptionMsg.contains("Translation file does not exist: MyTranslation.js"));
  }

  void runInvalidTranslationFileFormatTest()
  {
    QString exceptionMsg("");
    try
    {
      DataConverter converter;
      converter.setTranslation("test-files/DcGisRoads.osm");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);

    CPPUNIT_ASSERT(exceptionMsg.contains(
      "Invalid translation file format: test-files/DcGisRoads.osm"));
  }

  void runUrlOutputTest()
  {
    // just want to make sure an output dir isn't created for a url out

    const QString badPath = "osmapidb:";
    QDir(badPath).removeRecursively();

    DataConverter uut;
    const QString url = "osmapidb://user:password@postgres:5432";
    LOG_VART(url);
    uut._validateInput(QStringList("input1.osm"), url);
    QFileInfo outputInfo(badPath);
    CPPUNIT_ASSERT(!outputInfo.exists());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataConverterTest, "quick");

}
