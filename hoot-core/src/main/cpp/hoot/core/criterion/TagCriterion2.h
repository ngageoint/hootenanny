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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGCRITERION_H
#define TAGCRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/schema/TagFilter.h>

// Qt
#include <QString>
#include <QMap>

namespace hoot
{

/**
 *
 */
class TagCriterion2 : public ElementCriterion, public Configurable
{
public:

  static std::string className() { return "hoot::TagCriterion2"; }

  TagCriterion2();
  TagCriterion2(const QString filter);

  virtual bool isSatisfied(const ConstElementPtr& e) const;

  void setConfiguration(const Settings& s);

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new TagCriterion2()); }

  virtual QString getDescription() const
  { return "TODO"; }

private:

  //friend class TagCriterion2Test;

  QMap<QString, QList<TagFilter>> _tagFilters;

  void _parseFilterString(const QString filterStr);
  void _loadTagFilters(const QString tagFilterType, boost::shared_ptr<pt::ptree> propTree);

  bool _elementPassesTagFilter(const ConstElementPtr& e, const TagFilter& filter) const;
  bool _elementPassesMustTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesMustNotTagFilters(const ConstElementPtr& e) const;
  bool _elementPassesShouldTagFilters(const ConstElementPtr& e) const;
};

}

#endif // TAGCRITERION_H
