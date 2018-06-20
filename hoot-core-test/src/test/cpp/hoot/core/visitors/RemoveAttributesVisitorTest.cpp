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
#include <hoot/core/visitors/RemoveAttributesVisitor.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/elements/ElementAttributeType.h>

#include "../TestUtils.h"

// Qt
#include <QDir>

namespace hoot
{

class RemoveAttributesVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(RemoveAttributesVisitorTest);
  CPPUNIT_TEST(runRemoveAttributesTest);
  CPPUNIT_TEST(runRemoveInvalidAttributeKeyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void setUp()
  {
    TestUtils::mkpath("test-output/visitors");
  }

  void runRemoveAttributesTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMap::resetCounters();
    OsmMapReaderFactory::read(
      map, "test-files/visitors/RemoveAttributesVisitorTest.osm", false, Status::Unknown1);

    QStringList attributesToRemove;
    attributesToRemove.append("version");
    attributesToRemove.append("timestamp");
    attributesToRemove.append("changeset");
    attributesToRemove.append("uid");
    attributesToRemove.append("user");
    RemoveAttributesVisitor visitor(attributesToRemove);
    map->visitRw(visitor);

    OsmXmlWriter writer;
    writer.setIncludeCompatibilityTags(false);
    writer.write(
      map, "test-output/visitors/RemoveAttributesVisitorTest-runRemoveAttributesTest.osm");

    HOOT_FILE_EQUALS(
      "test-files/visitors/RemoveAttributesVisitorTest-runRemoveAttributesTest.osm",
      "test-output/visitors/RemoveAttributesVisitorTest-runRemoveAttributesTest.osm");
  }

  void runRemoveInvalidAttributeKeyTest()
  {
    QStringList attributesToRemove;
    attributesToRemove.append("blah");
    RemoveAttributesVisitor visitor;

    QString exceptionMsg("");
    try
    {
      visitor.setTypes(attributesToRemove);
    }
    catch (const IllegalArgumentException& e)
    {
      exceptionMsg = e.what();
    }
    LOG_VART(exceptionMsg);
    CPPUNIT_ASSERT(
      exceptionMsg.contains("Invalid element attribute type string"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(RemoveAttributesVisitorTest, "quick");

}


