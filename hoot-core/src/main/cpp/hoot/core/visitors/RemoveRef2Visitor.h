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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVEREF2VISITOR_H
#define REMOVEREF2VISITOR_H

// hoot
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/visitors/ElementVisitor.h>

// Qt
#include <QMap>
#include <QMutex>

namespace hoot
{

/**
 * If the specified criterion meets for both the REF1 and REF2 elements, then the REF2 tag is
 * removed.
 *
 * This class is re-entrant, but not thread safe.
 */
class RemoveRef2Visitor : public ElementVisitor, public ConstOsmMapConsumer,
  public ElementCriterionConsumer
{

public:

  using Ref1ToEid = QMap<QString, ElementId>;

  static QString className() { return "hoot::RemoveRef2Visitor"; }

  static int logWarnCount;

  RemoveRef2Visitor();
  virtual ~RemoveRef2Visitor() = default;

  void addCriterion(const ElementCriterionPtr& e) override;

  void setOsmMap(OsmMap* map) override;
  void setOsmMap(const OsmMap* /*map*/) override { assert(false); }

  void visit(const ElementPtr& e) override;

  virtual bool ref1CriterionSatisfied(const ConstElementPtr& e) const;
  virtual bool ref2CriterionSatisfied(const ConstElementPtr& e) const;

  QString getDescription() const override
  { return "Removes REF2 tags when a criterion is met for both REF1 and REF2 elements"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

protected:

  ElementCriterionPtr _criterion;

private:

  OsmMap* _map;
  Ref1ToEid _ref1ToEid;
  static QStringList _ref2Keys;
  static QMutex _mutex;
  bool _errorOnMissingRef1;

  bool _hasRef2Tag(ElementPtr e) const;
  void _checkAndDeleteRef2(ElementPtr e, QString ref);

};

}

#endif // REMOVEREF2VISITOR_H
