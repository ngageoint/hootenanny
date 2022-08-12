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

#ifndef TAG_FILTER_H
#define TAG_FILTER_H

// Hoot
#include <hoot/core/schema/OsmSchema.h>

// Boost
#include <boost/property_tree/json_parser.hpp>

namespace hoot
{

/**
 * A tag filter linked to the Hootenanny schema that can be applied to feature data with a criterion.
 * Multiple tag filters can be specified in the filter JSON.
 */
class TagFilter
{

public:

  /**
   * @brief Constructor
   * @param key tag key to apply to the filter
   * @param value tag value to apply to the filter
   * @param category schema category to apply to the filter
   */
  TagFilter(const QString& key, const QString& value,
            const OsmSchemaCategory& category = OsmSchemaCategory::Empty);

  /**
   * Creates a tag filter from a JSON node
   *
   * @param tagFilterPart a "must", "must_not", "should" boost prop tree node
   * @return a tag filter
   */
  static TagFilter fromJson(const boost::property_tree::ptree::value_type& tagFilterPart);

  OsmSchemaCategory getCategory() const { return _category; }
  QString getKey() const { return _key; }
  QString getValue() const { return _value; }
  bool getAllowAliases() const { return _allowAliases; }
  double getSimilarityThreshold() const { return _similarityThreshold; }
  bool getAllowChildren() const { return _allowChildren; }
  bool getAllowAncestors() const { return _allowAncestors; }
  bool getAllowAssociations() const { return _allowAssociations; }

  void setAllowAliases(bool allow) { _allowAliases = allow; }
  void setSimilarityThreshold(double threshold);
  void setAllowChildren(bool allow) { _allowChildren = allow; }
  void setAllowAncestors(bool allow) { _allowAncestors = allow; }
  void setAllowAssociations(bool allow) { _allowAssociations = allow; }

  QString toString() const;

private:

  friend class TagFilterTest;

  // allows for retrieving features by tag content (optional if category is used)
  QString _key;
  QString _value;

  // allows retrieving features similar to those brought back by the tag filter, if their
  // similarity score is above the threshold, where similarity is defined in the hoot schema; must
  // be a positive number <= 1.0 for use; -1.0 is the default value and is ignored (optional)
  double _similarityThreshold;

  // allows for retrieving related features with to those brought back by the tag filter;
  // See OsmSchema or the user documentation on tag filtering for more info (optional)
  bool _allowAliases;
  bool _allowChildren;
  bool _allowAncestors;
  bool _allowAssociations;

  // allows for retrieving features by a category, where categories are defined in the hoot schema
  // (optional if a tag filter is used)
  OsmSchemaCategory _category;
};

}

#endif // TAG_FILTER_H
