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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
// Hoot
#include <hoot/rnd/io/SparkChangesetWriter.h>
#include <hoot/core/TestUtils.h>

// Qt
#include <QDir>

namespace hoot
{

class SparkChangesetWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SparkChangesetWriterTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(missingHashTest);
  CPPUNIT_TEST(missingPreviousHashTest);
  CPPUNIT_TEST(wrongElementTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    DisableLog dl;

    const QString outputDir = "test-output/io/SparkChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-1.spark.1";

    SparkChangesetWriter changesetFileWriter;
    changesetFileWriter.open(changesetOutput);

    NodePtr node1(new Node(Status::Unknown1, 1, 56.546465, 99.21651651, 15.0));
    node1->getTags().set(MetadataTags::HootHash(), "1");
    node1->getTags().set("test1", "1");
    changesetFileWriter.writeChange(Change(Change::Create, node1));

    NodePtr node2(new Node(Status::Unknown1, 2, -34.44534436, 45.6575656, 15.0));
    node2->getTags().set(MetadataTags::HootHash(), "2");
    NodePtr node3(new Node(Status::Unknown1, 3, -34.44534436, 45.6575656, 15.0));
    node3->getTags().set(MetadataTags::HootHash(), "3");
    changesetFileWriter.writeChange(Change(Change::Modify, node2, node3));

    NodePtr node4(new Node(Status::Unknown1, 4, 34.56765, -45.6555, 15.0));
    node4->getTags().set(MetadataTags::HootHash(), "4");
    changesetFileWriter.writeChange(Change(Change::Delete, node4));

    changesetFileWriter.close();

    HOOT_FILE_EQUALS("test-files/io/SparkChangesetWriterTest/changeset-1.spark.1", changesetOutput);
  }

  void missingHashTest()
  {
    DisableLog dl;

    const QString outputDir = "test-output/io/SparkChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-1.spark.2";

    SparkChangesetWriter changesetFileWriter;
    changesetFileWriter.open(changesetOutput);

    NodePtr node1(new Node(Status::Unknown1, 1, 56.546465, 99.21651651, 15.0));
    node1->getTags().set("test1", "1");

    QString exceptionMsg("");
    try
    {
      changesetFileWriter.writeChange(Change(Change::Modify, node1));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No hash value set on element"));
  }

  void missingPreviousHashTest()
  {
    DisableLog dl;

    const QString outputDir = "test-output/io/SparkChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-1.spark.3";

    SparkChangesetWriter changesetFileWriter;
    changesetFileWriter.open(changesetOutput);

    NodePtr node1(new Node(Status::Unknown1, 1, 56.546465, 99.21651651, 15.0));
    node1->getTags().set(MetadataTags::HootHash(), "1");
    node1->getTags().set("test1", "1");
    NodePtr node2(new Node(Status::Unknown1, 2, -34.44534436, 45.6575656, 15.0));

    QString exceptionMsg("");
    try
    {
      changesetFileWriter.writeChange(Change(Change::Modify, node1, node2));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("No hash value set on previous element"));
  }

  void wrongElementTypeTest()
  {
    DisableLog dl;

    const QString outputDir = "test-output/io/SparkChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-1.spark.4";

    SparkChangesetWriter changesetFileWriter;
    changesetFileWriter.open(changesetOutput);

    NodePtr node1(new Node(Status::Unknown1, 1, 56.546465, 99.21651651, 15.0));
    node1->getTags().set(MetadataTags::HootHash(), "1");
    node1->getTags().set("test1", "1");
    NodePtr node2(new Node(Status::Unknown1, 2, 57.546465, 100.21651651, 15.0));
    node2->getTags().set(MetadataTags::HootHash(), "2");
    WayPtr way(new Way(Status::Unknown1, 1, 15.0));
    way->addNode(node1->getId());
    way->addNode(node2->getId());

    QString exceptionMsg("");
    try
    {
      changesetFileWriter.writeChange(Change(Change::Create, way));
    }
    catch (const HootException& e)
    {
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Only nodes are supported"));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SparkChangesetWriterTest, "quick");

}
