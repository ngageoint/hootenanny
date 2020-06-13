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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TAG_ADVANCED_CRITERION_H
#define TAG_ADVANCED_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/schema/TagFilter.h>

// Qt
#include <QString>
#include <QMap>
#include <QRegExp>

namespace hoot
{

/**
 * Criterion allowing for advanced tag filtering concepts, as defined in the Hootenanny schema,
 * including:
 *
 * - tag key/value string wildcard
   - type tag similarity
   - aliases
   - children/ancestors
   - categories
   - associations

   Accepts either a JSON string or file path as input.  See TagAdvancedCriterionTest and the
   Hootenanny User Documentation "Feature Filtering" section for filter examples.
 */
class TagAdvancedCriterion : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::TagAdvancedCriterion"; }

  TagAdvancedCriterion();
  TagAdvancedCriterion(const QString& filterJsonStrOrPath);
  virtual ~TagAdvancedCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TagAdvancedCriterion()); }

  void setConfiguration(const Settings& s);

  virtual QString getDescription() const
  { return "Identifies elements by tag using a set of advanced schema functionality"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

private:

  // one filter for each type: must, must_not, and should
  QMap<QString, QList<TagFilter>> _tagFilters;
  std::shared_ptr<QRegExp> _keyMatcher;
  std::shared_ptr<QRegExp> _valueMatcher;

  void _parseFilterString(const QString& filterJsonStringOrPath);
  void _loadTagFilters(const QString& tagFilterType,
                       const std::shared_ptr<boost::property_tree::ptree>& propTree);

  bool _elementPassesTagFilter(const ConstElementPtr& e, const TagFilter& filter) const;
  bool _elementPassesMustTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesMustNotTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesShouldTagFilters(const ConstElementPtr& e) const;

  /*
   * Checks for a match between the kvp portion of the tag filter and the input tags
   */
  bool _filterMatchesAnyTag(const TagFilter& filter, const Tags& tags) const;
  /*
   * Looks for match other than a tag string match...basically all other type of matches;
   * associations, etc.
   */
  bool _hasAuxMatch(const ConstElementPtr& e, const TagFilter& filter, const QString& type) const;
};

}

#endif // TAG_ADVANCED_CRITERION_H
