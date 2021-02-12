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
 * @copyright Copyright (C) 2014, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/visitors/AddAttributesVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>

namespace hoot
{

class AddAttributesVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(AddAttributesVisitorTest);
  CPPUNIT_TEST(runAddAttributesTest);
  CPPUNIT_TEST(runAddAttributesOnlyIfEmptyTest1);
  CPPUNIT_TEST(runAddAttributesOnlyIfEmptyTest2);
  CPPUNIT_TEST(runAddInvalidAttributeKeyTest);
  CPPUNIT_TEST(runAddMissingAttributeValueTest);
  CPPUNIT_TEST(runAddEmptyAttributeValueTest);
  CPPUNIT_TEST(runAddInvalidValueTest);
  CPPUNIT_TEST(runNegatedFilterTest);
  CPPUNIT_TEST(runMultipleCriteriaTest);
  // TODO (will need to modify the input file):
  //CPPUNIT_TEST(runChainCriteriaTest);
  CPPUNIT_TEST_SUITE_END();

public:

  AddAttributesVisitorTest()
    : HootTestFixture("test-files/visitors/AddAttributesVisitorTest/",
                      "test-output/visitors/AddAttributesVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runAddAttributesTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "AddAttributesVisitorTest.osm", false, Status::Unknown1);

    QStringList attributesToAdd;
    attributesToAdd.append("version=1");
    attributesToAdd.append("timestamp=2016-05-04T22:07:19Z");
    attributesToAdd.append("changeset=39108451");
    attributesToAdd.append("uid=550560");
    attributesToAdd.append("user=Seandebasti");
    AddAttributesVisitor visitor(attributesToAdd);
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunAddAttributesTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunAddAttributesTest.osm",
                     _outputPath + "RunAddAttributesTest.osm");
  }

  void runAddAttributesOnlyIfEmptyTest1()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "AddAttributesVisitorTest2.osm", false, Status::Unknown1);

    QStringList attributesToAdd;
    attributesToAdd.append("version=1");
    attributesToAdd.append("timestamp=2016-05-04T22:07:19Z");
    attributesToAdd.append("changeset=39108451");
    attributesToAdd.append("uid=550560");
    attributesToAdd.append("user=Seandebasti");
    AddAttributesVisitor visitor(attributesToAdd);
    visitor.setAddOnlyIfEmpty(true);
    //All of the attributes should be modified b/c we told the visitor to only modify empty
    //attributes, and all of the attributes in the input file are empty.
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunAddAttributesOnlyIfEmptyTest1.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunAddAttributesOnlyIfEmptyTest1.osm",
                     _outputPath + "RunAddAttributesOnlyIfEmptyTest1.osm");
  }

  void runAddAttributesOnlyIfEmptyTest2()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "AddAttributesVisitorTest3.osm", false, Status::Unknown1);

    QStringList attributesToAdd;
    attributesToAdd.append("version=1");
    attributesToAdd.append("timestamp=2016-05-04T22:07:19Z");
    attributesToAdd.append("changeset=39108451");
    attributesToAdd.append("uid=550560");
    attributesToAdd.append("user=Seandebasti");
    AddAttributesVisitor visitor(attributesToAdd);
    visitor.setAddOnlyIfEmpty(true);
    //None of the attributes should be modified b/c we told the visitor to only modify empty
    //attributes, and none of the attributes in the input file are empty.
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "RunAddAttributesOnlyIfEmptyTest2.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunAddAttributesOnlyIfEmptyTest2.osm",
                     _outputPath + "RunAddAttributesOnlyIfEmptyTest2.osm");
  }

  void runAddInvalidAttributeKeyTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "AddAttributesVisitorTest.osm", false, Status::Unknown1);

    QStringList attributesToAdd;
    attributesToAdd.append("blah=1");
    AddAttributesVisitor visitor;

    QString exceptionMsg("");
    visitor.setAttributes(attributesToAdd);
    try
    {
      map->visitRw(visitor);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid element attribute type string"));
  }

  void runAddMissingAttributeValueTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "AddAttributesVisitorTest.osm", false, Status::Unknown1);

    QStringList attributesToAdd;
    attributesToAdd.append("blah");
    AddAttributesVisitor visitor;

    QString exceptionMsg("");
    visitor.setAttributes(attributesToAdd);
    try
    {
      map->visitRw(visitor);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid attribute: blah"));
  }

  void runAddEmptyAttributeValueTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "AddAttributesVisitorTest.osm", false, Status::Unknown1);

    QStringList attributesToAdd;
    attributesToAdd.append("blah= ");
    AddAttributesVisitor visitor;

    QString exceptionMsg("");
    visitor.setAttributes(attributesToAdd);
    try
    {
      map->visitRw(visitor);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid empty attribute"));
  }

  void runAddInvalidValueTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "AddAttributesVisitorTest.osm", false, Status::Unknown1);

    AddAttributesVisitor visitor;
    QStringList attributesToAdd;
    QString exceptionMsg("");

    attributesToAdd.append("version=blah");
    visitor.setAttributes(attributesToAdd);
    try
    {
      map->visitRw(visitor);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid attribute value"));
    attributesToAdd.clear();
    exceptionMsg = "";

    attributesToAdd.append("timestamp=blah");
    visitor.setAttributes(attributesToAdd);
    try
    {
      map->visitRw(visitor);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    //This is actually testing OsmUtils, not AddAttributesVisitor.
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid timestamp string"));
    attributesToAdd.clear();
    exceptionMsg = "";

    attributesToAdd.append("changeset=blah");
    visitor.setAttributes(attributesToAdd);
    try
    {
      map->visitRw(visitor);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid attribute value"));
    attributesToAdd.clear();
    exceptionMsg = "";

    attributesToAdd.append("uid=blah");
    visitor.setAttributes(attributesToAdd);
    try
    {
      map->visitRw(visitor);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Invalid attribute value"));
    attributesToAdd.clear();
    exceptionMsg = "";
  }

  void runNegatedFilterTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveElementsVisitorTest/RemoveElementsVisitorInput.osm");

    QStringList attributesToAdd;
    attributesToAdd.append("version=1");
    attributesToAdd.append("timestamp=2016-05-04T22:07:19Z");
    attributesToAdd.append("changeset=39108451");
    attributesToAdd.append("uid=550560");
    attributesToAdd.append("user=Seandebasti");
    AddAttributesVisitor visitor(attributesToAdd, true);
    visitor.addCriterion(ElementCriterionPtr(new PoiCriterion()));
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "runNegatedFilterTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "runNegatedFilterTest.osm",
                     _outputPath + "runNegatedFilterTest.osm");
  }

  void runMultipleCriteriaTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveElementsVisitorTest/RemoveElementsVisitorInput.osm");

    QStringList attributesToAdd;
    attributesToAdd.append("version=1");
    attributesToAdd.append("timestamp=2016-05-04T22:07:19Z");
    attributesToAdd.append("changeset=39108451");
    attributesToAdd.append("uid=550560");
    attributesToAdd.append("user=Seandebasti");
    AddAttributesVisitor visitor(attributesToAdd);
    visitor.addCriterion(ElementCriterionPtr(new PoiCriterion()));
    visitor.addCriterion(ElementCriterionPtr(new HighwayCriterion(map)));
    map->visitRw(visitor);

    OsmMapWriterFactory::write(map, _outputPath + "runMultipleCriteriaTest.osm");

    HOOT_FILE_EQUALS(_inputPath + "runMultipleCriteriaTest.osm",
                     _outputPath + "runMultipleCriteriaTest.osm");
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddAttributesVisitorTest, "quick");

}


