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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSMMAPWRITER_H
#define OSMMAPWRITER_H

// hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * @brief The OsmMapWriter class writes an OsmMap to a specified format.
 *
 * @todo The debug map methods should be implemented across writers other than OsmXmlWriter.
 */
class OsmMapWriter
{
public:

  static QString className() { return "OsmMapWriter"; }

  OsmMapWriter() : _debug(false) { }
  virtual ~OsmMapWriter() = default;

  /**
   * Returns true if the output URL is likely supported. This won't necessarily do an exhaustive
   * check. It will look to see if the URL is properly formatted (e.g. ends in .osm, or starts
   * with postgresql:, etc.)
   */
  virtual bool isSupported(const QString& url) const = 0;

  /**
   * Opens the specified URL for writing.
   */
  virtual void open(const QString& url) = 0;

  /**
   * Writes the specified map out. When this method is complete the output will likely be closed
   * and all data is guaranteed to be flushed.
   */
  virtual void write(const ConstOsmMapPtr& map) = 0;

  /**
   * Lists supported data format extensions
   *
   * If multiple are supported, delimit them with a semicolon.
   *
   * @return a formats string
   */
  virtual QString supportedFormats() const = 0;

  /**
   * Gets flag indicating the writer is writing a debug map so that extra debugging metadata can
   * be included in each implementation of the output formats
   */
  bool getIsDebugMap() const { return _debug; }

  /**
   * Sets flag indicating the writer is writing a debug map so that extra debugging metadata is
   * included in the output
   */
  void setIsDebugMap(const bool is_debug = false) { _debug = is_debug; }

private:

  bool _debug;
};

}

#endif // OSMMAPWRITER_H
