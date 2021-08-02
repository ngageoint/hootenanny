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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/elements/Node.h>
#include <hoot/rnd/io/MultiaryIngestChangesetReader.h>

namespace hoot
{

class MultiaryIngestChangesetReaderTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(MultiaryIngestChangesetReaderTest);
  CPPUNIT_TEST(elementAsJsonTest);
  CPPUNIT_TEST(elementAsXmlTest);
  CPPUNIT_TEST(wrongElementTypeTest);
  CPPUNIT_TEST_SUITE_END();

public:

  MultiaryIngestChangesetReaderTest()
    : HootTestFixture("test-files/rnd/io/MultiaryIngestChangesetTests/",
                      UNUSED_PATH)
  {
  }

  void verifyInput(const QString& inputFile)
  {
    MultiaryIngestChangesetReader changesetFileReader;
    changesetFileReader.open(inputFile);

    //MultiaryIngestChangesetReader doesn't parse all the info from the input, as it doesn't currently
    //need all of it...we only test for what it actually parses.

    int ctr = 0;
    while (changesetFileReader.hasMoreChanges())
    {
      Change change = changesetFileReader.readNextChange();
      if (ctr == 0)
      {
        CPPUNIT_ASSERT(change.getType() == Change::Create);
        CPPUNIT_ASSERT(change.getElement()->getElementType() == ElementType::Node);
        CPPUNIT_ASSERT_EQUAL((long)1, change.getElement()->getId());
        ConstNodePtr node = std::dynamic_pointer_cast<const Node>(change.getElement());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(56.546465, node->getX(), 1e-5);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(99.21651651000001, node->getY(), 1e-5);
        CPPUNIT_ASSERT_EQUAL(1, change.getElement()->getTags().size());
      }
      else if (ctr == 1)
      {
        CPPUNIT_ASSERT(change.getType() == Change::Modify);
        CPPUNIT_ASSERT(change.getElement()->getElementType() == ElementType::Node);
        CPPUNIT_ASSERT_EQUAL((long)2, change.getElement()->getId());
        ConstNodePtr node = std::dynamic_pointer_cast<const Node>(change.getElement());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-34.44534436, node->getX(), 1e-5);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(45.6575656, node->getY(), 1e-5);
        CPPUNIT_ASSERT_EQUAL(2, change.getElement()->getTags().size());
      }
      else if (ctr == 2)
      {
        CPPUNIT_ASSERT(change.getType() == Change::Delete);
        CPPUNIT_ASSERT(change.getElement()->getElementType() == ElementType::Node);
        CPPUNIT_ASSERT_EQUAL((long)4, change.getElement()->getId());
        ConstNodePtr node = std::dynamic_pointer_cast<const Node>(change.getElement());
        CPPUNIT_ASSERT_DOUBLES_EQUAL(34.56765, node->getX(), 1e-5);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(-45.6555, node->getY(), 1e-5);
        CPPUNIT_ASSERT_EQUAL(0, change.getElement()->getTags().size());
      }
      else
      {
        throw HootException("Wrong number of nodes.");
      }

      ctr++;
    }
    CPPUNIT_ASSERT_EQUAL(3, ctr);

    changesetFileReader.close();
  }

  void elementAsJsonTest()
  {
    verifyInput(_inputPath + "changeset-1.spark.1");
  }

  void elementAsXmlTest()
  {
    verifyInput(_inputPath + "changeset-2.spark.1");
  }

  void wrongElementTypeTest()
  {
    MultiaryIngestChangesetReader changesetFileReader;
    QString exceptionMsg("");
    try
    {
      changesetFileReader.open(_inputPath + "changeset-3.spark.1");
      while (changesetFileReader.hasMoreChanges())
      {
        changesetFileReader.readNextChange();
      }
    }
    catch (const HootException& e)
    {
      changesetFileReader.close();
      exceptionMsg = e.what();
    }
    CPPUNIT_ASSERT(exceptionMsg.contains("Only nodes are supported"));
  }

};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MultiaryIngestChangesetReaderTest, "glacial");

}
