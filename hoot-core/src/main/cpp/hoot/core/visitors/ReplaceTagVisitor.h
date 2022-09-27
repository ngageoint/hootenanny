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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REPLACETAGVISITOR_H
#define REPLACETAGVISITOR_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * The ReplaceTagVisitor class replaces matching tags with the specified replacement tag.
 *
 * @todo Bring this class more in line with the options available in SetTagValueVisitor.
 */
class ReplaceTagVisitor : public ElementVisitor, public Configurable
{
public:

  /**
   * @brief className - get class name string
   * @return The namespace qualified class name
   */
  static QString className() { return "ReplaceTagVisitor"; }

  /**
   * @brief ReplaceTagVisitor - default constructor
   */
  ReplaceTagVisitor();
  /**
   * @brief ReplaceTagVisitor - This constructor lets you specify the tag
   * (k=v) to match, and the tag (k=v) to replace it with.
   * @param matchKey - key to match
   * @param matchValue - value to match
   * @param replaceKey - key to use as a replacement
   * @param replaceValue - value to use as a replacement
   */
  ReplaceTagVisitor(const QString& matchKey, const QString& matchValue, const QString& replaceKey,
                    const QString& replaceValue);
  ~ReplaceTagVisitor() override = default;

  /**
   * @brief visit - visit an element & perform check and replace
   * @param e - element to visit
   */
  void visit(const std::shared_ptr<Element>& e) override;

  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override
  { return "Replacing tags..."; }
  QString getCompletedStatusMessage() const override
  { return QString("Replaced %1 element tags").arg(QString::number(_numAffected)); }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Replaces matching tags with the specified replacement tag"; }

private:

  QString _matchKey;
  QString _matchValue;
  QString _replaceKey;
  QString _replaceValue;
};

}

#endif // REPLACETAGVISITOR_H
