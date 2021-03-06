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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "TagAdvancedCriterion.h"

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagAdvancedCriterion)

TagAdvancedCriterion::TagAdvancedCriterion() :
// not seeing a need to use anything other than case-insensitive here, but can make it
// configurable if needed
_keyMatcher(new QRegExp("*", Qt::CaseInsensitive, QRegExp::Wildcard)),
_valueMatcher(new QRegExp("*", Qt::CaseInsensitive, QRegExp::Wildcard))
{
  setConfiguration(conf());
}

TagAdvancedCriterion::TagAdvancedCriterion(const QString& filterJsonStringOrPath) :
_keyMatcher(new QRegExp("*", Qt::CaseInsensitive, QRegExp::Wildcard)),
_valueMatcher(new QRegExp("*", Qt::CaseInsensitive, QRegExp::Wildcard))
{
  _parseFilterString(filterJsonStringOrPath);
}

void TagAdvancedCriterion::setConfiguration(const Settings& s)
{
  // TODO: rename, since this crit isn't specific to conflation
  const QString tagFilter = ConfigOptions(s).getConflateTagFilter();
  if (!tagFilter.trimmed().isEmpty())
  {
    _parseFilterString(tagFilter);
  }
}

void TagAdvancedCriterion::_parseFilterString(const QString& filterJsonStringOrPath)
{
  std::shared_ptr<boost::property_tree::ptree> propTree;
  if (!filterJsonStringOrPath.toLower().endsWith(".json"))
  {
    propTree = StringUtils::jsonStringToPropTree(filterJsonStringOrPath);
  }
  else
  {
    propTree.reset(new boost::property_tree::ptree());
    try
    {
      boost::property_tree::read_json(filterJsonStringOrPath.toStdString(), *propTree);
    }
    catch (boost::property_tree::json_parser::json_parser_error& e)
    {
      throw HootException(
        QString("Error parsing JSON: %1 (line %2)")
          .arg(QString::fromStdString(e.message()))
          .arg(QString::number(e.line())));
    }
    catch (const std::exception& e)
    {
      const QString reason = e.what();
      throw HootException("Error parsing JSON " + reason);
    }
  }

  _tagFilters.clear();
  _loadTagFilters("must", propTree);
  _loadTagFilters("should", propTree);
  _loadTagFilters("must_not", propTree);

  if (_tagFilters["must"].size() == 0 && _tagFilters["must_not"].size() == 0 &&
      _tagFilters["should"].size() == 0)
  {
    throw IllegalArgumentException("Empty tag filter specified.");
  }
}

void TagAdvancedCriterion::_loadTagFilters(
  const QString& tagFilterType, const std::shared_ptr<boost::property_tree::ptree>& propTree)
{
  LOG_TRACE("Loading " << tagFilterType << " filters...");

  _tagFilters[tagFilterType].clear();
  boost::optional<boost::property_tree::ptree&> tagFilterChild =
    propTree->get_child_optional(tagFilterType.toStdString());
  if (tagFilterChild)
  {
    for (boost::property_tree::ptree::value_type& tagFilterPart :
         propTree->get_child(tagFilterType.toStdString()))
    {
      TagFilter tagFilter = TagFilter::fromJson(tagFilterPart);
      LOG_VART(tagFilter);
      _tagFilters[tagFilterType].append(tagFilter);
    }
  }
  LOG_TRACE(tagFilterType << "filters: " << _tagFilters[tagFilterType].size());
}

bool TagAdvancedCriterion::_hasAuxMatch(const ConstElementPtr& e, const TagFilter& filter,
                                        const QString& matchType) const
{
  LOG_TRACE("Checking for tag " << matchType << " match...");

  Tags filterTags;
  QString filterKey = filter.getKey();
  // We don't support kvp wildcards when we're looking for aux style matches.  So if the filter
  // specifies a wildcard, we'll simply remove it.
  if (filterKey != "*")
  {
    filterKey = filterKey.remove("*");
  }
  QString filterVal = filter.getValue();
  if (filterVal != "*")
  {
    filterVal = filterVal.remove("*");
  }
  filterTags.appendValue(filterKey, filterVal);

  // Here we finding a set of tags as defined by the type of match we are looking for, then for
  // each tag found, create a tag filter with the kvp and call back into _filterMatchesAnyTag with
  // that filter.

  Tags tags;
  if (matchType.toLower() == "alias")
  {
    tags = OsmSchema::getInstance().getAliasTags(filterTags);
  }
  else if (matchType.toLower() == "similar")
  {
    tags =
      OsmSchema::getInstance().getSimilarTags(
        filter.getKey() + "=" + filter.getValue(), filter.getSimilarityThreshold());
  }
  else if (matchType.toLower() == "child")
  {
    tags = OsmSchema::getInstance().getChildTags(filterTags);
  }
  else if (matchType.toLower() == "ancestor")
  {
    for (Tags::const_iterator tagItr = e->getTags().begin(); tagItr != e->getTags().end();
         ++tagItr)
    {
      if (OsmSchema::getInstance().isAncestor(
            filterKey + "=" + filterVal, tagItr.key() + "=" + tagItr.value()))
      {
        LOG_TRACE("Found " << matchType << " match.");
        return true;
      }
    }
    return false;
  }
  else if (matchType.toLower() == "association")
  {
    tags = OsmSchema::getInstance().getAssociatedTags(filterTags);
  }
  else if (matchType.toLower() == "category")
  {
    if (OsmSchema::getInstance().hasCategory(e->getTags(), filter.getCategory().toString()))
    {
      LOG_TRACE("Found " << matchType << " match.");
      return true;
    }
    return false;
  }
  else
  {
    throw IllegalArgumentException("Invalid aux tag match type: " + matchType);
  }
  LOG_VART(tags);

  for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
  {
    const QString tagKey = tagItr.key();
    const QString tagValue = tagItr.value();
    const QStringList tagValues = tagValue.split(";");
    for (int i = 0; i < tagValues.length(); i++)
    {
      const QString splitValue = tagValues.at(i).trimmed().toLower();
      if (!splitValue.isEmpty() && _filterMatchesAnyTag(TagFilter(tagKey, splitValue), e->getTags()))
      {
        LOG_TRACE("Found " << matchType << " match.");
        return true;
      }
    }
  }

  return false;
}

bool TagAdvancedCriterion::_elementPassesTagFilter(const ConstElementPtr& e,
                                                   const TagFilter& filter) const
{
  LOG_VART(e->getTags());
  LOG_VART(filter);
  bool foundFilterMatch = false;

  // If the kvp filter is empty, then the filter must be a category filter, so we skip to that.
  if (!(filter.getKey().isEmpty() && filter.getValue().isEmpty()))
  {
    // Trying to match the tag kvp first, then if no match is found, checking each additional aux
    // match type based on the filter's content.

    LOG_TRACE("Checking for tag match...");
    foundFilterMatch = _filterMatchesAnyTag(filter, e->getTags());
    if (foundFilterMatch)
    {
      LOG_TRACE("Found tag match.");
    }

    // there may be a way to make this section cleaner

    LOG_VART(filter.getAllowAliases());
    if (!foundFilterMatch && filter.getAllowAliases())
    {
      foundFilterMatch = _hasAuxMatch(e, filter, "alias");
    }

    LOG_VART(filter.getSimilarityThreshold());
    if (!foundFilterMatch && filter.getSimilarityThreshold() != -1.0)
    {
      foundFilterMatch = _hasAuxMatch(e, filter, "similar");
    }

    LOG_VART(filter.getAllowChildren());
    if (!foundFilterMatch && filter.getAllowChildren())
    {
      foundFilterMatch = _hasAuxMatch(e, filter, "child");
    }

    LOG_VART(filter.getAllowAncestors());
    if (!foundFilterMatch && filter.getAllowAncestors())
    {
      foundFilterMatch = _hasAuxMatch(e, filter, "ancestor");
    }

    LOG_VART(filter.getAllowAssociations());
    if (!foundFilterMatch && filter.getAllowAssociations())
    {
      foundFilterMatch = _hasAuxMatch(e, filter, "association");
    }
  }

  LOG_VART(filter.getCategory());
  if (!foundFilterMatch && filter.getCategory() != OsmSchemaCategory::Empty)
  {
    foundFilterMatch = _hasAuxMatch(e, filter, "category");
  }

  return foundFilterMatch;
}

bool TagAdvancedCriterion::_filterMatchesAnyTag(const TagFilter& filter, const Tags& tags) const
{
  LOG_VART(tags);

  bool keyMatched = false;
  LOG_VART(filter.getKey());
  if (filter.getKey() == "*")
  {
    LOG_TRACE("Key is generic wildcard.  Key matched.")
    keyMatched = true;
  }
  else if (tags.contains(filter.getKey()))
  {
    LOG_TRACE("Tags contain key exact match of: " << filter.getKey());
    keyMatched = true;
  }

  bool valueMatched = false;
  LOG_VART(filter.getValue());
  if (filter.getValue() == "*")
  {
    LOG_TRACE("Value is generic wildcard.  Value matched.");
    valueMatched = true;
  }
  else if (tags.get(filter.getKey()) == filter.getValue())
  {
    LOG_TRACE(
      "Tags contain exact value match of: " << filter.getValue() << " for key: " <<
      filter.getKey());
    valueMatched = true;
  }

  LOG_VART(keyMatched);
  LOG_VART(valueMatched);
  if (!keyMatched || !valueMatched)
  {
    _keyMatcher->setPattern(filter.getKey());
    _valueMatcher->setPattern(filter.getValue());

    for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
    {
      LOG_VART(tagItr.key());
      if (keyMatched || (!keyMatched && _keyMatcher->exactMatch(tagItr.key())))
      {
        LOG_TRACE("Tags match key on wildcard for key: " << filter.getKey());
        keyMatched = true;

        LOG_VART(tagItr.value());
        if (!valueMatched && _valueMatcher->exactMatch(tagItr.value()))
        {
          LOG_TRACE(
            "Tags match value on wildcard for key: " << filter.getKey() << " and value: " <<
            filter.getValue());
          valueMatched = true;
        }
      }

      if (keyMatched && valueMatched)
      {
        LOG_TRACE("Matched both key and value.");
        return true;
      }
    }
  }

  return keyMatched && valueMatched;
}

bool TagAdvancedCriterion::_elementPassesMustTagFilters(const ConstElementPtr& e) const
{
  const int filterSize = _tagFilters["must"].size();
  if (filterSize > 0)
  {
    LOG_TRACE("Checking " << filterSize << " 'must' filters...");

    // must pass all of these
    for (int i = 0; i < filterSize; i++)
    {
      if (!_elementPassesTagFilter(e, _tagFilters["must"].at(i)))
      {
        LOG_TRACE("Tag filtering failed a \"must\" criterion.");
        return false;
      }
    }
    LOG_TRACE("Tag filtering passed all \"must\" criteria.");
  }
  return true;
}

bool TagAdvancedCriterion::_elementPassesMustNotTagFilters(const ConstElementPtr& e) const
{
  const int filterSize = _tagFilters["must_not"].size();
  if (filterSize > 0)
  {
    LOG_TRACE("Checking " << filterSize << " 'must not' filters...");

    // must pass none of these
    for (int i = 0; i < filterSize; i++)
    {
      if (_elementPassesTagFilter(e, _tagFilters["must_not"].at(i)))
      {
        LOG_TRACE("Tag filtering failed a \"must not\" criterion.");
        return false;
      }
    }
    LOG_TRACE("Tag filtering passed all \"must not\" criteria.");
  }
  return true;
}

bool TagAdvancedCriterion::_elementPassesShouldTagFilters(const ConstElementPtr& e) const
{
  const int filterSize = _tagFilters["should"].size();
  if (filterSize > 0)
  {
    LOG_TRACE("Checking " << filterSize << " 'should' filters...");

    // just needs to pass any one of these
    for (int i = 0; i < filterSize; i++)
    {
      if (_elementPassesTagFilter(e, _tagFilters["should"].at(i)))
      {
        LOG_TRACE("Tag filtering passed on a \"should\" criterion.");
        return true;
      }
    }
    LOG_TRACE("Tag filtering failed to pass on any 'should' criterion.");
    return false;
  }
  return true;
}

bool TagAdvancedCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (!_elementPassesMustTagFilters(e))
  {
    return false;
  }
  if (!_elementPassesMustNotTagFilters(e))
  {
    return false;
  }
  if (!_elementPassesShouldTagFilters(e))
  {
    return false;
  }

  LOG_TRACE("Tag filtering passed all criteria.");
  return true;
}

}
