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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef TAGRENAMEKEYVISITOR_H
#define TAGRENAMEKEYVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Renames all keys for all elements with the specified key to a new key
 */
class TagRenameKeyVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "TagRenameKeyVisitor"; }

  /**
   * @brief TagRenameKeyVisitor - Constructor
   */
  TagRenameKeyVisitor() = default;
  /**
   * @brief TagRenameKeyVisitor - Constructor
   * @param oldKey tag key to rename
   * @param newKey tag key to rename with
   */
  TagRenameKeyVisitor(const QString& oldKey, const QString& newKey);
  ~TagRenameKeyVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& e) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override { return "Renaming tag keys..."; }
  QString getCompletedStatusMessage() const override
  { return QString("Renamed %1 tag keys").arg(QString::number(_numAffected)); }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Renames tag keys"; }

  void setOldKey(const QString& key) { _oldKey = key; }
  void setNewKey(const QString& key) { _newKey = key; }

private:

  QString _oldKey;
  QString _newKey;
};

}

#endif // TAGRENAMEKEYVISITOR_H
