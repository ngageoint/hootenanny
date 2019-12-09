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
 * TODO
 *
 * passing strings for all collection types in order to cut down on JNI calls for performance
 * reasons; also keeps the client code less complex
 *
 * Not using inheritance with JosmMapValidator since both this and that class need to be factory
 * registered as OsmMapOperations and that can't be done in a class inherited from a class that
 * is already registered.
 */
class JosmMapCleaner : public JosmMapValidatorAbstract
{

public:

  static std::string className() { return "hoot::JosmMapCleaner"; }

  /**
   * TODO
   */
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

  virtual QString getInitStatusMessage() const
  { return "Cleaning elements..."; }

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

  void setAddDebugTags(const bool add) { _addDebugTags = add; }

protected:

  virtual OsmMapPtr _getUpdatedMap(OsmMapPtr& inputMap);

  virtual void _getStats();

private:

  // TODO
  bool _addDebugTags;

  // TODO
  int _numGroupsOfElementsCleaned;
  // TODO
  QSet<ElementId> _deletedElementIds;

  QSet<ElementId> _elementIdsStrToElementIds(const QString elementIdsStr) const;

  QString _errorCountsByTypeStrToSummaryStr(
    const QString& errorCountsByTypeStr, const QString& errorFixCountsByTypeStr) const;
};

}

#endif // JOSM_MAP_CLEANER_H
