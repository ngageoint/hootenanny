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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/io/db/PostgresqlDumpfileWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class PostgresqlDumpfileWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(PostgresqlDumpfileWriterTest);
  CPPUNIT_TEST(runWriterTest);
  CPPUNIT_TEST_SUITE_END();

public:

  shared_ptr<OsmMap> _map;

  shared_ptr<Node> createNode(double x, double y)
  {
    shared_ptr<Node> n(new Node(Status::Unknown1, _map->createNextNodeId(), x, y, 10.0));
    _map->addNode(n);
    return n;
  }

  shared_ptr<OsmMap> createTestMap()
  {
    shared_ptr<OsmMap> map(new OsmMap());
    _map = map;

    shared_ptr<Node> n1 = createNode(-77.0, 38.0);
    n1->setTag("building", "yes");
    n1->setTag("name", "n1");

    shared_ptr<Way> w1(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w1->setTag("area", "yes");
    w1->setTag("building", "yes");
    w1->setTag("name", "w1");
    w1->addNode(createNode(-77.1, 38.0)->getId());
    w1->addNode(createNode(-77.2, 38.0)->getId());
    w1->addNode(createNode(-77.2, 38.1)->getId());
    w1->addNode(w1->getNodeId(0));
    map->addWay(w1);

    shared_ptr<Way> w2(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w2->setTag("highway", "track");
    w2->setTag("name", "w2");
    w2->addNode(createNode(-77.3, 38.0)->getId());
    w2->addNode(createNode(-77.3, 38.1)->getId());
    map->addWay(w2);

    shared_ptr<Way> w3(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w3->setTag("highway", "road");
    w3->setTag("name", "w3");
    w3->addNode(createNode(-77.4, 38.0)->getId());
    w3->addNode(createNode(-77.4, 38.1)->getId());
    map->addWay(w3);

    shared_ptr<Way> w4(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w4->addNode(createNode(-77.5, 38.0)->getId());
    w4->addNode(createNode(-77.7, 38.0)->getId());
    w4->addNode(createNode(-77.6, 38.1)->getId());
    w4->addNode(w4->getNodeId(0));
    map->addWay(w4);

    shared_ptr<Way> w5(new Way(Status::Unknown1, map->createNextWayId(), 13.0));
    w5->addNode(createNode(-77.55, 38.01)->getId());
    w5->addNode(createNode(-77.65, 38.01)->getId());
    w5->addNode(createNode(-77.6, 38.05)->getId());
    w5->addNode(w5->getNodeId(0));
    map->addWay(w5);

    shared_ptr<Relation> r1(new Relation(Status::Unknown1, 1, 15.0, "multipolygon"));
    r1->setTag("building", "yes");
    r1->setTag("name", "r1");
    r1->addElement("outer", w4->getElementId());
    r1->addElement("inner", w5->getElementId());
    map->addRelation(r1);

    return map;
  }

  void runWriterTest()
  {
    PostgresqlDumpfileWriter pgDumpWriter;

    QDir().mkpath("test-output/io/PostgresqlDumpWriterTest/");
    QString outFile = "test-output/io/PostgresqlDumpWriterTest/PostgresqlDumpWriter_out.sql";
    pgDumpWriter.open(outFile);
    pgDumpWriter.write(createTestMap());

    QRegExp reDate("[12][0-9][0-9][0-9]-[01][0-9]-[0-3][0-9]");
    QRegExp reTime("[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9][0-9][0-9]");

    QString stdFile = "test-files/io/PostgresqlDumpWriterTest/PostgresqlDumpWriter.sql";
    QFile stdInputFile(stdFile);

    QStringList stdList;
    if (stdInputFile.open(QIODevice::ReadOnly))
    {
      QTextStream in(&stdInputFile);
      while (!in.atEnd())
      {
        QString line = in.readLine();
        line = line.remove(reDate);
        line = line.remove(reTime);
        stdList << line;
      }
      stdInputFile.close();
    }

    QFile inputFile(outFile);
    QStringList inList;
    if (inputFile.open(QIODevice::ReadOnly))
    {
      QTextStream in(&inputFile);
      while (!in.atEnd())
      {
        QString line = in.readLine();
        line = line.remove(reDate);
        line = line.remove(reTime);
        inList << line;
      }
      inputFile.close();
    }
    CPPUNIT_ASSERT_EQUAL(inList.size(), stdList.size());
    pgDumpWriter.close();

    for (int i = 0; i < stdList.size(); i++)
    {
      HOOT_STR_EQUALS(stdList.at(i), inList.at(i));
    }
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PostgresqlDumpfileWriterTest, "quick");

}
