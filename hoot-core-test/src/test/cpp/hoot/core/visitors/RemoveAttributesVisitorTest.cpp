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
 * @copyright Copyright (C) 2014, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/elements/ElementAttributeType.h>
#include <hoot/core/visitors/RemoveAttributesVisitor.h>

namespace hoot
{

class RemoveAttributesVisitorTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(RemoveAttributesVisitorTest);
  CPPUNIT_TEST(runRemoveAttributesTest);
  CPPUNIT_TEST(runRemoveInvalidAttributeKeyTest);
  CPPUNIT_TEST_SUITE_END();

public:

  RemoveAttributesVisitorTest()
    : HootTestFixture("test-files/visitors/RemoveAttributesVisitorTest/",
                      "test-output/visitors/RemoveAttributesVisitorTest/")
  {
    setResetType(ResetBasic);
  }

  void runRemoveAttributesTest()
  {
    OsmMapPtr map(new OsmMap());
    OsmMapReaderFactory::read(
      map, _inputPath + "RemoveAttributesVisitorTest.osm", false, Status::Unknown1);

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
    writer.write(map, _outputPath + "RunRemoveAttributesTest.osm");

    HOOT_FILE_EQUALS( _inputPath + "RunRemoveAttributesTest.osm",
                     _outputPath + "RunRemoveAttributesTest.osm");
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


