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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/DataConverter.h>
#include <hoot/core/util/ConfigOptions.h>

#include "../TestUtils.h"

namespace hoot
{

class DataConverterTest : public CppUnit::TestFixture
{
  //just testing input validation here, as the command line tests get the rest
  CPPUNIT_TEST_SUITE(DataConverterTest);
  CPPUNIT_TEST(runEmptyInputsTest);
  CPPUNIT_TEST(runEmptyOutputTest);
  CPPUNIT_TEST(runColumnsNotOsmToShpTest1);
  CPPUNIT_TEST(runColumnsNotOsmToShpTest2);
  CPPUNIT_TEST(runBothTranslationAndColumnsTest);
  CPPUNIT_TEST(runFeatureLimitNonOgrInputsTest);
  CPPUNIT_TEST(runSpecifyTranslationFromCommandLineAndInConvertOptionTest);
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
      converter.setColumns(cols);
      converter.setColsArgSpecified(true);
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
      converter.setColumns(cols);
      converter.setColsArgSpecified(true);
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
      converter.setTranslation("MyTranslation.js");
      QStringList cols;
      cols.append("Test1");
      cols.append("Test2");
      converter.setColumns(cols);
      converter.setColsArgSpecified(true);
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
      converter.setFeatureReadLimit(2);
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

  void runSpecifyTranslationFromCommandLineAndInConvertOptionTest()
  {
    QString exceptionMsg("");

    try
    {
      DataConverter converter;
      converter.setTranslation("MyTranslation.js");

      QStringList convertOps;
      convertOps.append("hoot::TranslationOp");
      Settings conf;
      conf.set(ConfigOptions().getConvertOpsKey(), convertOps);
      converter.setConfiguration(conf);

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
        "Cannot specify both a translation as an input parameter as a configuration option"));
    exceptionMsg = "";

    try
    {
      DataConverter converter;
      converter.setTranslation("MyTranslation.js");

      QStringList convertOps;
      convertOps.append("hoot::TranslationVisitor");
      Settings conf;
      conf.set(ConfigOptions().getConvertOpsKey(), convertOps);
      converter.setConfiguration(conf);

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
        "Cannot specify both a translation as an input parameter as a configuration option"));
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataConverterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataConverterTest, "quick");

}
