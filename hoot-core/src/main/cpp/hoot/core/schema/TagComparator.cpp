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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#include "TagComparator.h"

// Hoot
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/language/ToEnglishDictionaryTranslator.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>

// Standard
#include <assert.h>
#include <queue>

using namespace std;

namespace hoot
{

struct Entry
{
  int i, j;
  double score;

  bool operator()(const Entry& left, const Entry& right) const
  {
    return left.score < right.score;
  }
};

void TagComparator::_addAsDefault(Tags& t, const QString& key, const QString& value) const
{
  if (t.contains(key) == false || t[key].isEmpty() == true)
    t[key] = value;
}

void TagComparator::_addDefaults(Tags& t) const
{
  if (t.contains("highway"))
  {
    _addAsDefault(t, "bridge", "no");
    _addAsDefault(t, "tunnel", "no");
    _addAsDefault(t, "oneway", "no");
  }
}

void TagComparator::_addNonConflictingTags(Tags& t1, const Tags& t2, Tags& result) const
{
  const OsmSchema& schema = OsmSchema::getInstance();

  // we're deleting as we iterate so be careful making changes.
  for (auto it1 = t1.begin(); it1 != t1.end(); )
  {
    QString kvp1 = it1.key() + "=" + it1.value();
    bool conflict = false;
    for (auto it2 = t2.begin(); it2 != t2.end(); ++it2)
    {
      QString kvp2 = it2.key() + "=" + it2.value();
      if (schema.score(kvp1, kvp2) > 0.0)
      {
        conflict = true;
        break;
      }
    }

    if (conflict)
      ++it1;
    else
    {
      result[it1.key()] = it1.value();
      t1.erase(it1++);
    }
  }
}

void TagComparator::averageTags(const Tags& t1, const Tags& t2, Tags& result, bool keepAllUnknownTags, bool caseSensitive)
{
  averageTags(t1, 1.0, t2, 1.0, result, keepAllUnknownTags, caseSensitive);
}

void TagComparator::averageTags(const Tags& t1In, double w1, const Tags& t2In, double w2, Tags& result, bool keepAllUnknownTags,
                                bool caseSensitive)
{
  result.clear();
  const OsmSchema& schema = OsmSchema::getInstance();

  Tags t1 = t1In;
  Tags t2 = t2In;

  set<QString> k1, k2;

  mergeNames(t1, t2, result, QStringList()/*FIXEME*/, caseSensitive);

  // Merge any text fields by concatenating the lists.
  mergeText(t1, t2, result, QStringList()/*FIXEME*/, caseSensitive);

  if (keepAllUnknownTags)
    _mergeUnrecognizedTags(t1, t2, result);

  for (auto it1 = t1.begin(); it1 != t1.end(); ++it1)
  {
    QString kvp1 = it1.key() + "=" + it1.value();
    QString kvp2;
    double bestScore = 0;
    QString bestKvp;
    QString bestK2;
    for (auto it2 = t2.begin(); it2 != t2.end(); ++it2)
    {
      kvp2 = it2.key() + "=" + it2.value();
      double score;
      QString thisKvp = schema.average(kvp1, w1, kvp2, w2, score);
      if (score > bestScore && k2.find(it2.key()) == k2.end())
      {
        bestScore = score;
        bestKvp = thisKvp;
        bestK2 = it2.key();
      }
    }
    if (bestKvp.isEmpty() == false)
    {
      k1.insert(it1.key());
      k2.insert(bestK2);

      if (bestKvp.contains("="))
      {
        QStringList sl = bestKvp.split("=");
        result[sl[0]] = sl[1];
      }
      else
        result[it1.key()] = it1.value();
    }
  }

  for (auto it2 = t2.begin(); it2 != t2.end(); ++it2)
  {
    if (k2.find(it2.key()) == k2.end())
      result[it2.key()] = it2.value();
  }

  for (auto it1 = t1.begin(); it1 != t1.end(); ++it1)
  {
    if (k1.find(it1.key()) == k1.end())
      result[it1.key()] = it1.value();
  }
}

void TagComparator::compareEnumeratedTags(Tags t1, Tags t2, double& score, double& weight) const
{
  const OsmSchema& schema = OsmSchema::getInstance();
  score = 1.0;
  weight = 0;

  // Put all the enumerated tags in a list.
  vector<QString> n1, n2;

  _addDefaults(t1);
  _addDefaults(t2);

  vector<SchemaVertex> v1 = schema.getUniqueSchemaVertices(t1);
  vector<SchemaVertex> v2 = schema.getUniqueSchemaVertices(t2);

  for (const auto& tv : v1)
  {
    if (tv.getValueType() == Enumeration)
    {
      if (tv.getValue() == "*")
        n1.push_back(OsmSchema::toKvp(tv.getKey(), t1[tv.getKey()]));
      else
        n1.push_back(tv.getName());
    }
  }

  for (const auto& tv : v2)
  {
    if (tv.getValueType() == Enumeration)
    {
      if (tv.getValue() == "*")
        n2.push_back(OsmSchema::toKvp(tv.getKey(), t2[tv.getKey()]));
      else
        n2.push_back(tv.getName());
    }
  }

  priority_queue<Entry, deque<Entry>, Entry> heap;

  // create a n x m matrix of scores
  vector<vector<double>> scores;
  scores.resize(n1.size());
  Entry e;
  for (size_t i = 0; i < n1.size(); i++)
  {
    scores[i].resize(n2.size());
    e.i = static_cast<int>(i);
    for (size_t j = 0; j < n2.size(); j++)
    {
      e.j = static_cast<int>(j);
      e.score = schema.score(n1[i], n2[j]);
      LOG_TRACE("n1: " << n1[i] << " n2: " << n2[j] << " " << e.score);
      heap.push(e);
    }
  }

  set<int> used1;
  set<int> used2;

  while (!heap.empty())
  {
    e = heap.top();
    heap.pop();
    if (e.score > 0.0 && used1.find(e.i) == used1.end() && used2.find(e.j) == used2.end())
    {
      LOG_TRACE("  " << n1[e.i] << ", " << n2[e.j] << ": " << e.score);
      score *= e.score;
      used1.insert(e.i);
      used2.insert(e.j);
    }
  }

  weight = static_cast<double>(used1.size());
  LOG_TRACE("score: " << score);
}

void TagComparator::compareTextTags(const Tags& t1, const Tags& t2, double& score, double& weight) const
{
  const OsmSchema& schema = OsmSchema::getInstance();

  score = 1.0;
  weight = 0.0;

  for (auto it = t1.begin(); it != t1.end(); ++it)
  {
    const SchemaVertex& tv = schema.getTagVertex(it.key());
    if (schema.isAncestor(it.key(), "abstract_name") == false && tv.getValueType() == Text && t2.contains(it.key()))
    {
      score *= LevenshteinDistance::score(it.value(), t2[it.key()]);
      weight += tv.getInfluence();
    }
  }

  // if the weight is zero don't confuse things with a low score.
  if (weight == 0.0)
    score = 1;
}

void TagComparator::compareNames(const Tags& t1, const Tags& t2, double& score, double& weight, bool strict) const
{
  // Check out picard's coefficient or sum(top scores) / min(t1.count, t2.count) score for the same
  // and score for different.
  score = 0;
  weight = 0;

  QStringList n1 = t1.getNames();
  QStringList n2 = t2.getNames();

  ToEnglishDictionaryTranslator translator;
  for (int i = 0; i < n1.size(); i++)
    n1[i] = translator.translateStreet(n1[i]);

  for (int i = 0; i < n2.size(); i++)
    n2[i] = translator.translateStreet(n2[i]);

  priority_queue<Entry, deque<Entry>, Entry> heap;

  // create a n x m matrix of scores
  vector<vector<double>> scores;
  scores.resize(n1.size());
  Entry e;
  for (int i = 0; i < n1.size(); i++)
  {
    scores[i].resize(n2.size());
    e.i = i;
    for (int j = 0; j < n2.size(); j++)
    {
      e.j = j;
      e.score = LevenshteinDistance::score(n1[i], n2[j]);
      //LOG_INFO("n1: " << n1[i].toStdString() << " n2: " << n2[j].toStdString() << " " << e.score);
      heap.push(e);
    }
  }

  int scoreCount = (int)((double)std::min(n1.size(), n2.size()) / 2.0 + 0.5);
  weight = scoreCount;

  set<int> used1;
  set<int> used2;

  while (scoreCount > 0)
  {
    assert(heap.size() > 0);

    e = heap.top();
    heap.pop();
    if (used1.find(e.i) == used1.end() && used2.find(e.j) == used2.end())
    {
      LOG_TRACE("  " << n1[e.i].toStdString() << ", " << n2[e.j].toStdString() << " " << e.score);
      score += e.score;
      used1.insert(e.i);
      used2.insert(e.j);
      scoreCount--;
    }
  }

  if (weight > 0)
    score /= weight;
  else if (strict && (!n1.empty()) != (!n2.empty()))  // if this is strict checking and one entry doesn't have a name.
    score = 0.2;
  else
    score = 1;
}

double TagComparator::compareTags(const Tags &t1, const Tags &t2, bool strict) const
{
  // compare and get a score for name comparison
  double nameScore, nameWeight;
  compareNames(t1, t2, nameScore, nameWeight, strict);
  LOG_TRACE("Name score: " << nameScore << "(" << nameWeight << ")");

  double textScore, textWeight;
  compareTextTags(t1, t2, textScore, textWeight);
  LOG_TRACE("Text score: " << textScore << " (" << textWeight << ")");

  // compare the enumerated tags
  double enumScore, enumWeight;
  compareEnumeratedTags(t1, t2, enumScore, enumWeight);
  LOG_TRACE("enumScore: " << enumScore << "(" << enumWeight << ")");

  // comparing numerical tags is difficult without some concept of the distribution. For that
  // reason I'm avoiding it for now.

  if (nameWeight + enumWeight <= 0)
    return -1;
  else
  {
    LOG_VART(nameScore);
    LOG_VART(enumScore);
    LOG_VART(textScore);
    return nameScore * enumScore * textScore;
  }
}

bool TagComparator::nonNameTagsExactlyMatch(const Tags& t1, const Tags& t2, bool caseSensitive) const
{
  const Qt::CaseSensitivity caseSensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

  Tags t1Filtered;
  for (auto it1 = t1.begin(); it1 != t1.end(); ++it1)
  {
    QString key = it1.key();
    QString value = it1.value();
    //  Metadata keys are controlled by hoot and, therefore, should always be lower case, so no
    //  case check needed.
    if (!Tags::getNameKeys().contains(key, caseSensitivity) && !OsmSchema::getInstance().isMetaData(key, value))
    {
      if (!caseSensitive)
      {
        key = key.toUpper();
        value = value.toUpper();
      }
      t1Filtered.insert(key, value);
    }
  }

  Tags t2Filtered;
  for (auto it2 = t2.begin(); it2 != t2.end(); ++it2)
  {
    QString key = it2.key();
    QString value = it2.value();
    if (!Tags::getNameKeys().contains(key, caseSensitivity) && !OsmSchema::getInstance().isMetaData(key, value))
    {
      if (!caseSensitive)
      {
        key = key.toUpper();
        value = value.toUpper();
      }
      t2Filtered.insert(key, value);
    }
  }

  return t1Filtered == t2Filtered;
}

TagComparator& TagComparator::getInstance()
{
  //  Local static singleton instance
  static TagComparator instance;
  return instance;
}

Tags TagComparator::generalize(Tags t1, Tags t2, bool overwriteUnrecognizedTags, bool caseSensitive)
{
  // NOTE: Not using const references on the inputs b/c we'd have to copy anyway.

  Tags result;

  // Names are merged using _mergeNames.
  mergeNames(t1, t2, result, QStringList(), caseSensitive);

  // Exact matches are unchanged
  _mergeExactMatches(t1, t2, result);

  // Merge any text fields by adding to a list.
  mergeText(t1, t2, result, QStringList(), caseSensitive);

  // Unrecognized tags are concatenated in a list.
  if (overwriteUnrecognizedTags)
    _overwriteUnrecognizedTags(t1, t2, result);
  else
    _mergeUnrecognizedTags(t1, t2, result);

  // Tags that share an ancestor are promoted to the first common ancestor
  _promoteToCommonAncestor(t1, t2, result);

  // If there are no conflicting tags, the tag is kept.
  _addNonConflictingTags(t1, t2, result);
  _addNonConflictingTags(t2, t1, result);

  return result;
}

void TagComparator::_mergeExactMatches(Tags& t1, Tags& t2, Tags& result) const
{
  const OsmSchema& schema = OsmSchema::getInstance();

  Tags t1Copy = t1;
  for (auto it1 = t1Copy.begin(); it1 != t1Copy.end(); ++it1)
  {
    auto it2 = t2.find(it1.key());
    if (it2 != t2.end())
    {
      bool keepIt = false;
      if (schema.isList(it1.key(), it1.value()))
      {
        // treat the inputs as unordered lists
        QSet<QString> values1 = QSet<QString>::fromList(t1.getList(it1.key()));
        QSet<QString> values2 = QSet<QString>::fromList(t2.getList(it2.key()));
        values1.intersect(values2);
        if (values1.size() == values2.size())
          keepIt = true;
      }
      else if (it2.value() == it1.value())
        keepIt = true;

      if (keepIt)
      {
        result[it1.key()] = it1.value();
        t1.remove(it1.key());
        t2.remove(it1.key());
      }
    }
  }
}

void TagComparator::mergeNames(Tags& t1, Tags& t2, Tags& result, const QStringList& overwriteExcludeTagKeys, bool caseSensitive) const
{
  LOG_TRACE("Merging names...");
  LOG_VART(t1);
  LOG_VART(t2);
  LOG_VART(result);
  LOG_VART(overwriteExcludeTagKeys);

  set<QString> altNames, nonAltNames;
  set<QString> toRemove;
  const Qt::CaseSensitivity caseSensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
  LOG_VART(caseSensitive);

  toRemove.insert("alt_name");

  for (auto it1 = t1.begin(); it1 != t1.end(); ++it1)
  {
    //LOG_VART(it1.key());
    //LOG_VART(it1.value());
    if (it1.key() == "alt_name" && !overwriteExcludeTagKeys.contains("alt_name"))
    {
      QStringList sl = Tags::split(it1.value());
      altNames.insert(sl.begin(), sl.end());
    }
    else if (OsmSchema::getInstance().isAncestor(it1.key(), "abstract_name") &&
             !overwriteExcludeTagKeys.contains(it1.key(), caseSensitivity))
    {
      result[it1.key()] = it1.value();
      QStringList sl = Tags::split(it1.value());
      // keep track of all the names we've used
      nonAltNames.insert(sl.begin(), sl.end());
      toRemove.insert(it1.key());
    }
  }
//  LOG_VART(altNames);
//  LOG_VART(nonAltNames);
//  LOG_VART(toRemove);
//  LOG_VART(result);

  for (auto it2 = t2.begin(); it2 != t2.end(); ++it2)
  {
    //LOG_VART(it2.key());
    //LOG_VART(it2.value());
    if (it2.key() == "alt_name")
    {
      QStringList sl = Tags::split(it2.value());
      altNames.insert(sl.begin(), sl.end());
    }
    else if (result.contains(it2.key()))
    {
      if (result[it2.key()].compare(it2.value(), caseSensitivity) != 0)
      {
        QStringList sl = Tags::split(it2.value());
        altNames.insert(sl.begin(), sl.end());
      }
    }
    else if (OsmSchema::getInstance().isAncestor(it2.key(), "abstract_name"))
    {
      result[it2.key()] = it2.value();
      QStringList sl = Tags::split(it2.value());
      nonAltNames.insert(sl.begin(), sl.end());
      toRemove.insert(it2.key());
    }
  }
//  LOG_VART(altNames);
//  LOG_VART(nonAltNames);
//  LOG_VART(toRemove);
//  LOG_VART(result);

  for (auto it = toRemove.begin(); it != toRemove.end(); ++it)
  {
    t1.remove(*it);
    t2.remove(*it);
  }
//  LOG_VART(t1);
//  LOG_VART(t2);

  // add all the altNames that don't exist in nonAltNames
  QStringList l;
  for (const auto& name : altNames)
  {
    if (nonAltNames.find(name) == nonAltNames.end())
      l.append(name);
  }
  //LOG_VART(l);

  if (!l.empty())
    result.setList("alt_name", l);

  LOG_VART(result);
}

void TagComparator::mergeText(Tags& t1, Tags& t2, Tags& result, const QStringList& overwriteExcludeTagKeys,
                              bool caseSensitive) const
{
  LOG_TRACE("Merging text...");
  LOG_VART(t1);
  LOG_VART(t2);
  const OsmSchema& schema = OsmSchema::getInstance();

  const Tags t1Copy = t1;
  for (auto it1 = t1Copy.begin(); it1 != t1Copy.end(); ++it1)
  {
    const SchemaVertex& tv = schema.getTagVertex(it1.key());

    // if this is a text field and it exists in both tag sets.
    if (tv.getValueType() == Text && t2.contains(it1.key()))
    {
      // only keep the unique text fields
      QStringList values1 = t1.getList(it1.key());
      QStringList values2 = t2.getList(it1.key());

      // append all unique values in the existing order; don't overwrite tags in t2 that are in the
      // exclude list
      const Qt::CaseSensitivity caseSensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
      for (const auto& value : qAsConst(values1))
      {
        if (value.isEmpty() == false && (!t2.contains(it1.key()) || !overwriteExcludeTagKeys.contains(it1.key(), caseSensitivity)))
          result.appendValueIfUnique(it1.key(), value);
      }
      for (const auto& value : qAsConst(values2))
      {
        if (value.isEmpty() == false)
          result.appendValueIfUnique(it1.key(), value);
      }

      t1.remove(it1.key());
      t2.remove(it1.key());
    }
  }
  LOG_VART(result);
}

void TagComparator::_mergeUnrecognizedTags(Tags& t1, Tags& t2, Tags& result)
{
  const OsmSchema& schema = OsmSchema::getInstance();

  const Tags t1Copy = t1;
  for (auto it1 = t1Copy.begin(); it1 != t1Copy.end(); ++it1)
  {
    // if this is an unknown type
    if (schema.getTagVertex(it1.key() + "=" + it1.value()).isEmpty() &&
        schema.getTagVertex(it1.key()).isEmpty())
    {
      // if this is also in t2.
      if (t2.contains(it1.key()))
      {
        // get the set of all values.
        QSet<QString> values = _toSet(t1, it1.key());
        values.unite(_toSet(t2, it1.key()));
        QList<QString> sortEm = values.toList();
        qSort(sortEm);

        // remove it from the inputs
        t1.remove(it1.key());
        t2.remove(it1.key());
        // set the united set in the output
        result.set(it1.key(), sortEm.begin(), sortEm.end());
      }
      else
        result[it1.key()] = it1.value();
    }
  }

  // go through any remaining tags in t2
  const Tags t2Copy = t2;
  for (auto it2 = t2Copy.begin(); it2 != t2Copy.end(); ++it2)
  {
    // if this is an unknown type
    if (schema.getTagVertex(it2.key() + "=" + it2.value()).isEmpty())
    {
      // we know it isn't in t1, or it would have been handled in the above loop so just deal with t2
      t2.remove(it2.key());
      result[it2.key()] = it2.value();
    }
  }
}

Tags TagComparator::overwriteMerge(Tags t1, Tags t2, const QStringList& overwriteExcludeTagKeys,
                                   const QStringList& accumulateValuesTagKeys, bool caseSensitive)
{ 
  Tags result;

  // Names are merged using _mergeNames.
  mergeNames(t1, t2, result, overwriteExcludeTagKeys, caseSensitive);

  // concatenate the known text fields (e.g. note)
  mergeText(t1, t2, result, overwriteExcludeTagKeys, caseSensitive);

  // use the tags in t1 first, then fall back to tags in t2
  _overwriteRemainingTags(t1, t2, result, overwriteExcludeTagKeys, accumulateValuesTagKeys, caseSensitive);

  return result;
}

Tags TagComparator::replaceMerge(const Tags& t1, const Tags& t2, const QStringList& overwriteExcludeTagKeys, bool caseSensitive)
{
  Tags result;

  // We're keeping t1, so start with them as the result.
  result = t1;

  // Now see if there are any configured exclusions we should keep from t2.
  if (!overwriteExcludeTagKeys.empty())
  {
    const Qt::CaseSensitivity caseSensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    for (auto it = t2.constBegin(); it != t2.constEnd(); ++it)
    {
      if (!it.value().isEmpty() && (result.contains(it.key()) && overwriteExcludeTagKeys.contains(it.key(), caseSensitivity)))
        result[it.key()] = it.value();
    }
  }

  return result;
}

void TagComparator::_overwriteRemainingTags(Tags& t1, Tags& t2, Tags& result, const QStringList& overwriteExcludeTagKeys,
                                            const QStringList& accumulateValuesTagKeys, bool caseSensitive) const
{
  LOG_TRACE("Overwriting remaining tags...");

  LOG_VART(t1);
  LOG_VART(t2);
  LOG_VART(overwriteExcludeTagKeys);
  LOG_VART(accumulateValuesTagKeys);
  LOG_VART(caseSensitive);

  // Add t2 tags
  for (auto it2 = t2.constBegin(); it2 != t2.constEnd(); ++it2)
  {
    if (it2.value().isEmpty() == false)
      result[it2.key()] = it2.value();
  }

  // Add t1 tags overwriting any t2 tags in the process.
  const Qt::CaseSensitivity caseSensitivity = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
  for (auto it1 = t1.constBegin(); it1 != t1.constEnd(); ++it1)
  {
    LOG_VART(it1.key());
    LOG_VART(it1.value());
    LOG_VART(overwriteExcludeTagKeys.contains(it1.key(), caseSensitivity));
    LOG_VART(accumulateValuesTagKeys.contains(it1.key(), caseSensitivity));
    LOG_VART(result.contains(it1.key()));

    const bool tagsHaveKey = result.contains(it1.key());
    if (it1.value().isEmpty() == false && (!tagsHaveKey || // Do not overwrite any tags whose keys are exclude list.
         !overwriteExcludeTagKeys.contains(it1.key(), caseSensitivity)))
    {
      if (!tagsHaveKey || !accumulateValuesTagKeys.contains(it1.key(), caseSensitivity))
        result[it1.key()] = it1.value();
      else
      {
        // If the tag was marked for preservation by accumulation, append values to each other
        // rather than overwriting them.
        result[it1.key()] = result[it1.key()] + ";" + it1.value();
      }
    }
  }
  LOG_VART(result);

  t1.clear();
  t2.clear();
}

void TagComparator::_overwriteUnrecognizedTags(Tags& t1, Tags& t2, Tags& result) const
{
  const OsmSchema& schema = OsmSchema::getInstance();

  const Tags t1Copy = t1;
  for (auto it1 = t1Copy.begin(); it1 != t1Copy.end(); ++it1)
  {
    // if this is an unknown type
    if (schema.getTagVertex(it1.key() + "=" + it1.value()).isEmpty() &&
        schema.getTagVertex(it1.key()).isEmpty() &&
        // if this is also in t2
        t2.contains(it1.key()))
    {
      result[it1.key()] = it1.value();
      t1.remove(it1.key());
      t2.remove(it1.key());
    }
  }

  // go through any remaining tags in t2
  const Tags t2Copy = t2;
  for (auto it2 = t2Copy.begin(); it2 != t2Copy.end(); ++it2)
  {
    // if this is an unknown type
    if (schema.getTagVertex(it2.key() + "=" + it2.value()).isEmpty())
    {
      // we know it isn't in t1, or it would have been handled in the above loop so just deal with
      // t2
      t2.remove(it2.key());
      result[it2.key()] = it2.value();
    }
  }
}

void TagComparator::_promoteToCommonAncestor(Tags& t1, Tags& t2, Tags& result) const
{
  const OsmSchema& schema = OsmSchema::getInstance();

  // we're deleting as we iterate so be careful making changes.
  for (auto it1 = t1.begin(); it1 != t1.end(); )
  {
    for (auto it2 = t2.begin(); it2 != t2.end(); )
    {
      const SchemaVertex& ancestor = schema.getFirstCommonAncestor(it1.key() + "=" + it1.value(), it2.key() + "=" + it2.value());
      if (ancestor.isEmpty() == false)
      {
        // erase from the iterators in a safe way
        t1.erase(it1++);
        t2.erase(it2++);
        if (ancestor.getValue().isEmpty() == false)
          result[ancestor.getKey()] = ancestor.getValue();
      }
      else  // if we didn't erase anything then increment the iterators.
        ++it2;
    }
    if (it1 != t1.end())
      ++it1;
  }
}

QSet<QString> TagComparator::_toSet(const Tags& t, const QString& k)
{
  auto it = t.find(k);
  if (OsmSchema::getInstance().isList(k, *it))
    return QSet<QString>::fromList(t.getList(k));

  QSet<QString> result;
  result.insert(*it);
  return result;
}

}
