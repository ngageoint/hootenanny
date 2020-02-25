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

#ifndef DUPLICATENAMEREMOVER_H
#define DUPLICATENAMEREMOVER_H

// Qt
#include <QString>

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{
class OsmMap;

/**
 * Searches for ways that contain the same name multiple times in the name and/or alt_name fields.
 * Any duplicates in the alt_name field will be removed.
 */
class DuplicateNameRemover : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::DuplicateNameRemover"; }

  DuplicateNameRemover();

  void apply(std::shared_ptr<OsmMap>& map);

  /**
   * Remove alternate names that are duplicates.
   */
  static void removeDuplicates(std::shared_ptr<OsmMap> map);

  virtual void setConfiguration(const Settings& conf);

  void setCaseSensitive(bool caseSensitive) { _caseSensitive = caseSensitive; }

  virtual QString getInitStatusMessage() const { return "Removing duplicate name tags..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " duplicate name tags"; }

  virtual QString getDescription() const { return "Removes duplicate name tags from a feature"; }

  virtual std::string getClassName() const { return className(); }

private:

  bool _caseSensitive;
  // If the feature has a name tag, enabling this will preserve the name tag value in the "name"
  // tag.  Otherwise, names and alternate names are treated equally.  Currently, this setting is
  // used by Attribute Conflation only, but possibly could be made the default behavior at some
  // point.
  bool _preserveOriginalName;

  std::shared_ptr<OsmMap> _map;

  QString _getBestName(const QString& n1, const QString& n2);
};

}

#endif // DUPLICATENAMEREMOVER_H
