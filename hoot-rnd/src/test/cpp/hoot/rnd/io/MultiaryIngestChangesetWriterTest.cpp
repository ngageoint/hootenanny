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
#include <hoot/rnd/io/MultiaryIngestChangesetWriter.h>
#include <hoot/core/TestUtils.h>

//#include <hoot/core/io/GeoNamesReader.h>
//#include <hoot/rnd/io/MultiaryIngestChangesetReader.h>

// Qt
#include <QDir>

namespace hoot
{

class MultiaryIngestChangesetWriterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryIngestChangesetWriterTest);
  CPPUNIT_TEST(elementAsJsonTest);
  CPPUNIT_TEST(elementAsXmlTest);
  CPPUNIT_TEST(missingHashTest);
  CPPUNIT_TEST(missingPreviousHashTest);
  CPPUNIT_TEST(wrongElementTypeTest);
  //CPPUNIT_TEST(passingUnicodeConversionTest);
  //CPPUNIT_TEST(failingUnicodeConversionTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void writeNodes(const QString output, const QString format)
  {
    MultiaryIngestChangesetWriter changesetFileWriter;
    changesetFileWriter.setElementPayloadFormat(format);
    changesetFileWriter.open(output);

    NodePtr node1(new Node(Status::Unknown1, 1, 56.546465, 99.21651651, 15.0));
    node1->getTags().set(MetadataTags::HootHash(), "1");
    node1->getTags().set("test1", "1");
    changesetFileWriter.writeChange(Change(Change::Create, node1));

    NodePtr node2(new Node(Status::Unknown1, 2, -34.44534436, 45.6575656, 15.0));
    node2->getTags().set(MetadataTags::HootHash(), "2");
    node2->getTags().set("test2", "2");
    node2->getTags().set("test3", "3");
    NodePtr node3(new Node(Status::Unknown1, 3, -34.44534436, 45.6575656, 15.0));
    node3->getTags().set(MetadataTags::HootHash(), "3");
    changesetFileWriter.writeChange(Change(Change::Modify, node2, node3));

    NodePtr node4(new Node(Status::Unknown1, 4, 34.56765, -45.6555, 15.0));
    node4->getTags().set(MetadataTags::HootHash(), "4");
    changesetFileWriter.writeChange(Change(Change::Delete, node4));

    changesetFileWriter.close();
  }

  void elementAsJsonTest()
  {
    DisableLog dl;

    const QString outputDir = "test-output/io/MultiaryIngestChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-1.spark.1";

    writeNodes(changesetOutput, "json");

    HOOT_FILE_EQUALS(
      "test-files/io/MultiaryIngestChangesetWriterTest/changeset-1.spark.1", changesetOutput);
  }

  void elementAsXmlTest()
  {
    DisableLog dl;

    const QString outputDir = "test-output/io/MultiaryIngestChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-2.spark.1";

    writeNodes(changesetOutput, "xml");

    HOOT_FILE_EQUALS(
      "test-files/io/MultiaryIngestChangesetWriterTest/changeset-2.spark.1", changesetOutput);
  }

  void missingHashTest()
  {
    DisableLog dl;

    const QString outputDir = "test-output/io/MultiaryIngestChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-3.spark.1";

    MultiaryIngestChangesetWriter changesetFileWriter;
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

    const QString outputDir = "test-output/io/MultiaryIngestChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-4.spark.1";

    MultiaryIngestChangesetWriter changesetFileWriter;
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

    const QString outputDir = "test-output/io/MultiaryIngestChangesetWriterTest";
    QDir().mkpath(outputDir);
    const QString changesetOutput = outputDir + "/changeset-5.spark.1";

    MultiaryIngestChangesetWriter changesetFileWriter;
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

//  void passingUnicodeConversionTest()
//  {
//    //TODO: to use this you have to comment out the line:
      //if (!change.getElement()->getTags().contains(MetadataTags::HootHash()))
      //in SparkChangesetWriter

//    GeoNamesReader geonamesReader;
//    geonamesReader.setUseDataSourceIds(true);
//    geonamesReader.open(
//      "test-files/cmd/slow/ServiceMultiaryIngestCmdTest/allCountries-8-15-17-10.geonames");

//    SparkChangesetWriter sparkChangesetWriter;
//    //This line is the only difference between this test and failingConversionTest
//    sparkChangesetWriter.setElementPayloadFormat("xml");
//    sparkChangesetWriter.open("tmp/test.spark.1");
//
//    LOG_DEBUG("Reading geonames and writing to spark xml...");
//    while (geonamesReader.hasMoreElements())
//    {
//      ElementPtr element = geonamesReader.readNextElement();
//      //these elements will *not* have uncorrupted tags
//      LOG_VART(element);
//      sparkChangesetWriter.writeChange(Change(Change::Create, element));
//    }
//    geonamesReader.close();
//    sparkChangesetWriter.close();

//    SparkChangesetReader sparkChangesetReader;
//    sparkChangesetReader.open("tmp/test.spark.1");
//    LOG_DEBUG("Reading spark xml...");
//    while (sparkChangesetReader.hasMoreChanges())
//    {
//      Change change = sparkChangesetReader.readNextChange();
//      //these elements will *not* have corrupted tags
//      //reader
//      LOG_VART(change.getElement());
//    }
//    sparkChangesetReader.close();
//  }

//  //this reproduces #1772
//  void failingUnicodeConversionTest()
//  {
//    //TODO: to use this you have to comment out the line:
//    //if (!change.getElement()->getTags().contains(MetadataTags::HootHash()))
//    //in SparkChangesetWriter

//    GeoNamesReader geonamesReader;
//    geonamesReader.setUseDataSourceIds(true);
//    geonamesReader.open(
//      "test-files/cmd/slow/ServiceMultiaryIngestCmdTest/allCountries-8-15-17-10.geonames");

//    SparkChangesetWriter sparkChangesetWriter;
//    //This line is the only difference between this test and passingConversionTest
//    sparkChangesetWriter.setElementPayloadFormat("json");
//    sparkChangesetWriter.open("tmp/test.spark.1");

//    LOG_DEBUG("Reading geonames and writing to spark json...");
//    while (geonamesReader.hasMoreElements())
//    {
//      ElementPtr element = geonamesReader.readNextElement();
//      //these elements will *not* have uncorrupted tags
//      LOG_VART(element);
//      sparkChangesetWriter.writeChange(Change(Change::Create, element));
//    }
//    geonamesReader.close();
//    sparkChangesetWriter.close();

//    SparkChangesetReader sparkChangesetReader;
//    sparkChangesetReader.open("tmp/test.spark.1");
//    LOG_DEBUG("Reading spark json...");
//    while (sparkChangesetReader.hasMoreChanges())
//    {
//      Change change = sparkChangesetReader.readNextChange();
//      //these elements *will* have corrupted tags; seems to be caused by the boost property json
//      //reader
//      LOG_VART(change.getElement());
//    }
//    sparkChangesetReader.close();
//  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryIngestChangesetWriterTest, "quick");

}
