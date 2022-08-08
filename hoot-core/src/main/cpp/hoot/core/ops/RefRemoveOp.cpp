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
#include "RefRemoveOp.h"

// hoot
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RefRemoveOp)

class GetRefVisitor : public ElementConstOsmMapVisitor
{
public:

  GetRefVisitor(ElementCriterionPtr c) : _criterion(c) { }
  ~GetRefVisitor() override = default;

  const set<QString>& getRefs() const { return _refs; }

  void visit(const std::shared_ptr<const Element>& e) override
  {
    if (e->getTags().contains(MetadataTags::Ref1()) && _criterion->isSatisfied(e))
    {
      _refs.insert(e->getTags().get(MetadataTags::Ref1()));
    }
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

private:

  ElementCriterionPtr _criterion;
  set<QString> _refs;
};

class RefRemoveVisitor : public ElementOsmMapVisitor
{
public:

  RefRemoveVisitor(ElementCriterionPtr c) : _criterion(c) { }
  ~RefRemoveVisitor() override = default;

  void visit(const std::shared_ptr<Element>& e) override
  {
    if (_criterion->isSatisfied(e))
    {
      NotCriterionPtr nc = std::make_shared<NotCriterion>(_criterion);
      RecursiveElementRemover(e->getElementId(), false, nc).apply(_map->shared_from_this());
    }
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

private:

  ElementCriterionPtr _criterion;
};

class UpdateRefVisitor : public ElementOsmMapVisitor
{
public:

  UpdateRefVisitor(const set<QString>& refs) : _refs(refs) { }
  ~UpdateRefVisitor() override = default;

  QStringList _removeRef1(QStringList refs) const
  {
    QStringList result;
    for (int i = 0; i < refs.size(); i++)
    {
      if (_refs.count(refs[i]) == 0)
      {
        result.append(refs[i]);
      }
    }

    return result;
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

  void visit(const std::shared_ptr<Element>& e) override
  {
    if (e->getTags().contains(MetadataTags::Ref2()))
    {
      QStringList ref2 = _removeRef1(e->getTags().getList(MetadataTags::Ref2()));
      if (ref2.empty())
      {
        e->getTags().set(MetadataTags::Ref2(), "none");
      }
      else
      {
        e->getTags().setList(MetadataTags::Ref2(), ref2);
      }
    }
    e->getTags().setList("REVIEW", _removeRef1(e->getTags().getList("REVIEW")));
    e->getTags().setList("CONFLICT", _removeRef1(e->getTags().getList("CONFLICT")));
    e->getTags().setList("DIVIDED1", _removeRef1(e->getTags().getList("DIVIDED1")));
    e->getTags().setList("DIVIDED2", _removeRef1(e->getTags().getList("DIVIDED2")));
  }

private:

  const set<QString>& _refs;
};

void RefRemoveOp::apply(std::shared_ptr<OsmMap>& map)
{
  // get all the REF1 IDs that will be removed
  GetRefVisitor grv(_criterion);
  map->visitRo(grv);

  // remove all the elements that meet the criteria
  RefRemoveVisitor rrv(_criterion);
  map->visitRw(rrv);

  // remove all the REF1 references that were removed from REF2 entries. This might happen if
  // you remove all buildings, but there are some POIs remaining that were matched to the buildings.
  UpdateRefVisitor urv(grv.getRefs());
  map->visitRw(urv);
}

}
