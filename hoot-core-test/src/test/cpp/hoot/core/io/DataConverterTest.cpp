
// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/DataConverter.h>

#include "../TestUtils.h"

namespace hoot
{

class DataConverterTest : public CppUnit::TestFixture
{
  //just testing validation here, as the command line tests get the rest
  CPPUNIT_TEST_SUITE(DataConverterTest);
  CPPUNIT_TEST(runEmptyInputsTest);
  CPPUNIT_TEST(runEmptyOutputTest);
  //TODO: re-enable
  //CPPUNIT_TEST(runConvertToSameTypeTest);
  CPPUNIT_TEST(runMultipleNonOgrInputsTest1);
  CPPUNIT_TEST(runMultipleNonOgrInputsTest2);
  CPPUNIT_TEST(runTranslationNonOgrTest);
  CPPUNIT_TEST(runColumnsNotOsmToShpTest1);
  CPPUNIT_TEST(runColumnsNotOsmToShpTest2);
  CPPUNIT_TEST(runBothTranslationAndColumnsTest);
  CPPUNIT_TEST(runFeatureLimitNonOgrInputsTest);
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

  void runConvertToSameTypeTest()
  {
    QString exceptionMsg("");
    try
    {
      QStringList inputs;
      inputs.append("test1.shp");
      DataConverter().convert(inputs, "test2.shp");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(exceptionMsg.contains("Attempting to convert a file to the same file type"));
  }

  void runMultipleNonOgrInputsTest1()
  {
    QString exceptionMsg("");
    try
    {
      QStringList inputs;
      inputs.append("test1.osm");
      inputs.append("test2.osm");
      DataConverter().convert(inputs, "test2.shp");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(exceptionMsg.contains("Multiple inputs are only allowed when converting from an OGR format to OSM"));
  }

  void runMultipleNonOgrInputsTest2()
  {
    QString exceptionMsg("");
    try
    {
      QStringList inputs;
      inputs.append("test1.shp");
      inputs.append("test2.shp");
      DataConverter().convert(inputs, "test2.geojson");
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }

    CPPUNIT_ASSERT(
      exceptionMsg.contains(
        "Multiple inputs are only allowed when converting from an OGR format to OSM"));
  }

  void runTranslationNonOgrTest()
  {
    QString exceptionMsg("");
    try
    {
      DataConverter converter;
      converter.setTranslation("test.js");
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
        "A translation can only be specified when converting to/from OGR formats"));
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
        "Columns may only be specified when converting from an OSM format to the shape file format"));
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
        "Columns may only be specified when converting from an OSM format to the shape file format"));
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
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataConverterTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DataConverterTest, "quick");

}
