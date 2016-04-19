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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/OsmChangesetXmlFileWriter.h>

// Boost
using namespace boost;

// Qt
#include <QDebug>
#include <QFile>
#include <QFileInfo>

#include "../TestUtils.h"

namespace hoot
{

//dummy implementation for testing the OsmChangeWriter
class TestChangesetProvider : public ChangeSetProvider
{

public:

  TestChangesetProvider() : _ctr(0) { }
  ~TestChangesetProvider() { }

  boost::shared_ptr<OGRSpatialReference> getProjection() const
  {
    return boost::shared_ptr<OGRSpatialReference>();
  }

  void close() { }

  bool hasMoreChanges() { while (_ctr < 3) { return true; } return false; }

  Change readNextChange()
  {
    NodePtr node1(new Node(Status::Unknown1, 1, 0.0, 0.0, 15.0));
    NodePtr node2(new Node(Status::Unknown1, 2, 1.0, 0.0, 15.0));
    WayPtr way(new Way(Status::Unknown1, 3, 15.0));
    way->addNode(node1->getId());
    way->addNode(node2->getId());
    way->setTag("key1", "value1");
    RelationPtr relation(new Relation(Status::Unknown1, 1, 15.0));
    relation->addElement("role1", node1->getElementId());
    relation->addElement("role2", way->getElementId());
    relation->setTag("key2", "value2");

    Change change;
    if (_ctr == 0)
    {
      change.e = node1;
      change.type = Change::Create;
    }
    else if (_ctr == 1)
    {
      change.e = relation;
      change.type = Change::Delete;
    }
    else if (_ctr == 2)
    {
      change.e = way;
      change.type = Change::Modify;
    }
    _ctr++;

    return change;
  }

private:

  int _ctr;

};

class OsmChangesetXmlFileWriterTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(OsmChangesetXmlFileWriterTest);
    CPPUNIT_TEST(runTest);
    CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    shared_ptr<ChangeSetProvider> changesetProvider(new TestChangesetProvider());
    OsmChangesetXmlFileWriter().write(
      "test-output/io/OsmChangesetXmlFileWriterTest.osc", changesetProvider);

    HOOT_STR_EQUALS(
      TestUtils::readFile("test-files/io/OsmChangesetXmlFileWriterTest/OsmChangesetXmlFileWriterTest.osc"),
      TestUtils::readFile("test-output/io/OsmChangesetXmlFileWriterTest.osc"));
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmChangesetXmlFileWriterTest, "quick");

}



