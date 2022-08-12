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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "TagFilter.h"

namespace hoot
{

TagFilter::TagFilter(const QString& key, const QString& value, const OsmSchemaCategory& category)
  : _key(key.trimmed().toLower()),
    _value(value.trimmed().toLower()),
    _similarityThreshold(-1.0),
    _allowAliases(false),
    _allowChildren(false),
    _allowAncestors(false),
    _allowAssociations(false),
    _category(category)
{
  LOG_VART(_key);
  LOG_VART(_value);
  LOG_VART(_category);
  if (_category == OsmSchemaCategory::Empty)
  {
    if (_key.isEmpty())
      throw IllegalArgumentException("Invalid tag filter tag key: " + _key);
    if (_value.isEmpty())
      throw IllegalArgumentException("Invalid tag filter tag value: " + _value);
  }
}

void TagFilter::setSimilarityThreshold(double threshold)
{
  if (threshold != -1.0 && (threshold < 0.001 || threshold > 1.0))
    throw IllegalArgumentException("Invalid tag filter similarity threshold: " + QString::number(threshold));
  _similarityThreshold = threshold;
}

TagFilter TagFilter::fromJson(const boost::property_tree::ptree::value_type& tagFilterPart)
{
/*
A non-sensical and contradictory example, but it illustrates all the possible types of JSON filter inputs
{
 "must":
 [
   {
     "tag": "tourism=hotel",
     "allowAliases": "true"
   },
   {
     "tag": "surface=gravel",
     "allowChildren": "true"
   },
   {
     "category": "transportation"
   },
   {
     "tag": "building:part=yes",
     "allowAssociations": "true"
   }
 ]
 "should":
 [
   {
     "tag": "amenity=restaurant",
     "similarityThreshold": "0.8"
   },
   {
     "tag": "amenity=place_of_worship"
   },
   {
     "tag": "*address*=*"
   },
   {
     "tag": "poi*=*"
   },
   {
     "tag": "*building=*"
   },
   {
     "tag": "*=*address*"
   },
   {
     "tag": "*=poi*"
   },
   {
     "tag": "*=*building"
   }
 ],
 "must_not":
 [
   {
     "tag": "amenity=chapel"
   },
   {
     "tag": "highway=secondary",
     "allowAncestors": "true"
   }
 ]
}
*/

  bool categorySpecified = false;
  QString categoryStr = "invalid";
  boost::optional<std::string> categoryProp = tagFilterPart.second.get_optional<std::string>("category");
  if (categoryProp)
  {
    categoryStr = QString::fromStdString(categoryProp.get()).trimmed().toLower();
    if (categoryStr.isEmpty() || categoryStr == "empty")
      throw IllegalArgumentException("Unknown category.");
  }
  if (categoryStr.isEmpty())
    throw IllegalArgumentException("Empty tag category.");
  else if (categoryStr == "invalid")
    categoryStr = "";
  OsmSchemaCategory category(OsmSchemaCategory::fromString(categoryStr));
  // an empty category is ignored
  if (category != OsmSchemaCategory::Empty)
    categorySpecified = true;
  LOG_VART(category);

  boost::optional<std::string> filterProp = tagFilterPart.second.get_optional<std::string>("tag");
  // must have either a tag key/value or a category filter per input
  if ((filterProp && categorySpecified) || (!filterProp && !categorySpecified))
    throw IllegalArgumentException("Invalid tag filter.");
  QString key = "";
  QString value = "";
  if (filterProp)
  {
    const QString filter = QString::fromStdString(filterProp.get());
    LOG_VART(filter);
    // don't allow full wildcard, like "*", so all inputs should be a kvp with or w/o wildcards
    if (filter.trimmed().isEmpty() || !filter.contains("="))
      throw IllegalArgumentException("Invalid tag filter: " + filter);
    const QStringList filterParts = filter.split("=");
    key = filterParts[0].trimmed().toLower();
    LOG_VART(key);
    value = filterParts[1].trimmed().toLower();
    LOG_VART(value);
  }

  bool allowAliases = false;
  boost::optional<bool> allowAliasesProp = tagFilterPart.second.get_optional<bool>("allowAliases");
  if (allowAliasesProp)
    allowAliases = allowAliasesProp.get();
  LOG_VART(allowAliases);

  // similarity thresholds = -1 are ignored throughout
  double similarityThreshold = -1.0;
  boost::optional<double> similarityThresholdProp = tagFilterPart.second.get_optional<double>("similarityThreshold");
  if (similarityThresholdProp)
    similarityThreshold = similarityThresholdProp.get();
  LOG_VART(similarityThreshold);

  bool allowChildren = false;
  boost::optional<bool> allowChildrenProp = tagFilterPart.second.get_optional<bool>("allowChildren");
  if (allowChildrenProp)
    allowChildren = allowChildrenProp.get();
  LOG_VART(allowChildren);

  bool allowAncestors = false;
  boost::optional<bool> allowAncestorsProp = tagFilterPart.second.get_optional<bool>("allowAncestors");
  if (allowAncestorsProp)
    allowAncestors = allowAncestorsProp.get();
  LOG_VART(allowAncestors);

  bool allowAssociations = false;
  boost::optional<bool> allowAssociationsProp = tagFilterPart.second.get_optional<bool>("allowAssociations");
  if (allowAssociationsProp)
    allowAssociations = allowAssociationsProp.get();
  LOG_VART(allowAssociations);

  TagFilter filter(key, value, category);
  filter.setAllowAliases(allowAliases);
  filter.setSimilarityThreshold(similarityThreshold);
  filter.setAllowChildren(allowChildren);
  filter.setAllowAncestors(allowAncestors);
  filter.setAllowAssociations(allowAssociations);
  return filter;
}

QString TagFilter::toString() const
{
  QString str = "filter: " + _key + "=" + _value + ", allowAliases: ";
  if (_allowAliases)
    str += "true";
  else
    str += "false";
  if (_similarityThreshold != -1.0)
    str += ", similarity threshold: " + QString::number(_similarityThreshold);
  str += ", allowChildren: ";
  if (_allowChildren)
    str += "true";
  else
    str += "false";
  str += ", allowAncestors: ";
  if (_allowAncestors)
    str += "true";
  else
    str += "false";
  str += ", allowAssociations: ";
  if (_allowAssociations)
    str += "true";
  else
    str += "false";
  if (_category != OsmSchemaCategory::Empty)
    str += ", category: " + _category.toString();
  return str;
}

}
