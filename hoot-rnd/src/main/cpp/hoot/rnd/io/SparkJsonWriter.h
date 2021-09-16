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
#ifndef SPARKJSONWRITER_H
#define SPARKJSONWRITER_H

// hoot
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/visitors/AddExportTagsVisitor.h>
#include <hoot/core/conflate/SearchBoundsCalculator.h>

namespace hoot
{

/**
 * Uses the match creator (probably ScriptMatchCreator) to determine the search bounds of each
 * element, then writes a record that is suitable for reading in Spark prototype code.
 *
 * @note Only nodes are supported.
 */
class SparkJsonWriter : public PartialOsmMapWriter
{
public:

  static QString className() { return "SparkJsonWriter"; }

  SparkJsonWriter();
  ~SparkJsonWriter() = default;

  /**
   * @see OsmMapWriter
   */
  void close() override { if (_fp.get()) { _fp->close(); _fp.reset(); } }

  /**
   * @see PartialOsmMapWriter
   */
  void finalizePartial() override { close(); }

  /**
   * @see OsmMapWriter
   */
  bool isSupported(const QString& url) const override { return url.endsWith(".spark"); }

  /**
   * Open the specified filename for writing.
   */
  void open(const QString& fileName) override;

  /**
   * @see PartialOsmMapWriter
   */
  void writePartial(const ConstNodePtr& n) override;

  /**
   * @see PartialOsmMapWriter
   */
  void writePartial(const ConstWayPtr&) override { throw NotImplementedException(); }

  /**
   * @see PartialOsmMapWriter
   */
  void writePartial(const ConstRelationPtr&) override { throw NotImplementedException(); }

  // no point in showing this in the format list at this point, since its not actively maintained
  QString supportedFormats() const override { return ""; }

private:

  std::shared_ptr<QFile> _fp;

  SearchBoundsCalculatorPtr _bounds;
  AddExportTagsVisitor _addExportTagsVisitor;

  int _precision;
};

}

#endif // SPARKJSONWRITER_H
