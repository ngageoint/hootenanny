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
 * @copyright Copyright (C) 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MULTIARY_INGEST_CHANGESET_WRITER_H
#define MULTIARY_INGEST_CHANGESET_WRITER_H

// hoot
#include <hoot/core/io/OsmChangeWriter.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/rnd/visitors/AddExportTagsVisitor.h>

// Qt
#include <QFile>

namespace hoot
{

/**
 * This is a writer for the internal temp changeset format used by the multiary ingester.
 */
class MultiaryIngestChangesetWriter : public OsmChangeWriter, public Configurable
{
public:

  static std::string className() { return "hoot::MultiaryIngestChangesetWriter"; }

  MultiaryIngestChangesetWriter();

  virtual ~MultiaryIngestChangesetWriter();

  void close() { if (_fp.get()) { _fp->close(); _fp.reset(); } }

  /**
   * @see OsmChangeWriter
   *
   * "mic" stands for "multiary ingest changeset"
   */
  virtual bool isSupported(QString url) override { return url.endsWith(".mic"); }

  /**
   * Open the specified filename for writing.
   */
  virtual void open(QString fileName) override;

  /**
   * @see OsmChangeWriter
   */
  virtual void writeChange(const Change& change) override;

  virtual void setConfiguration(const Settings& conf);

  QString getElementPayloadFormat() const { return _elementPayloadFormat; }
  virtual void setElementPayloadFormat(const QString format) override;

private:

  boost::shared_ptr<QFile> _fp;
  int _precision;
  OsmJsonWriter _jsonWriter;
  AddExportTagsVisitor _exportTagsVisitor;
  //needed due to #1772 - options are "json" (default) and "xml"
  QString _elementPayloadFormat;

};

}

#endif // MULTIARY_INGEST_CHANGESET_WRITER_H
