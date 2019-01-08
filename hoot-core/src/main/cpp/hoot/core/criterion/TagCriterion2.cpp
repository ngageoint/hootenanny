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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "TagCriterion2.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/schema/OsmSchema.h>

// Boost
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

// Qt
#include <QRegExp>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, TagCriterion2)

TagCriterion2::TagCriterion2()
{
  setConfiguration(conf());
}

TagCriterion2::TagCriterion2(const QString filter)
{
  _parseFilterString(filter);
}

void TagCriterion2::setConfiguration(const Settings& s)
{
  _parseFilterString(ConfigOptions(s).getConflateTagFilter());
}

void TagCriterion2::_parseFilterString(const QString filterStr)
{
  const boost::shared_ptr<pt::ptree> propTree = StringUtils::jsonStringToPropTree(filterStr);
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

void TagCriterion2::_loadTagFilters(const QString tagFilterType,
                                    boost::shared_ptr<pt::ptree> propTree)
{
  LOG_TRACE("Loading " << tagFilterType << " filters...");

  _tagFilters[tagFilterType].clear();
  boost::optional<pt::ptree&> tagFilterChild =
    propTree->get_child_optional(tagFilterType.toStdString());
  if (tagFilterChild)
  {
    for (pt::ptree::value_type& tagFilterPart : propTree->get_child(tagFilterType.toStdString()))
    {
      TagFilter tagFilter = TagFilter::fromJson(tagFilterPart);
      LOG_VART(tagFilter);
      _tagFilters[tagFilterType].append(tagFilter);
    }
  }
  LOG_TRACE(tagFilterType << "filters: " << _tagFilters[tagFilterType].size());
}

bool TagCriterion2::_hasAuxMatch(const ConstElementPtr& e, const TagFilter& filter,
                                 const QString matchType) const
{
  LOG_TRACE("Checking for tag " << matchType << " match...");

  Tags filterTags;
  if (filter.getKey() != "*" && filter.getValue() != "*")
  {
    filterTags.appendValue(filter.getKey().remove("*"), filter.getValue().remove("*"));

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

    for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
    {
      const QString tagKey = tagItr.key();
      const QString tagValue = tagItr.value();
      const QStringList tagValues = tagValue.split(";");
      for (int i = 0; i < tagValues.length(); i++)
      {
        const QString tagValue = tagValues.at(i).trimmed().toLower();
        if (!tagValue.isEmpty() && _filterMatchesAnyTag(TagFilter(tagKey, tagValue), e->getTags()))
        {
          return true;
        }
      }
    }
  }

  return false;
}

bool TagCriterion2::_elementPassesTagFilter(const ConstElementPtr& e, const TagFilter& filter) const
{
  LOG_VART(e->getTags());
  LOG_VART(filter);

  LOG_TRACE("Checking for tag match...");
  bool foundFilterMatch = _filterMatchesAnyTag(filter, e->getTags());
  if (foundFilterMatch)
  {
    LOG_TRACE("Found tag match.");
  }

  LOG_VART(filter.getAllowAliases());
  if (!foundFilterMatch && filter.getAllowAliases())
  {
    foundFilterMatch = _hasAuxMatch(e, filter, "alias");
    if (foundFilterMatch)
    {
      LOG_TRACE("Found alias match.");
    }
  }

  LOG_VART(filter.getSimilarityThreshold());
  if (!foundFilterMatch && filter.getSimilarityThreshold() != -1.0)
  {
    foundFilterMatch = _hasAuxMatch(e, filter, "similar");
    if (foundFilterMatch)
    {
      LOG_TRACE("Found similarity match.");
    }
  }

  return foundFilterMatch;
}

bool TagCriterion2::_filterMatchesAnyTag(const TagFilter& filter, const Tags& tags) const
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
    QRegExp keyMatcher(filter.getKey());
    keyMatcher.setPatternSyntax(QRegExp::Wildcard);
    QRegExp valueMatcher(filter.getValue());
    valueMatcher.setPatternSyntax(QRegExp::Wildcard);

    for (Tags::const_iterator tagItr = tags.begin(); tagItr != tags.end(); ++tagItr)
    {
      LOG_VART(tagItr.key());
      if (keyMatched || (!keyMatched && keyMatcher.exactMatch(tagItr.key())))
      {
        LOG_TRACE("Tags match key on wildcard for key: " << filter.getKey());
        keyMatched = true;

        LOG_VART(tagItr.value());
        if (!valueMatched && valueMatcher.exactMatch(tagItr.value()))
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

bool TagCriterion2::_elementPassesMustTagFilters(const ConstElementPtr& e) const
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

bool TagCriterion2::_elementPassesMustNotTagFilters(const ConstElementPtr& e) const
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

bool TagCriterion2::_elementPassesShouldTagFilters(const ConstElementPtr& e) const
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

bool TagCriterion2::isSatisfied(const ConstElementPtr& e) const
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
