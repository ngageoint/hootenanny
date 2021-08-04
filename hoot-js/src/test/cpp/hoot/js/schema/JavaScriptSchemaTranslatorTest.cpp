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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/ScriptSchemaTranslator.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/schema/ScriptSchemaTranslatorFactory.h>
#include <hoot/core/io/schema/Feature.h>
#include <hoot/core/io/schema/FeatureDefinition.h>
#include <hoot/core/io/schema/FieldDefinition.h>
#include <hoot/core/io/schema/Schema.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;

namespace hoot
{

class JavaScriptSchemaTranslatorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(JavaScriptSchemaTranslatorTest);
  CPPUNIT_TEST(runSchemaTest);
  CPPUNIT_TEST(runToOsmTest);
  CPPUNIT_TEST(runLayerNameFilterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToOsmTest()
  {
    // Great bit of code taken from TranslatedTagDifferencer.cpp
    std::shared_ptr<ScriptSchemaTranslator> st =
      ScriptSchemaTranslatorFactory::getInstance().createTranslator(
        "test-files/io/SampleTranslation.js");

    std::shared_ptr<ScriptToOgrSchemaTranslator> uut =
      std::dynamic_pointer_cast<ScriptToOgrSchemaTranslator>(st);

    if (!uut)
    {
      throw HootException("Error allocating translator. the translation script must support "
                          "converting to OGR.");
    }

    std::shared_ptr<const Schema> schema = uut->getOgrOutputSchema();

    ScriptToOgrSchemaTranslator::TranslatedFeature tf;
    QString layer;

    Tags t;

    t.clear();
    t["building"] = "yes";
    t["name"] = "foo";
    tf = uut->translateToOgr(t, ElementType::Node, GEOS_POINT)[0];
    HOOT_STR_EQUALS("PAL015", tf.tableName);
    HOOT_STR_EQUALS("[2]{(ARA, -999999), (NAM, foo)}", tf.feature->getValues());

    t.clear();
    t["building"] = "yes";
    t["name"] = "foo";
    tf = uut->translateToOgr(t, ElementType::Way, GEOS_POLYGON)[0];
    HOOT_STR_EQUALS("AAL015", tf.tableName);
    HOOT_STR_EQUALS("[2]{(ARA, 10), (NAM, foo)}", tf.feature->getValues());

    t.clear();
    t["highway"] = "track";
    t["name"] = "bar";
    tf = uut->translateToOgr(t, ElementType::Way, GEOS_LINESTRING)[0];
    HOOT_STR_EQUALS("LAP010", tf.tableName);
    HOOT_STR_EQUALS("[3]{(ARA, -999999), (NAM, bar), (PCF, 1)}", tf.feature->getValues());

    t.clear();
    t["highway"] = "road";
    t["name"] = "bar";
    tf = uut->translateToOgr(t, ElementType::Way, GEOS_LINESTRING)[0];
    HOOT_STR_EQUALS("LAP030", tf.tableName);
    HOOT_STR_EQUALS("[4]{(ARA, -999999), (LTN, 2), (NAM, bar), (PCF, 2)}", tf.feature->getValues());


    // throw an exception because the BAD field shouldn't be there.
    t.clear();
    t["tableName"] = "PAL015";
    t["NAM"] = "foo";
    t["BAD"] = "tag";
    t["ARA"] = "-999999";
    CPPUNIT_ASSERT_THROW(uut->translateToOgr(t, ElementType::Node, GEOS_POINT),
                         FieldDefinition::InvalidValueException);

    // throw an exception because ARA can't be -1.
    t.clear();
    t["tableName"] = "PAL015";
    t["NAM"] = "foo";
    t["ARA"] = "-1";
    CPPUNIT_ASSERT_THROW(uut->translateToOgr(t, ElementType::Node, GEOS_POINT),
                         FieldDefinition::InvalidValueException);

    // throw an exception because the NAM field is missing.
    t.clear();
    t["tableName"] = "PAL015";
    t["ARA"] = "-999999";
    CPPUNIT_ASSERT_THROW(uut->translateToOgr(t, ElementType::Node, GEOS_POINT),
                         FieldDefinition::InvalidValueException);

    // throw an exception because the PCF field has an invalid value.
    t.clear();
    t["tableName"] = "LAP030";
    t["ARA"] = "-999999";
    t["NAM"] = "foo";
    t["LTN"] = "2";
    t["PCF"] = "3";
    CPPUNIT_ASSERT_THROW(uut->translateToOgr(t, ElementType::Way, GEOS_LINESTRING),
                         FieldDefinition::InvalidValueException);
  }

  void runLayerNameFilterTest()
  {
    // Great bit of code taken from TranslatedTagDifferencer.cpp
    // We just need a standard ScriptSchemaTranslator for this test.
    std::shared_ptr<ScriptSchemaTranslator> uut =
      ScriptSchemaTranslatorFactory::getInstance().createTranslator(
        "test-files/io/SampleTranslation.js");

    if (!uut)
    {
      throw HootException("Error allocating translator.");
    }

    // Check that we can access a value from a Javascript function
    HOOT_STR_EQUALS("Papa Smurf", uut->getLayerNameFilter());
  }

  void runSchemaTest()
  {
    // Great bit of code taken from TranslatedTagDifferencer.cpp
    std::shared_ptr<ScriptSchemaTranslator> st =
      ScriptSchemaTranslatorFactory::getInstance().createTranslator(
        "test-files/io/SampleTranslation.js");

    std::shared_ptr<ScriptToOgrSchemaTranslator>uut =
      std::dynamic_pointer_cast<ScriptToOgrSchemaTranslator>(st);

    if (!uut)
    {
      throw HootException("Error allocating translator");
    }

    std::shared_ptr<const Schema> schema = uut->getOgrOutputSchema();

    QString result;
    // handy for creating the c formatted version.
    QString sep1, sep2;
    for (size_t i = 0; i < schema->getLayerCount(); i++)
    {
      std::shared_ptr<const Layer> l = schema->getLayer(i);
      result += sep1 + l->getName() + sep2 + "\n";
      std::shared_ptr<const FeatureDefinition> fd = l->getFeatureDefinition();
      for (size_t j = 0; j < fd->getFieldCount(); j++)
      {
        std::shared_ptr<const FieldDefinition> f = fd->getFieldDefinition(j);
        result += sep1 + "  " + f->toString() + sep2 + "\n";
      }
    }

    QString expected = "AAL015\n"
        "  String name: NAM default: \n"
        "  Double name: ARA min: 0 max: 1.79769e+308 default: -1.79769e+308 enumerations: [1]{-999999}\n"
        "LAP010\n"
        "  String name: NAM default: \n"
        "  Double name: ARA min: 0 max: 1.79769e+308 default: -1.79769e+308 enumerations: [1]{-999999}\n"
        "  Int name: PCF min: -2147483647 max: 2147483647 default: -999999 enumerations: [3]{-999999, 1, 2}\n"
        "LAP020\n"
        "  String name: NAM default: \n"
        "  Double name: ARA min: 0 max: 1.79769e+308 default: -1.79769e+308 enumerations: [1]{-999999}\n"
        "  Int name: PCF min: -2147483647 max: 2147483647 default: -999999 enumerations: [3]{-999999, 1, 2}\n"
        "  Int name: LTN min: 1 max: 99 default: 1 enumerations: [0]{}\n"
        "LAP030\n"
        "  String name: NAM default: \n"
        "  Double name: ARA min: 0 max: 1.79769e+308 default: -1.79769e+308 enumerations: [1]{-999999}\n"
        "  Int name: PCF min: -2147483647 max: 2147483647 default: -999999 enumerations: [3]{-999999, 1, 2}\n"
        "  Int name: LTN min: 1 max: 99 default: 1 enumerations: [0]{}\n"
        "PAL015\n"
        "  String name: NAM default: \n"
        "  Double name: ARA min: 0 max: 1.79769e+308 default: -1.79769e+308 enumerations: [1]{-999999}\n";

    CPPUNIT_ASSERT_EQUAL(expected.toStdString(), result.toStdString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(JavaScriptSchemaTranslatorTest, "quick");

}
