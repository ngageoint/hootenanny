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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DANGLERREMOVERMANIPULATION_H
#define DANGLERREMOVERMANIPULATION_H

// Hoot
#include <hoot/core/manipulators/Manipulation.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

/**
 * Removes very small lines that are unlikely to be correct.
 */
class DanglerRemoverManipulation : public Manipulation
{
public:
  DanglerRemoverManipulation(long wayId, ConstOsmMapPtr map, Meters errorPlus);

  virtual ~DanglerRemoverManipulation() {}

  virtual void applyManipulation(OsmMapPtr wm, std::set<ElementId>& impactedElements,
    std::set<ElementId>& newElements) const;

  virtual double calculateProbability(ConstOsmMapPtr map) const;

  virtual double calculateScore(ConstOsmMapPtr map) const;

  virtual const std::set<ElementId>& getImpactedElementIds(const ConstOsmMapPtr& map) const;

  virtual double getProbabilityEstimate() const { return _p; }

  virtual double getScoreEstimate() const { return _estimate; }

  virtual bool isValid(ConstOsmMapPtr map) const;

  virtual QString toString() const;

private:

  mutable std::set<ElementId> _impactedElements;
  mutable double _p;

  double _errorPlus;
  long _wayId;
};

}

#endif // DANGLERREMOVERMANIPULATION_H
