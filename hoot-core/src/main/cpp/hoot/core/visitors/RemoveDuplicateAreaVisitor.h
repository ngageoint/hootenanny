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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVEDUPLICATEAREAVISITOR_H
#define REMOVEDUPLICATEAREAVISITOR_H

// Hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

// geos
#include <geos/geom/Geometry.h>

namespace hoot
{

class TagDifferencer;

/**
 * Locates area elements that are duplicates and removes one of the duplicates. An area is
 * considered a duplicate if:
 *  - The status is the same (e.g. Unknown1 == Unknown1)
 *  - One element is not an ancestor of the other
 *  - The overlap of the two geometries is equal (with reasonable precision)
 *  - Neither element has an area of zero.
 *  - TagComparator::compareTags() returns 1.0.
 *
 * The element with the smaller number of tags will be deleted. If they have the same number of tags
 * then the element with the smaller id is deleted.
 *
 * RecursiveElementRemover is used to remove the element.
 */
class RemoveDuplicateAreaVisitor : public ElementOsmMapVisitor
{
public:

  static std::string className() { return "hoot::RemoveDuplicateAreaVisitor"; }

  RemoveDuplicateAreaVisitor();

  virtual void visit(const std::shared_ptr<Element>& e) override;

  virtual QString getInitStatusMessage() const { return "Removing duplicate areas..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " duplicate areas"; }

  virtual QString getDescription() const { return "Removes duplicate areas"; }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const;

  virtual std::string getClassName() const { return className(); }

private:

  std::shared_ptr<TagDifferencer> _diff;
  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>> _geoms;

  std::shared_ptr<geos::geom::Geometry> _convertToGeometry(const std::shared_ptr<Element>& e1);
  bool _equals(const std::shared_ptr<Element>& e1, const std::shared_ptr<Element>& e2);
  void _removeOne(const std::shared_ptr<Element>& e1, const std::shared_ptr<Element>& e2);
};

}

#endif // REMOVEDUPLICATEAREAVISITOR_H
