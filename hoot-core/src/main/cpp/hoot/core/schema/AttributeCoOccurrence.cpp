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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "AttributeCoOccurrence.h"

// Hoot
#include <hoot/core/algorithms/extractors/NameExtractor.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/language/ToEnglishTranslateStringDistance.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/scoring/TextTable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

// tgs
#include <tgs/HashMap.h>

using namespace std;

namespace hoot
{

/**
 * Traverses the OsmMap and creates a map from REF tags to ElementIds.
 */
class RefToEidVisitor : public ConstElementVisitor
{
public:

  using RefToEid = map<QString, set<ElementId>>;

  explicit RefToEidVisitor(QString ref) : _ref(ref) { }
  ~RefToEidVisitor() override = default;

  const RefToEid& getRefToEid() const { return _ref2Eid; }

  void visit(const ConstElementPtr& e) override
  {
    QStringList refs;
    const Tags& tags = e->getTags();
    if (tags.contains(_ref))
      tags.readValues(_ref, refs);
    refs.removeAll("todo");
    refs.removeAll("none");

    for (const auto& ref : qAsConst(refs))
      _ref2Eid[ref].insert(e->getElementId());
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

private:

  QString _ref;
  RefToEid _ref2Eid;
};

/**
 * Traverses the OsmMap and build a hashmap of Attribute Co-Occurrence values.
 */
class CoOccurrenceVisitor : public ConstElementVisitor, public ConstOsmMapConsumer
{
public:

  CoOccurrenceVisitor(const RefToEidVisitor::RefToEid& refSet, AttributeCoOccurrence::CoOccurrenceHash& h)
    : _refSet(refSet), _coOccurrence(h) { }
  ~CoOccurrenceVisitor() override = default;

  void setOsmMap(const OsmMap* map) override { _map = map; }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void visit(const ConstElementPtr& e) override
  {
    QStringList refs;
    const Tags& tags = e->getTags();
    if (tags.contains(MetadataTags::Ref1()))
    {
      tags.readValues(MetadataTags::Ref1(), refs);

      refs.removeAll("todo");
      refs.removeAll("none");

      if (!refs.empty())
      {
        // Find the REF1 id's in REF2.
        // NOTE: this blindly assumes that there is only ONE value in the REF1 tag list
        RefToEidVisitor::RefToEid::const_iterator refId = _refSet.find(refs[0]);

        if (refId != _refSet.end())
        {
          // Loop through the element Id's in REF2
          for (const auto& eid : refId->second)
          {
            // Loop through the Tags in REF1
            for (auto tag1 = tags.begin(); tag1 != tags.end(); ++tag1)
            {
              QString kvp1 = OsmSchema::toKvp(tag1.key(), tag1.value());

              // We are only looking at Enumerated tags
              if (OsmSchema::getInstance().getTagVertex(kvp1).getValueType() == hoot::Enumeration)
              {
                // Get the value from the corresponding tag in REF2
                QString kvp2 = OsmSchema::toKvp(tag1.key(), _map->getElement(eid)->getTags()[tag1.key()]);
                // LOG_INFO("Got Tags:" + kvp1 + " " + kvp2);
                _coOccurrence[kvp1][kvp2]++;
              }
            }
            // now loop through the REF2 tag list and fill in any missing tags.
            const Tags& tags2 = _map->getElement(eid)->getTags();
            for (auto tag2 = tags2.begin(); tag2 != tags2.end(); ++tag2 )
            {
              QString kvp2 = OsmSchema::toKvp(tag2.key(),tag2.value());

              // Skip the tags that are common
              if (tags.contains(tag2.key())) continue;

              if (OsmSchema::getInstance().getTagVertex(kvp2).getValueType() == hoot::Enumeration)
              {
                // "Missing" == "" tag value
                QString kvp1 = OsmSchema::toKvp(tag2.key(),"");
                // LOG_INFO("Got Tags:" + kvp1 + " " + kvp2);
                _coOccurrence[kvp1][kvp2]++;
              }
            }
            // now try matching up the "name" fields
            QString name1 = tags["name"];
            QString name2 = _map->getElement(eid)->getTags()["name"];

            QString kvpNull = OsmSchema::toKvp("name","<NULL>");
            QString kvpNonNull = OsmSchema::toKvp("name","<NON NULL>");
            QString kvpSame = OsmSchema::toKvp("name","<SIMILAR>");

            if (name1 == "")
            {
              if (name2 == "")
                _coOccurrence[kvpNull][kvpNull]++;
              else
                _coOccurrence[kvpNull][kvpNonNull]++;
            }
            else if (name2 == "")
              _coOccurrence[kvpNonNull][kvpNull]++;
            else
            {
              // Check if the names match
              if (_calculateNameScore(e, _map->getElement(eid)) >= ConfigOptions().getPoiPolygonNameScoreThreshold())
                _coOccurrence[kvpSame][kvpSame]++;
              else
                _coOccurrence[kvpNonNull][kvpNonNull]++;
            }
          }
        }
      }
    }
  }

private:

  const OsmMap* _map;
  RefToEidVisitor::RefToEid _refSet;
  AttributeCoOccurrence::CoOccurrenceHash& _coOccurrence;

  double _calculateNameScore(ConstElementPtr e1, ConstElementPtr e2) const
  {
    // This was taken directly from PoiPolygonMatch.cpp:
    // "double PoiPolygonMatch::_calculateNameScore"

    // found experimentally when doing building name comparisons
    return
      NameExtractor(
        std::make_shared<ToEnglishTranslateStringDistance>(
          std::make_shared<MeanWordSetDistance>(std::make_shared<LevenshteinDistance>(1.45))))
        .extract(e1, e2);
  }
};


void AttributeCoOccurrence::addToMatrix(const ConstOsmMapPtr& in)

{
  RefToEidVisitor ref2(MetadataTags::Ref2());
  in->visitRo(ref2);

  CoOccurrenceVisitor coOccurrenceResult(ref2.getRefToEid(), _resultMatrix);
  in->visitRo(coOccurrenceResult);
}


QString AttributeCoOccurrence::printTable()
{
  TextTable::Data data;

  for (auto it = _resultMatrix.begin(); it != _resultMatrix.end(); ++it)
  {
    // Get the list of keys and build a reverse matrix
    for (auto jt = it->second.begin(); jt != it->second.end(); ++jt)
      data[jt->first][it->first] = jt->second;
  }

  TextTable table(data);

  return table.toWikiString();
}


QString AttributeCoOccurrence::printList()
{
  QStringList keyList;
  QString result;

  // Build a list of REF1 keys so we can sort the table
  for (auto it = _resultMatrix.begin(); it != _resultMatrix.end(); ++it)
    keyList.append(it->first);

  keyList.sort();

  // Print the Matrix as a list, sorted by REF1 keys
  result.append(QString("N  : %1 -> %2\n").arg(MetadataTags::Ref1(), MetadataTags::Ref2()));
  for (const auto& key : qAsConst(keyList))
  {
    CoOccurrenceHash::const_iterator jt = _resultMatrix.find(key);
    for (auto kt = jt->second.begin(); kt != jt->second.end(); ++kt)
      result += QString("%1: %2 -> %3\n").arg(kt->second,-3,10).arg(key, kt->first);
  }

  return result;
}

}
