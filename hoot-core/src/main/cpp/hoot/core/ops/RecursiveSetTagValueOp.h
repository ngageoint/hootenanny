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

#ifndef RECURSIVE_SET_TAG_VALUE_OP_H
#define RECURSIVE_SET_TAG_VALUE_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/visitors/SetTagValueVisitor.h>

namespace hoot
{

/**
 * Allows for setting tags on elements and their children (way nodes, relation members)
 */
class RecursiveSetTagValueOp : public OsmMapOperation, public ElementCriterionConsumer
{
public:

  static QString className() { return "hoot::RecursiveSetTagValueOp"; }

  RecursiveSetTagValueOp() = default;
  // We have some of the constructor signatures from SetTagValueVisitor here, as well as a signature
  // that allows passing in an already configured, possibly complex, criterion. We may want to
  // extend that capability to SetTagValueVisitor at some point.
  RecursiveSetTagValueOp(
    const QStringList& keys, const QStringList& values, ElementCriterionPtr elementCriterion,
    bool appendToExistingValue = false, const bool overwriteExistingTag = true);
  RecursiveSetTagValueOp(
    const QString& key, const QString& value, ElementCriterionPtr elementCriterion,
    bool appendToExistingValue = false, const bool overwriteExistingTag = true);
    ~RecursiveSetTagValueOp() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @see ElementCriterionConsumer
   */
  void addCriterion(const ElementCriterionPtr& e) override;

  void setNegateCriterion(bool negate) { _negateCriterion = negate; }

  /**
   * @see ApiEntityInfo
   */
  QString getDescription() const override
  { return "Adds or updates specific tags on elements and their children"; }

  /**
   * @see OperationStatus
   */
  QString getInitStatusMessage() const override { return _tagger->getInitStatusMessage(); }

  /**
   * @see OperationStatus
   */
  QString getCompletedStatusMessage() const override { return _tagger->getCompletedStatusMessage(); }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  ElementCriterionPtr _crit;
  bool _negateCriterion;
  // Wrap SetTagValueVisitor for tagging purposes only and handle the criterion filter separately
  // within this class.
  std::shared_ptr<SetTagValueVisitor> _tagger;

  void _setCriterion(const QString& criterionName);
};

}

#endif // RECURSIVE_SET_TAG_VALUE_OP_H
