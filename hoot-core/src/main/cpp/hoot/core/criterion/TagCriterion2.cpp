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

// Boost
//#include <boost/property_tree/json_parser.hpp>
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

  /*
   * non-sensical example, but illustrates all the possible filter inputs
   *
   * {
       "must":
       [
         {
           "filter": "tourism=hotel",
           "allowAliases": "true"
         }
       ]
       "should":
       [
         {
           "filter": "amenity=restaurant",
           "similarityThreshold": "0.8"
         },
         {
           "filter": "amenity=place_of_worship"
         },
         {
           "filter": "*address*=*"
         },
         {
           "filter": "poi*=*"
         },
         {
           "filter": "*building=*"
         },
         {
           "filter": "*=*address*"
         },
         {
           "filter": "*=poi*"
         },
         {
           "filter": "*=*building"
         }
       ],
       "must_not":
       [
         {
           "filter": "amenity=chapel"
         }
       ]
     }
   */

  _tagFilters.clear();
  _loadTagFilters("must", propTree);
  _loadTagFilters("should", propTree);
  _loadTagFilters("must_not", propTree);
}

void TagCriterion2::_loadTagFilters(const QString tagFilterType,
                                    boost::shared_ptr<pt::ptree> propTree)
{
  LOG_TRACE("Loading " << tagFilterType << " filters...");
  _tagFilters[tagFilterType].clear();
  for (pt::ptree::value_type& tagFilterPart : propTree->get_child(tagFilterType.toStdString()))
  {
    TagFilter tagFilter = TagFilter::fromJson(tagFilterPart);
    LOG_VART(tagFilter);
    _tagFilters[tagFilterType].append(tagFilter);
  }
  LOG_VART(_tagFilters[tagFilterType].size());
}

bool TagCriterion2::_elementPassesTagFilter(const ConstElementPtr& e, const TagFilter& filter) const
{
  bool keyMatched = false;
  bool valueMatched = false;
  const Tags& tags = e->getTags();
  LOG_VART(tags);

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
      if (!keyMatched && keyMatcher.exactMatch(tagItr.key()))
      {
        LOG_TRACE("Tags match key on wildcard for key: " << filter.getKey());
        keyMatched = true;

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
        break;
      }
    }
  }

  return keyMatched && valueMatched;
}

bool TagCriterion2::isSatisfied(const ConstElementPtr& e) const
{
  //TODO: add similarity and aliases

  for (int i = 0; i < _tagFilters["must"].size(); i++)
  {
    if (!_elementPassesTagFilter(e, _tagFilters["must"].at(i)))
    {
      LOG_TRACE("Tag filtering failed a \"must\" criterion.");
      return false;
    }
  }

  for (int i = 0; i < _tagFilters["must_not"].size(); i++)
  {
    if (_elementPassesTagFilter(e, _tagFilters["must_not"].at(i)))
    {
      LOG_TRACE("Tag filtering failed a \"must not\" criterion.");
      return false;
    }
  }

  for (int i = 0; i < _tagFilters["should"].size(); i++)
  {
    if (_elementPassesTagFilter(e, _tagFilters["should"].at(i)))
    {
      LOG_TRACE("Tag filtering passed on a \"should\" criterion.");
      return true;
    }
  }

  LOG_TRACE("Tag filtering failed to pass any criterion.");
  return false;
}

}
