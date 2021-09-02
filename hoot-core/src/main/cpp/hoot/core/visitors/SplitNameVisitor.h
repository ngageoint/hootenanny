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
#ifndef SPLITNAMEVISITOR_H
#define SPLITNAMEVISITOR_H

// Hoot
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Sometimes name fields have more characters than we'd like. This method takes all names beyond
 * the a specified character limit and puts them into fields named 'name:0', 'name:1', etc.
 */
class SplitNameVisitor : public ElementVisitor, public Configurable
{
public:

  static QString className() { return "SplitNameVisitor"; }

  SplitNameVisitor();
  ~SplitNameVisitor() = default;

  void visit(const std::shared_ptr<Element>& e) override;

  QString getInitStatusMessage() const override
  { return "..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " building outlines"; }

  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return QString("Splits name tags over %1 characters").arg(_maxSize); }

  void setMaxSize(int s) { _maxSize = s; }

private:

  int _maxSize;

  void _addExtraNames(Tags& t, const QStringList& extraNames) const;

  int _getNextNameId(const Tags& t, int lastId) const;

  /**
   * Given a name list greater than a certain character size, split the names up so that the result
   * is a string <= the max allowed character size, and put all the extra names into extras.
   *
   * If a name is greater than the character max size, it is cropped down to the max size minus
   * three plus "..." at the end.
   */
  QStringList _splitNames(const QString& v, QStringList& extras) const;
};

}

#endif // SPLITNAMEVISITOR_H
