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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

//  Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

class OsmApiChangesetTest : public HootTestFixture
{
  CPPUNIT_TEST_SUITE(OsmApiChangesetTest);
  CPPUNIT_TEST(runXmlChangesetTest);
  CPPUNIT_TEST(runNonAsciiXmlChangesetTest);
  CPPUNIT_TEST(runXmlChangesetJoinTest);
  CPPUNIT_TEST(runXmlChangesetUpdateTest);
  CPPUNIT_TEST(runXmlChangesetSplitTest);
  CPPUNIT_TEST(runXmlChangesetSplitWayTest);
  CPPUNIT_TEST(runXmlChangesetErrorFixTest);
  CPPUNIT_TEST(runXmlChangesetFailureMatchesTest);
  CPPUNIT_TEST(runXmlChangesetSplitDeleteTest);
  CPPUNIT_TEST_SUITE_END();

public:

  OsmApiChangesetTest()
    : HootTestFixture("test-files/io/OsmChangesetElementTest/",
                      UNUSED_PATH)
  {
  }

  void runXmlChangesetTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");

    QString expectedText = FileUtils::readFully(_inputPath + "ToyTestAChangeset1.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runNonAsciiXmlChangesetTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "DjiboutiNonAsciiTest.osc");

    QString expectedText = FileUtils::readFully(_inputPath + "DjiboutiNonAsciiTestExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetJoinTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset.split.osc");
    changeset.loadChangeset("test-files/io/OsmXmlChangesetFileWriterTest/changeset-001.split.osc");

    QString expectedText = FileUtils::readFully(_inputPath + "ChangesetMergeExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetUpdateTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    QString update =
        "<diffResult generator=\"OpenStreetMap Server\" version=\"0.6\">\n"
        "  <node old_id=\"-1\" new_id=\"1\" new_version=\"1\"/>\n"
        "  <node old_id=\"-2\" new_id=\"2\" new_version=\"1\"/>\n"
        "  <node old_id=\"-32\" new_id=\"32\" new_version=\"1\"/>\n"
        "  <node old_id=\"-7\" new_id=\"7\" new_version=\"1\"/>\n"
        "  <node old_id=\"-8\" new_id=\"8\" new_version=\"1\"/>\n"
        "  <node old_id=\"-33\" new_id=\"33\" new_version=\"1\"/>\n"
        "  <way old_id=\"-1\" new_id=\"1\" new_version=\"1\"/>\n"
        "  <way old_id=\"-2\" new_id=\"2\" new_version=\"1\"/>\n"
        "</diffResult>";

    changeset.updateChangeset(update);

    QString expectedText = FileUtils::readFully(_inputPath + "ToyTestAChangeset2.osc");

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 2));
  }

  void runXmlChangesetSplitTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");

    //  Four elements max will divide the changeset into four changesets
    //  one for each way, with its corresponding nodes
    changeset.setMaxPushSize(4);

    QStringList expectedFiles;
    expectedFiles.append(_inputPath + "ToyTestASplit1.osc");
    expectedFiles.append(_inputPath + "ToyTestASplit2.osc");
    expectedFiles.append(_inputPath + "ToyTestASplit3.osc");
    expectedFiles.append(_inputPath + "ToyTestASplit4.osc");

    QStringList updatedFiles;
    updatedFiles.append(_inputPath + "ToyTestASplit1.response.xml");
    updatedFiles.append(_inputPath + "ToyTestASplit2.response.xml");
    updatedFiles.append(_inputPath + "ToyTestASplit3.response.xml");
    updatedFiles.append(_inputPath + "ToyTestASplit4.response.xml");

    long processed[] = { 4, 8, 37, 40 };

    ChangesetInfoPtr info;
    int index = 0;
    while (!changeset.isDone())
    {
      info.reset(new ChangesetInfo());
      changeset.calculateChangeset(info);

      QString expectedText = FileUtils::readFully(expectedFiles[index]);

      HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, index + 1));

      QString updatedText = FileUtils::readFully(updatedFiles[index]);
      changeset.updateChangeset(updatedText);

      CPPUNIT_ASSERT_EQUAL(processed[index], changeset.getProcessedCount());

      ++index;
    }
    CPPUNIT_ASSERT_EQUAL(40L, changeset.getTotalElementCount());
    CPPUNIT_ASSERT_EQUAL(36L, changeset.getTotalNodeCount());
    CPPUNIT_ASSERT_EQUAL(4L, changeset.getTotalWayCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalRelationCount());
    CPPUNIT_ASSERT_EQUAL(40L, changeset.getTotalCreateCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalModifyCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalDeleteCount());
  }

  void runXmlChangesetSplitWayTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ToyTestAInput.osc");
    //  Split the ways to a max of 8 nodes per way
    changeset.splitLongWays(8);

    QString expectedText = FileUtils::readFully(_inputPath + "ChangesetSplitWayExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    HOOT_STR_EQUALS(expectedText, changeset.getChangesetString(info, 1));
  }

  void runXmlChangesetErrorFixTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "ChangesetErrorFixInput.osc");
    //  Fix the bad input changeset
    changeset.fixMalformedInput();

    QString expectedText = FileUtils::readFully(_inputPath + "ChangesetErrorFixExpected.osc");

    ChangesetInfoPtr info(new ChangesetInfo());
    changeset.calculateChangeset(info);

    QString change = changeset.getChangesetString(info, 1);
    HOOT_STR_EQUALS(expectedText, change);

    QString error = changeset.getFailedChangesetString();
    QString expectedError = FileUtils::readFully(_inputPath + "ChangesetErrorFixErrors.osc");
    HOOT_STR_EQUALS(expectedError, error);
  }

  void runXmlChangesetFailureMatchesTest()
  {
    XmlChangeset changeset;

    bool found = false;
    long id = 0;
    long id2 = 0;
    std::vector<long> ids;
    ElementType::Type type = ElementType::Unknown;
    ElementType::Type type2 = ElementType::Unknown;
    QString hint;

    //  Test Placeholder failure with node in way
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Placeholder node not found for reference -145213 in way -5687";
    found = changeset.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-145213L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Node, type);
    CPPUNIT_ASSERT_EQUAL(-5687L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type2);

    //  Test Placeholder failure with way in relation
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "Placeholder Way not found for reference -12257 in Relation -51";
    found = changeset.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-12257L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type);
    CPPUNIT_ASSERT_EQUAL(-51L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type2);

    //  Test Placeholder failure with empty string
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    type2 = ElementType::Unknown;
    hint = "";
    found = changeset.matchesPlaceholderFailure(hint, id, type, id2, type2);
    CPPUNIT_ASSERT_EQUAL(false, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type);
    CPPUNIT_ASSERT_EQUAL(0L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type2);

    //  Test relation failure with no relation ID
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "Relation with id  cannot be saved due to Relation with id 1707699";
    found = changeset.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(1707699L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type);

    //  Test relation failure with relation ID
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "Relation with id 122 cannot be saved due to Way with id 7699";
    found = changeset.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(122L, id);
    CPPUNIT_ASSERT_EQUAL(7699L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Way, type);

    //  Test multi relation failure with relation ID
    id = 0;
    ids.clear();
    type = ElementType::Unknown;
    hint = "Relation -2 requires the relations with id in 1707148,1707249,1707264,1707653,1707654,1707655,1707656,1707657,1707658,1707699,1707700, which either do not exist, or are not visible.";
    found = changeset.matchesMultiRelationFailure(hint, id, ids, type);
    std::vector<long> expected_ids { 1707148,1707249,1707264,1707653,1707654,1707655,1707656,1707657,1707658,1707699,1707700 };
    CPPUNIT_ASSERT_EQUAL(true, found);
    CPPUNIT_ASSERT_EQUAL(-2L, id);
    CPPUNIT_ASSERT_EQUAL(ElementType::Relation, type);
    CPPUNIT_ASSERT_EQUAL(11, (int)ids.size());
    for (int i = 0; i < (int)ids.size(); ++i)
      CPPUNIT_ASSERT_EQUAL(expected_ids[i], ids[i]);


    //  Test relation failure with empty string
    id = 0;
    id2 = 0;
    type = ElementType::Unknown;
    hint = "";
    found = changeset.matchesRelationFailure(hint, id, id2, type);
    CPPUNIT_ASSERT_EQUAL(false, found);
    CPPUNIT_ASSERT_EQUAL(0L, id);
    CPPUNIT_ASSERT_EQUAL(0L, id2);
    CPPUNIT_ASSERT_EQUAL(ElementType::Unknown, type);
  }

  void runXmlChangesetSplitDeleteTest()
  {
    XmlChangeset changeset;
    changeset.loadChangeset(_inputPath + "DeleteSplit.osc");

    //  8 elements max will divide the changeset into 4 changesets
    changeset.setMaxPushSize(8);

    QStringList expectedFiles;
    expectedFiles.append(_inputPath + "DeleteSplit1.osc");
    expectedFiles.append(_inputPath + "DeleteSplit2.osc");
    expectedFiles.append(_inputPath + "DeleteSplit3.osc");
    expectedFiles.append(_inputPath + "DeleteSplit4.osc");

    QStringList updatedFiles;
    updatedFiles.append(_inputPath + "DeleteSplit1.response.xml");
    updatedFiles.append(_inputPath + "DeleteSplit2.response.xml");
    updatedFiles.append(_inputPath + "DeleteSplit3.response.xml");
    updatedFiles.append(_inputPath + "DeleteSplit4.response.xml");

    long processed[] = { 8, 8, 16, 24 };

    int index = 0;
    while (!changeset.isDone())
    {
      ChangesetInfoPtr info1(new ChangesetInfo());
      changeset.calculateChangeset(info1);
      QString expectedText1 = FileUtils::readFully(expectedFiles[index]);
      HOOT_STR_EQUALS(expectedText1, changeset.getChangesetString(info1, index + 1));

      ChangesetInfoPtr info2(new ChangesetInfo());
      changeset.calculateChangeset(info2);
      QString expectedText2 = FileUtils::readFully(expectedFiles[index + 1]);
      HOOT_STR_EQUALS(expectedText2, changeset.getChangesetString(info2, index + 2));

      QString updatedText1 = FileUtils::readFully(updatedFiles[index]);
      changeset.updateChangeset(updatedText1);
      CPPUNIT_ASSERT_EQUAL(processed[index], changeset.getProcessedCount());

      QString updatedText2 = FileUtils::readFully(updatedFiles[index + 1]);
      changeset.updateChangeset(updatedText2);
      CPPUNIT_ASSERT_EQUAL(processed[index + 1], changeset.getProcessedCount());

      index += 2;
    }
    CPPUNIT_ASSERT_EQUAL(24L, changeset.getTotalElementCount());
    CPPUNIT_ASSERT_EQUAL(23L, changeset.getTotalNodeCount());
    CPPUNIT_ASSERT_EQUAL(1L, changeset.getTotalWayCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalRelationCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalCreateCount());
    CPPUNIT_ASSERT_EQUAL(0L, changeset.getTotalModifyCount());
    CPPUNIT_ASSERT_EQUAL(24L, changeset.getTotalDeleteCount());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetTest, "quick");
//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(OsmApiChangesetElementTest, "current");

}
