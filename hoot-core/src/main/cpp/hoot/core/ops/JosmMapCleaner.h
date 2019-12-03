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
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/info/OperationStatusInfo.h>

// JNI
#include <jni.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * TODO
 *
 * passing strings for all collection types in order to cut down on JNI calls for performance
 * reasons; also keeps the client code less complex
 */
class JosmMapCleaner : public OsmMapOperation, public Configurable, public OperationStatusInfo
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
   * TODO
   *
   * @return
   */
  QMap<QString, QString> getAvailableCleaners();

  /**
   * @see ApiEntityInfo
   */
  virtual QString getDescription() const { return "TODO"; }

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

  int getNumElementsProcessed() const { return _numAffected; }
  int getNumValidationErrors() const { return _numValidationErrors; }
  int getNumGroupsOfElementsCleaned() const { return _numGroupsOfElementsCleaned; }
  int getNumElementsDeleted() const { return _deletedElementIds.size(); }

  QStringList getJosmCleanersUsed() const { return _josmCleanersToUse; }
  void setCleanersJosmNamespace(const QString& cleanersNamespace)
  { _cleanersJosmNamespace = cleanersNamespace; }
  void setCleanerInterfaceName(const QString& name) { _cleanerInterfaceName = name; }
  void setJosmCleanersExclude(const QStringList& cleaners) { _josmCleanersExclude = cleaners; }
  void setJosmCleanersInclude(const QStringList& cleaners) { _josmCleanersInclude = cleaners; }
  void setAddValidationTags(const bool add) { _addValidationTags = add; }
  void setAddDebugTags(const bool add) { _addDebugTags = add; }

private:

  // TODO - assuming a single one for now
  QString _cleanersJosmNamespace;
  // TODO
  QString _cleanerInterfaceName;
  // TODO
  QStringList _josmCleanersToUse;
  // TODO
  QStringList _josmCleanersInclude;
  // TODO
  QStringList _josmCleanersExclude;
  // TODO
  bool _addValidationTags;
  // TODO
  bool _addDebugTags;

  // TODO
  JNIEnv* _javaEnv;
  // TODO
  jclass _cleanerInterfaceClass;
  // TODO
  jobject _cleanerInterface;
  // TODO
  bool _cleanerInterfaceInitialized;

  // TODO
  int _numValidationErrors;
  // TODO
  int _numGroupsOfElementsCleaned;
  // TODO
  QSet<ElementId> _deletedElementIds;

  void _initCleanerImplementation();
  void _initJosmCleanersList();
  void _updateJosmCleanersWithNamepace(QStringList& cleaners);

  void _getCleanStats();

  /*
   * TODO
   */
  OsmMapPtr _getCleanedMap(OsmMapPtr& inputMap);

  QSet<ElementId> _elementIdsStrToElementIds(const QString elementIdsStr) const;
};

}

#endif // JOSM_MAP_CLEANER_H
