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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/TestUtils.h>
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/visitors/AddImplicitlyDerivedTagsPoiVisitor.h>
#include <hoot/rnd/io/ImplicitTagRuleSqliteRecordWriter.h>
#include <hoot/rnd/schema/ImplicitTagRule.h>

// Qt
#include <QDir>

namespace hoot
{

class AddImplicitlyDerivedTagsPoiVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(AddImplicitlyDerivedTagsPoiVisitorTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  static QString outDir() { return "test-output/io/AddImplicitlyDerivedTagsPoiVisitorTest"; }

  ImplicitTagRules getTestRules()
  {
    ImplicitTagRules rules;

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("mosque"));
    rules.back()->getWords().insert(QString::fromUtf8("msjd"));
    rules.back()->getWords().insert(QString::fromUtf8("masjid"));
    rules.back()->getWords().insert(QString::fromUtf8("Jāmi"));
    rules.back()->getTags().appendValue("amenity=place_of_worship");
    rules.back()->getTags().appendValue("religion=muslim");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("alshyt"));
    rules.back()->getWords().insert(QString::fromUtf8("whdt"));
    rules.back()->getWords().insert(QString::fromUtf8("alshy"));
    rules.back()->getWords().insert(QString::fromUtf8("almrkz"));
    rules.back()->getWords().insert(QString::fromUtf8("shyt"));
    rules.back()->getWords().insert(QString::fromUtf8("bny"));
    rules.back()->getWords().insert(QString::fromUtf8("Sihhi"));
    rules.back()->getWords().insert(QString::fromUtf8("Şiḩḩī"));
    rules.back()->getWords().insert(QString::fromUtf8("alwhdt"));
    rules.back()->getTags().appendValue("amenity=clinic");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("Şiḩḩīy"));
    rules.back()->getWords().insert(QString::fromUtf8("Mustashfa"));
    rules.back()->getTags().appendValue("amenity=hospital");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("Shurţat"));
    rules.back()->getWords().insert(QString::fromUtf8("Police"));
    rules.back()->getTags().appendValue("amenity=police");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("school"));
    rules.back()->getTags().appendValue("amenity=school");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("embassy"));
    rules.back()->getWords().insert(QString::fromUtf8("ﺲﻓﺍﺭﺓ"));
    rules.back()->getTags().appendValue("amenity=embassy");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("petroleumeum"));
    rules.back()->getTags().appendValue("amenity=fuel");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("bank"));
    rules.back()->getTags().appendValue("amenity=bank");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("hotel"));
    rules.back()->getWords().insert(QString::fromUtf8("funduq"));
    rules.back()->getWords().insert(QString::fromUtf8("ﻢﻧﺎﻣﺓ"));
    rules.back()->getTags().appendValue("tourism=hotel");

    rules.append(ImplicitTagRulePtr(new ImplicitTagRule()));
    rules.back()->getWords().insert(QString::fromUtf8("museum"));
    rules.back()->getTags().appendValue("tourism=museum");

    foreach (ImplicitTagRulePtr r, rules)
    {
      const QSet<QString> words = r->getWords();
      QSet<QString> lowerCaseWords;
      for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end(); ++wordItr)
      {
        lowerCaseWords.insert((*wordItr).toLower());
      }
      r->setWords(lowerCaseWords);
    }

    return rules;
  }

  void runBasicTest()
  {
    QDir().mkpath(outDir());
    const QString databaseFile =
      outDir() + "/AddImplicitlyDerivedTagsPoiVisitorTest-runBasicTest-rules.sqlite";
    writeRules(databaseFile);

    QString testJsonStr = QString::fromUtf8(
      "{                                      \n"
      " 'elements': [                         \n"
      " { 'type': 'node', 'id': -1, 'lat': 2.0, 'lon': -3.0, \n"
      "   'tags': { 'amenity': 'pub', 'name': 'Alshy Burgers' } },\n"
      " { 'type': 'node', 'id': -2, 'lat': 3.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Alshy Clinic' } },\n"
      " { 'type': 'node', 'id': -3, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'poi': 'yes', 'name': 'masjid' } },\n"
      " { 'type': 'node', 'id': -4, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'place': 'locality', 'name': 'Mustashfa alwhdt' } },\n"
      " { 'type': 'node', 'id': -5, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Sihhi' } },\n"
      " { 'type': 'node', 'id': -6, 'lat': 14.0, 'lon': -3.0, \n"
      "   'tags': { 'name': 'Sihhi', 'alt_name': 'Clinic' } }\n"
      "]                                      \n"
      "}                                      \n");

    OsmMapPtr map = OsmJsonReader().loadFromString(testJsonStr);
    // the JSON parser doesn't handle exotic characters.
    map->getNode(-5)->getTags()["alt_name"] = QString::fromUtf8("Şiḩḩī");

    AddImplicitlyDerivedTagsPoiVisitor uut(databaseFile);

    map->visitRw(uut);

    HOOT_STR_EQUALS("name = Alshy Burgers\n"
                    "amenity = pub\n",
                    map->getNode(-1)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: alshy\n"
                    "name = Alshy Clinic\n"
                    "amenity = clinic\n",
                    map->getNode(-2)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: masjid\n"
                    "poi = yes\n"
                    "religion = muslim\n"
                    "name = masjid\n"
                    "amenity = place_of_worship\n",
                    map->getNode(-3)->getTags());
    HOOT_STR_EQUALS("place = locality\n"
                    "note = Found multiple possible matches for implicit tags: alwhdt, mustashfa\n"
                    "name = Mustashfa alwhdt\n",
                    map->getNode(-4)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: sihhi, şiḩḩī\n"
                    "alt_name = Şiḩḩī\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-5)->getTags());
    HOOT_STR_EQUALS("note = Implicitly derived tags based on: sihhi\n"
                    "alt_name = Clinic\n"
                    "name = Sihhi\n"
                    "amenity = clinic\n",
                    map->getNode(-6)->getTags());
  }

  void writeRules(const QString outputPath)
  {
    ImplicitTagRuleSqliteRecordWriter rulesWriter;
    rulesWriter.open(outputPath);
    const ImplicitTagRules rules = getTestRules();
    for (ImplicitTagRules::const_iterator ruleItr = rules.begin(); ruleItr != rules.end();
         ++ruleItr)
    {
      const ImplicitTagRulePtr rule = *ruleItr;
      const QSet<QString> words = rule->getWords();
      const Tags tags = rule->getTags();
      for (QSet<QString>::const_iterator wordItr = words.begin(); wordItr != words.end();
           ++wordItr)
      {
        const QString word = *wordItr;
        for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
        {
          const QString kvp = tagItr.key() + "=" + tagItr.value();
          rulesWriter.write(word, kvp, 1);   //don't care about the tag counts for this test
        }
      }
    }
    rulesWriter.close();
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(AddImplicitlyDerivedTagsPoiVisitorTest, "quick");

}
