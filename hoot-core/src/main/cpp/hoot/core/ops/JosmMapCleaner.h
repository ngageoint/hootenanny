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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef JOSM_MAP_CLEANER_H
#define JOSM_MAP_CLEANER_H

// hoot
#include <hoot/core/ops/JosmMapValidatorAbstract.h>

namespace hoot
{

/**
 * Calls a hoot-josm Java map cleaner that runs selected JOSM validation routines on a map and
 * cleans any failing elements where possible. Optionally marks elements that fail validation with
 * custom tags, TODO: and if elements were deleted adds their IDs to the map's base tags.
 *
 * See note in JosmMapValidatorAbstract about handling of collection objects via JNI.
 *
 * Not using inheritance with JosmMapValidator since both this and that class need to be factory
 * registered as OsmMapOperations and that can't be done in a class inherited from a class that
 * is already registered.
 */
class JosmMapCleaner : public JosmMapValidatorAbstract
{

public:

  static std::string className() { return "hoot::JosmMapCleaner"; }

  JosmMapCleaner();

  /**
   * @see OsmMapOperation
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const { return "Cleans map data using JOSM"; }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  /**
   * @see OperationStatusInfo
   */
  virtual QString getInitStatusMessage() const
  { return "Cleaning elements..."; }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getCompletedStatusMessage() const
  {
    return
      QString::number(_numValidationErrors) + " / " + QString::number(_numAffected) +
      " elements had validation errors. Fixed " + QString::number(_numGroupsOfElementsCleaned) +
      " groups of elements with errors. Deleted " + QString::number(getNumElementsDeleted()) +
      " elements.";
  }

  int getNumGroupsOfElementsCleaned() const { return _numGroupsOfElementsCleaned; }
  int getNumElementsDeleted() const { return _deletedElementIds.size(); }
  void setAddDetailTags(const bool add) { _addDetailTags = add; }

protected:

  /*
   * @see JosmMapValidatorAbstract
   */
  virtual OsmMapPtr _getUpdatedMap(OsmMapPtr& inputMap);

  /*
   * @see JosmMapValidatorAbstract
   */
  virtual void _getStats();

private:

  // optionally tags elements that were cleaned; if an element was deleted, their ID is added
  // to the maps base tags
  bool _addDetailTags;

  // Each JOSM command capable of cleaning elements after validation breaks the elements up into
  // cleanable groups.
  int _numGroupsOfElementsCleaned;
  // the IDs of the elements deleted as a result of cleaning
  QSet<ElementId> _deletedElementIds;

  /*
   * Converts the delimited element IDs string returned by hoot-josm into a collection
   */
  QSet<ElementId> _elementIdsStrToElementIds(const QString elementIdsStr) const;

  /*
   * Converts the error status info returned by hoot-josm into a readable string
   */
  QString _errorCountsByTypeStrToSummaryStr(
    const QString& errorCountsByTypeStr, const QString& errorFixCountsByTypeStr) const;
};

}

#endif // JOSM_MAP_CLEANER_H
