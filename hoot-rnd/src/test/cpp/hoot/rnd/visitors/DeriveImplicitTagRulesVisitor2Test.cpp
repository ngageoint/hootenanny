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
#include <hoot/rnd/visitors/DeriveImplicitTagRulesVisitor2.h>
#include <hoot/rnd/visitors/DeriveImplicitTagRulesVisitor.h>
#include <hoot/rnd/io/ImplicitTagRulesSqliteWriter.h>

// Qt
#include <QDir>

namespace hoot
{

class DeriveImplicitTagRulesVisitor2Test : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(DeriveImplicitTagRulesVisitor2Test);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void tearDown()
  {
    TestUtils::resetEnvironment();
  }

  static QString outDir() { return "test-output/io/DeriveImplicitTagRulesVisitor2Test"; }

  void runBasicTest()
  {
    QDir().mkpath(outDir());
    const QString databaseFile =
      outDir() + "/DeriveImplicitTagRulesVisitor2Test-runBasicTest-rules.db";
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

    DeriveImplicitTagRulesVisitor2 uut(databaseFile);

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
    ImplicitTagRulesSqliteWriter rulesWriter;
    rulesWriter.open(outputPath);
    rulesWriter.write(convertRulesToRulesMap(DeriveImplicitTagRulesVisitor().getTestRules()));
    rulesWriter.close();
  }

  //temporary method to convert between rule data structures; shouldn't need it permanently
  ImplicitTagRules convertRulesToRulesMap(const QList<ImplicitRulePtr>& rules)
  {
    ImplicitTagRules rulesMap;
    for (QList<ImplicitRulePtr>::const_iterator rulesItr = rules.begin();
         rulesItr != rules.end(); ++rulesItr)
    {
      ImplicitRulePtr rule = *rulesItr;
      const QStringList words = rule->words;
      const Tags& tags = rule->tags;

      for (int i = 0; i < words.size(); i++)
      {
        const QString word = words.at(i);
        LOG_VART(word);
        if (!rulesMap.contains(word))
        {
          rulesMap[word] = QMap<QString, long>();
        }
        for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
        {
          //we're not concerned with the occurrance count values here
          rulesMap[word][tagItr.key() + "=" + tagItr.value()] = 1;
        }
      }
    }
    LOG_VART(rulesMap);
    return rulesMap;
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(DeriveImplicitTagRulesVisitor2Test, "quick");

}
