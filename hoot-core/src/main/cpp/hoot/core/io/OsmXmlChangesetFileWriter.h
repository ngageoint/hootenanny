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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMXMLCHANGESETFILEWRITER_H
#define OSMXMLCHANGESETFILEWRITER_H

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmChangesetFileWriter.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/ScoreMatrix.h>

// Qt
#include <QMap>
class QXmlStreamWriter;

namespace hoot
{

/**
 * Writes an OSM changeset to an XML file.
 *
 * Before writing this changeset to an OSM API database, the corresponding changeset would have to
 * be created using the API.  Optionally, after writing this the changeset can be closed via the
 * API.
 */
class OsmXmlChangesetFileWriter : public OsmChangesetFileWriter
{

public:

  static std::string className() { return "hoot::OsmXmlChangesetFileWriter"; }

  OsmXmlChangesetFileWriter();
  virtual ~OsmXmlChangesetFileWriter() = default;

  /**
   * @see ChangesetFileWriter
   */
  virtual void write(const QString& path, const ChangesetProviderPtr& changesetProvider);

  /**
   * @see ChangesetFileWriter
   */
  virtual void write(const QString& path, const QList<ChangesetProviderPtr>& changesetProviders);

  /**
   * @see ChangesetFileWriter
   */
  virtual QString getStatsTable(
    const ChangesetStatsFormat& format = ChangesetStatsFormat::Text) const;

  /**
   * @see ChangesetFileWriter
   */
  virtual bool isSupported(const QString& output) const { return output.endsWith(".osc"); }

  virtual void setConfiguration(const Settings &conf);

private:

  /** Settings from the config file */
  int _precision;

  Change _change;
  // element IDs associated with a changes encountered
  QList<ElementId> _parsedChangeIds;

  bool _addTimestamp;
  bool _includeDebugTags;
  bool _includeCircularErrorTags;

  OsmXmlWriter _invalidCharacterHandler;

  QMap<ElementType::Type, long> _newElementIdCtrs;
  //keeping track of these mappings unfortunately makes this writer memory bound
  QMap<ElementType::Type, QMap<long, long>> _newElementIdMappings;

  //  Keep track of the matrix of add, modify, delete for nodes, ways, relations
  ScoreMatrix<long> _stats;

  // list of metadata tag keys allowed to be written to the changeset
  QStringList _metadataAllowKeys;

  /** Helper functions to write nodes, ways, and relations. */
  void _writeNode(QXmlStreamWriter& writer, ConstElementPtr node, ConstElementPtr previous);
  void _writeWay(QXmlStreamWriter& writer, ConstElementPtr way, ConstElementPtr previous);
  void _writeRelation(QXmlStreamWriter& writer, ConstElementPtr relation, ConstElementPtr previous);
  void _writeTags(QXmlStreamWriter& writer, Tags& tags, const Element* element);

  void _initIdCounters();
  void _initStats();
};

}

#endif // OSMXMLCHANGESETFILEWRITER_H
