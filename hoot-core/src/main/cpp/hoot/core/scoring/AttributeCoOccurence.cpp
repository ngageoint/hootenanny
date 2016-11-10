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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AttributeCoOccurence.h"

// Hoot
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/scoring/TextTable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>

// tgs
#include <tgs/HashMap.h>


namespace hoot
{

/**
 * Traverses the OsmMap and creates a map from REF tags to ElementIds.
 */
class RefToEidVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:
  typedef map<QString, set<ElementId> > RefToEid;

  RefToEidVisitor(QString ref) : _ref(ref) {}

  virtual ~RefToEidVisitor() {}

  const RefToEid& getRefToEid() const { return _ref2Eid; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e)
  {
    QStringList refs;
    if (e->getTags().contains(_ref))
    {
      e->getTags().readValues(_ref, refs);
    }
    refs.removeAll("todo");
    refs.removeAll("none");

    if (refs.size() > 0)
    {
      for (int i = 0; i < refs.size(); i++)
      {
        _ref2Eid[refs[i]].insert(e->getElementId());
      }
    }
  }

private:
  const OsmMap* _map;
  QString _ref;
  RefToEid _ref2Eid;
};

/**
 * Traverses the OsmMap and build a hashmap of Attribute Co-Occurence values.
 */
class CoOccurenceVisitor : public ElementVisitor, public ConstOsmMapConsumer
{
public:
  // CoOccurenceVisitor(RefToEidVisitor::RefToEid refSet) : _refSet(refSet) {}
  CoOccurenceVisitor(RefToEidVisitor::RefToEid refSet, AttributeCoOccurence::CoOccurenceHash& h) : _refSet(refSet), _coOccurence(h) {}

  virtual ~CoOccurenceVisitor() {}

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e)
  {
    QStringList refs;
    if (e->getTags().contains("REF1"))
    {
      e->getTags().readValues("REF1", refs);

      refs.removeAll("todo");
      refs.removeAll("none");

      if (refs.size() > 0)
      {
        // Find the REF1 id's in REF2.
        // NOTE: this blindly assumes that there is only ONE value in the REF1 tag list
        RefToEidVisitor::RefToEid::const_iterator refId = _refSet.find(refs[0]);

        if (refId != _refSet.end())
        {
          // Loop through the element Id's in REF2
          for (set<ElementId>::const_iterator eid = refId->second.begin(); eid != refId->second.end(); ++eid)
          {
            // Loop through the Tags in REF1
            for (Tags::const_iterator tag1 = e->getTags().begin(); tag1 != e->getTags().end(); ++tag1 )
            {
              QString kvp1 = OsmSchema::getInstance().toKvp(tag1.key(),tag1.value());

              // We are only looking at Enumerated tags
              if (OsmSchema::getInstance().getTagVertex(kvp1).valueType == hoot::Enumeration)
              {
                // Get the value from the corresponding tag in REF2
                QString kvp2 = OsmSchema::getInstance().toKvp(tag1.key(), _map->getElement(*eid)->getTags()[tag1.key()]);

                // LOG_INFO("Got Tags:" + kvp1 + " " + kvp2);
                _coOccurence[kvp1][kvp2]++;
              }
            } // End for REF1 tag list

            // now loop through the REF2 tag list and fill in any missing tags.
            for (Tags::const_iterator tag2 = _map->getElement(*eid)->getTags().begin();
                 tag2 != _map->getElement(*eid)->getTags().end(); ++tag2 )
            {
              QString kvp2 = OsmSchema::getInstance().toKvp(tag2.key(),tag2.value());

              // Skip the tags that are common
              if (e->getTags().contains(tag2.key())) continue;

              if (OsmSchema::getInstance().getTagVertex(kvp2).valueType == hoot::Enumeration)
              {
                // "Missing" == "" tag value
                QString kvp1 = OsmSchema::getInstance().toKvp(tag2.key(),"");

                // LOG_INFO("Got Tags:" + kvp1 + " " + kvp2);
                _coOccurence[kvp1][kvp2]++;
              }

            } // End for REF2 tag list

            // now try matching up the "name" fields
            QString name1 = e->getTags()["name"];
            QString name2 = _map->getElement(*eid)->getTags()["name"];

            QString kvpNull = OsmSchema::getInstance().toKvp("name","<NULL>");
            QString kvpNonNull = OsmSchema::getInstance().toKvp("name","<NON NULL>");
            QString kvpSame = OsmSchema::getInstance().toKvp("name","<SIMILAR>");

            if (name1 == "")
            {
              if (name2 == "")
              {
                _coOccurence[kvpNull][kvpNull]++;
              }
              else
              {
                _coOccurence[kvpNull][kvpNonNull]++;
              }
            }
            else if (name2 == "")
            {
              _coOccurence[kvpNonNull][kvpNull]++;
            }
            else
            {
              // Check if the names match
              double nameScore = _calculateNameScore(e, _map->getElement(*eid));
              bool nameMatch = nameScore >= ConfigOptions().getPoiPolygonNameScoreThreshold();

              if (nameMatch)
              {
                _coOccurence[kvpSame][kvpSame]++;
              }
              else
              {
                _coOccurence[kvpNonNull][kvpNonNull]++;
              }
            }

          } // End for ref2 Element ID's

        } // End refId != End

      } // End refs > 0

    } // End contains "REF1"

  } // End Visit

private:
  const OsmMap* _map;
  RefToEidVisitor::RefToEid _refSet;
  AttributeCoOccurence::CoOccurenceHash& _coOccurence;

  double _calculateNameScore(ConstElementPtr e1, ConstElementPtr e2) const
  {
    // This was taken directly from PoiPolygonMatch.cpp:
    // "double PoiPolygonMatch::_calculateNameScore"

    // found experimentally when doing building name comparisons
    double score = NameExtractor(
          new TranslateStringDistance(
            new MeanWordSetDistance(new LevenshteinDistance(1.45)))).extract(e1, e2);

    return score;
  }

};


AttributeCoOccurence::AttributeCoOccurence() {}

void AttributeCoOccurence::addToMatrix(const ConstOsmMapPtr& in)

{
  RefToEidVisitor ref2("REF2");

  in->visitRo(ref2);

  CoOccurenceVisitor coOccurenceResult(ref2.getRefToEid(), _resultMatrix);

  in->visitRo(coOccurenceResult);
}


QString AttributeCoOccurence::printTable()
{
  TextTable::Data data;

  for(CoOccurenceHash::const_iterator it = _resultMatrix.begin(); it != _resultMatrix.end(); ++it)
  {
    // Get the list of keys and build a reverse matrix
    for (HashMap<QString, int>::const_iterator jt = it->second.begin();
         jt != it->second.end(); ++jt)
    {
      data[jt->first][it->first] = jt->second;
    }
  }

  TextTable table(data);

  return table.toWikiString();
}


QString AttributeCoOccurence::printList()
{
  QStringList keyList;
  QString result;

  // Build a list of REF1 keys so we can sort the table
  for(CoOccurenceHash::const_iterator it = _resultMatrix.begin(); it != _resultMatrix.end(); ++it)
  {
    keyList.append(it->first);
  }

  keyList.sort();

  // Print the Matrix as a list, sorted by REF1 keys
  result += "N  : REF1 -> REF2\n";

  for(int it=0; it < keyList.size(); it++)
  {
    CoOccurenceHash::const_iterator jt = _resultMatrix.find(keyList[it]);

    for (HashMap<QString, int>::const_iterator kt = jt->second.begin(); kt != jt->second.end(); ++kt)
    {
      result += QString("%1: %2 -> %3\n").arg(kt->second,-3,10).arg(keyList[it]).arg(kt->first);
    }
  }

  return result;
}

}
