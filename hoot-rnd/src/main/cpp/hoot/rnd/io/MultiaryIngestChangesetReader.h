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
 * @copyright Copyright (C) 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIARY_INGEST_CHANGESET_READER_H
#define MULTIARY_INGEST_CHANGESET_READER_H

// hoot
#include <hoot/core/io/OsmJsonReader.h>
#include <hoot/core/algorithms/changeset/ChangesetProvider.h>
#include <hoot/core/io/OsmXmlReader.h>

// Qt
#include <QFile>

namespace hoot
{

/**
 * This is a reader for the internal temp changeset format used by the multiary ingester.
 */
class MultiaryIngestChangesetReader : public ChangesetProvider
{

public:

  MultiaryIngestChangesetReader();

  virtual ~MultiaryIngestChangesetReader();

  /**
   * @see ChangesetProvider
   */
  virtual std::shared_ptr<OGRSpatialReference> getProjection() const;

  void open(QString fileName);

  /**
   * @see ChangesetProvider
   */
  virtual void close();

  /**
   * @see ChangesetProvider
   */
  virtual bool hasMoreChanges();

  /**
   * @see ChangesetProvider
   */
  virtual Change readNextChange();

private:

  QFile _file;
  mutable std::shared_ptr<OGRSpatialReference> _wgs84;
  OsmJsonReader _jsonReader;
  OsmXmlReader _xmlReader;

};

}

#endif // MULTIARY_INGEST_CHANGESET_READER_H
