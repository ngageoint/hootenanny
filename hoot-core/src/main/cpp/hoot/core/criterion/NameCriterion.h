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
#ifndef NAME_CRITERION_H
#define NAME_CRITERION_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

// Qt


namespace hoot
{

/**
 * Identifies elements that contain a specified name
 *
 * This is currently using very simple partial matching enabled by string containment. It could be
 * upgraded to use wildcard matching, if needed (see TagAdvancedCriterion).
 */
class NameCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "hoot::NameCriterion"; }

  NameCriterion();
  NameCriterion(const QStringList& names, const bool caseSensitive = false,
                const bool partialMatch = false);
  ~NameCriterion() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override
  { return std::make_shared<NameCriterion>(_names, _caseSensitive); }

  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override
  { return "Identifies elements that contain a specified name"; }

  QStringList getNames() const { return _names; }
  bool getCaseSensitive() const { return _caseSensitive; }

  void setNames(const QStringList& names) { _names = names; }
  void setCaseSensitive(bool caseSens) { _caseSensitive = caseSens; }
  void setPartialMatch(bool partialMatch) { _partialMatch = partialMatch; }

private:

  QStringList _names;
  // if enabled, names must match case
  bool _caseSensitive;
  // if enabled, the input string can match if it is contained within the feature name
  bool _partialMatch;
};

}

#endif // NAME_CRITERION_H
