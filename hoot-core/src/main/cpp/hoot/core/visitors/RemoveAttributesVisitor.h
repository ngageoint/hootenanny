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
#ifndef REMOVEATTRIBUTESVISITOR_H
#define REMOVEATTRIBUTESVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

class ElementAttributeType;

/**
 * Sets one or more element properties stored in XML attributes to an empty value, which will cause
 * them to be dropped when written to file output.  Only common OSM attributes may be removed
 * (see ElementAttributeType).  For some attributes to be removed in output (e.g. version,
 * timestamp) an additional setting may have to be set to break OSM compatibility
 * (e.g. see OsmXmlWriter::_includeCompatibilityTags).
 */
class RemoveAttributesVisitor : public ElementVisitor, public Configurable
{

public:

  static QString className() { return "RemoveAttributesVisitor"; }

  RemoveAttributesVisitor();
  explicit RemoveAttributesVisitor(const QStringList types);
  explicit RemoveAttributesVisitor(const QList<ElementAttributeType>& types);
  ~RemoveAttributesVisitor() = default;

  void visit(const std::shared_ptr<Element>& e) override;

  void setConfiguration(const Settings &conf) override;

  QString getDescription() const override
  { return "Removes one or more common OSM attributes from features"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setTypes(const QStringList types);

private:

  QList<ElementAttributeType> _types;
};

}

#endif // REMOVEATTRIBUTESVISITOR_H
