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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SPARKCHANGESETWRITER_H
#define SPARKCHANGESETWRITER_H

// hoot
#include <hoot/core/io/OsmChangeWriter.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/visitors/AddExportTagsVisitor.h>
#include <hoot/core/conflate/SearchBoundsCalculator.h>

namespace hoot
{

/**
 * Outputs a changeset usable by Spark
 *
 * @note Only nodes are supported.
 */
class SparkChangesetWriter : public OsmChangeWriter, public Configurable
{
public:

  static QString className() { return "SparkChangesetWriter"; }

  SparkChangesetWriter();
  ~SparkChangesetWriter();

  /**
   * @see OsmChangeWriter
   */
  bool isSupported(const QString& url) const override { return url.endsWith(".spark.1"); }

  /**
   * Open the specified filename for writing.
   */
  void open(const QString& fileName) override;

  void close() override;

  /**
   * @see OsmChangeWriter
   */
  void writeChange(const Change& change) override;

  void setConfiguration(const Settings& conf) override;

  void setElementPayloadFormat(const QString& format) override;

private:

  std::shared_ptr<QFile> _addFile;
  std::shared_ptr<QFile> _deleteFile;
  SearchBoundsCalculatorPtr _boundsCalculator;
  int _precision;
  OsmJsonWriter _jsonWriter;
  AddExportTagsVisitor _exportTagsVisitor;

};

}

#endif // SPARKCHANGESETWRITER_H
