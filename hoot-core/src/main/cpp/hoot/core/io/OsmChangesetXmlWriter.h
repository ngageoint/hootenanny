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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMCHANGESETXMLWRITER_H
#define OSMCHANGESETXMLWRITER_H

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/util/Configurable.h>

// Qt
class QXmlStreamWriter;

namespace hoot
{

/**
 * Writes an OSM changeset to an XML file.
 *
 * Before writing this changeset to an OSM API database, the corresponding changeset
 * would have to be created using the API.  Optionally, after writing this the changeset
 * can be closed via the API.
 */
class OsmChangesetXmlWriter : public Configurable
{

public:

  OsmChangesetXmlWriter();

  /**
   * Write the changeset out to the specified file and any changes over changeset.max.size
   * will be written to another file with a path like this:
   *  <filepath>/<filename>.<ext>
   *  <filepath>/<filename>-001.<ext>
   *  <filepath>/<filename>-002.<ext>
   *  ...
   *  <filepath>/<filename>-00n.<ext>
   *
   * @param path Pathname for the output file(s)
   * @param cs Changeset provider to stream the changes from
   */
  void write(QString path, ChangeSetProviderPtr cs);

  /**
   * Set the configuration settings
   *
   * @param conf Settings object containing updated value for changeset.max.size setting
   */
  virtual void setConfiguration(const Settings &conf);

  bool getMultipleChangesetsWritten() const { return _multipleChangesetsWritten; }

private:

  /** Helper functions to write nodes, ways, and relations. */
  void writeNode(QXmlStreamWriter& writer, ConstNodePtr n);
  void writeWay(QXmlStreamWriter& writer, ConstWayPtr w);
  void writeRelation(QXmlStreamWriter& writer, ConstRelationPtr n);

  /** Settings from the config file */
  int _precision;
  long _changesetMaxSize;

  Change _change;

  bool _multipleChangesetsWritten;

};

}

#endif // OSMCHANGESETXMLWRITER_H
