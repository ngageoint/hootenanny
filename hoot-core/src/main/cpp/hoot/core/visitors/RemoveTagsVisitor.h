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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVETAGSVISITOR_H
#define REMOVETAGSVISITOR_H

// hoot
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

// Qt
#include <QRegExp>

namespace hoot
{

/**
 * Removes any tags with keys matching those passed to this visitor
 *
 * This really should be an ElementVisitor. See #2831.
 */
class RemoveTagsVisitor : public ElementOsmMapVisitor, public Configurable,
  public ElementCriterionConsumer
{
public:

  static QString className() { return "RemoveTagsVisitor"; }

  RemoveTagsVisitor();
  explicit RemoveTagsVisitor(const QStringList& keys);
  ~RemoveTagsVisitor() override = default;

  void addCriterion(const ElementCriterionPtr& e) override;

  void setConfiguration(const Settings& conf) override;

  /**
   * @see ElementVisitor
   */
  void visit(const std::shared_ptr<Element>& e) override;

  QString getInitStatusMessage() const override
  { return "Removing tags..."; }
  QString getCompletedStatusMessage() const override
  {
    return QString("Removed %1 tags from %2 different elements")
            .arg(StringUtils::formatLargeNumber(_numTagsRemoved), StringUtils::formatLargeNumber(_numAffected));
  }

  QString getDescription() const override { return "Removes tags by key"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setNegateCriterion(bool negate) { _negateCriterion = negate; }

protected:

  // Tag keys that match these get removed.
  QList<QRegExp> _keyRegexs;

  // Criterion the element whose tags are to be remove must match
  std::shared_ptr<ElementCriterion> _criterion;

  long _numTagsRemoved;

private:

  // This allows for negating the criterion as an option sent in from the command line.
  bool _negateCriterion;

  void _setCriterion(const QString& criterionName);
  void _setKeys(const QStringList& keys);
};

}

#endif // REMOVETAGSVISITOR_H
