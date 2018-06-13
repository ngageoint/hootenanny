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
 * @copyright Copyright (C) 2014, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/AddAttributesVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

#include "../TestUtils.h"

// Qt
#include <QDir>

namespace hoot
{

class AddAttributesVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(AddAttributesVisitorTest);
  CPPUNIT_TEST(runAddAttributesTest);
  CPPUNIT_TEST(runAddAttributesOnlyIfEmptyTest1);
  CPPUNIT_TEST(runAddAttributesOnlyIfEmptyTest2);
  CPPUNIT_TEST(runAddInvalidAttributeKeyTest);
  CPPUNIT_TEST(runAddMissingAttributeValueTest);
  CPPUNIT_TEST(runAddEmptyAttributeValueTest);
  CPPUNIT_TEST(runAddInvalidValueTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/visitors");
  }

  void runAddAttributesTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/AddAttributesVisitorTest.osm", false, Status::Unknown1);

    QStringList attributesToAdd;
    attributesToAdd.append("version=1");
    attributesToAdd.append("timestamp=2016-05-04T22:07:19Z");
    attributesToAdd.append("changeset=39108451");
    attributesToAdd.append("uid=550560");
    attributesToAdd.append("user=Seandebasti");
    AddAttributesVisitor visitor(attributesToAdd);
    map->visitRw(visitor);

    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/AddAttributesVisitorTest-runAddAttributesTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/AddAttributesVisitorTest-runAddAttributesTest.osm",
      "test-output/visitors/AddAttributesVisitorTest-runAddAttributesTest.osm");
  }

  void runAddAttributesOnlyIfEmptyTest1()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/AddAttributesVisitorTest2.osm", false, Status::Unknown1);

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

    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/AddAttributesVisitorTest-runAddAttributesOnlyIfEmptyTest1.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/AddAttributesVisitorTest-runAddAttributesOnlyIfEmptyTest1.osm",
      "test-output/visitors/AddAttributesVisitorTest-runAddAttributesOnlyIfEmptyTest1.osm");
  }

  void runAddAttributesOnlyIfEmptyTest2()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/AddAttributesVisitorTest3.osm", false, Status::Unknown1);

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

    OsmMapWriterFactory::getInstance().write(map,
      "test-output/visitors/AddAttributesVisitorTest-runAddAttributesOnlyIfEmptyTest2.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/AddAttributesVisitorTest-runAddAttributesOnlyIfEmptyTest2.osm",
      "test-output/visitors/AddAttributesVisitorTest-runAddAttributesOnlyIfEmptyTest2.osm");
  }

  void runAddInvalidAttributeKeyTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/AddAttributesVisitorTest.osm", false, Status::Unknown1);

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
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/AddAttributesVisitorTest.osm", false, Status::Unknown1);

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
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/AddAttributesVisitorTest.osm", false, Status::Unknown1);

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
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/AddAttributesVisitorTest.osm", false, Status::Unknown1);

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
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddAttributesVisitorTest, "quick");

}


