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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef _ELEMENT_ID_REMAPPER_H_
#define _ELEMENT_ID_REMAPPER_H_

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Allows for assigning a completely new unique set of IDs to a map, which can later be restored
 * back to their original values
 */
class ElementIdRemapper : public OsmMapOperation
{
public:

  static QString className() { return "hoot::ElementIdRemapper"; }

  ElementIdRemapper();
  ElementIdRemapper(const ElementCriterionPtr& remapFilter);
  ElementIdRemapper(
    const ElementCriterionPtr& remapFilter, const ElementCriterionPtr& restoreFilter);
  virtual ~ElementIdRemapper() = default;

  /**
   * @see OsmMapOperation
   */
  virtual void apply(OsmMapPtr& map);

  /**
   * Restores all IDs in the map back to their original values
   *
   * @param map map to modify
   */
  void restore(OsmMapPtr& map);

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual QString getInitStatusMessage() const
  { return "Remapping element IDs..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return "Remapped " + StringUtils::formatLargeNumber(_numAffected) + " element IDs.";
  }

  QString getRestoreCompletedStatusMessage() const
  {
    return "Restored " + StringUtils::formatLargeNumber(_restoredIds) + " element IDs.";
  }

  virtual QString getDescription() const
  { return "Remaps element IDs and is capable of restoring the IDs"; }

  QMap<ElementId, ElementId> getIdMappings() const { return _originalToRemappedElementIds; }
  int getNumRestoredIds() const { return _restoredIds; }

private:

  // the mappings; needed for restoring the original ids
  QMap<ElementId, ElementId> _originalToRemappedElementIds;
  // optional filter that determines which elements have their IDs remapped
  ElementCriterionPtr _remapFilter;
  // optional filter that determines which elements have their IDs restored; if this is null, the
  // _remapFilter is used to determine what is restored
  ElementCriterionPtr _restoreFilter;
  // the restored ID count
  int _restoredIds;
};

}

#endif // _ELEMENT_ID_REMAPPER_H_
