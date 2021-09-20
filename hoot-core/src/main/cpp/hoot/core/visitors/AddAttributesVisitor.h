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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ADDATTRIBUTESVISITOR_H
#define ADDATTRIBUTESVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/ElementAttributeType.h>
#include <hoot/core/visitors/MultipleCriterionConsumerVisitor.h>

namespace hoot
{

/**
 * Adds one or more attributes to elements. Only common OSM attributes may be added
 * (see ElementAttributeType).
 */
class AddAttributesVisitor : public MultipleCriterionConsumerVisitor, public Configurable
{

public:

  static QString className() { return "AddAttributesVisitor"; }

  AddAttributesVisitor();
  explicit AddAttributesVisitor(const QStringList attributes, const bool negateCriteria = false);
  ~AddAttributesVisitor() override = default;

  void visit(const std::shared_ptr<Element>& e) override;

  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override { return "Adding attributes..."; }
  QString getCompletedStatusMessage() const override
  { return "Added " + QString::number(_numAffected) + " attributes"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Adds one or more common OSM attributes to features"; }

  void setAttributes(const QStringList attributes) { _attributes = attributes; }
  void setAddOnlyIfEmpty(const bool addOnlyIfEmpty) { _addOnlyIfEmpty = addOnlyIfEmpty; }

private:

  //a semicolon delimited list of attributes of the form key=value
  QStringList _attributes;

  //forces the visitor to only update features where the attribute has an empty (default) value
  bool _addOnlyIfEmpty;

  ElementAttributeType::Type _getAttributeType(
    const QString& attribute, QString& attributeValue) const;
};

}

#endif // ADDATTRIBUTESVISITOR_H
