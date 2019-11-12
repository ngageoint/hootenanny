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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMMAPREADER_H
#define OSMMAPREADER_H

// hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class OsmMapReader
{

public:

  static std::string className() { return "hoot::OsmMapReader"; }

  OsmMapReader() : _ignoreDuplicates(false), _warnOnVersionZeroElement(false) {}

  virtual ~OsmMapReader() {}

  /**
   * Returns true if the output URL is likely supported. This won't necessarily do an exhaustive
   * check. It will look to see if the URL is properly formatted (e.g. ends in .osm, or starts
   * with postgresql:, etc.)
   */
  virtual bool isSupported(const QString& url) = 0;

  /**
   * Opens the specified URL for reading.
   */
  virtual void open(const QString& url) { _url = url; }

  /**
   * Reads the specified map. When this method is complete the input will likely be closed.
   */
  virtual void read(const OsmMapPtr& map) = 0;

  /**
   * Determines the reader's default element status
   */
  virtual void setDefaultStatus(Status status) = 0;

  /**
   * Determines whether the reader should use the element id's from the data being read
   */
  virtual void setUseDataSourceIds(bool useDataSourceIds) = 0;

  /**
   * Uses the element status from the file
   */
  virtual void setUseFileStatus(bool /*useFileStatus*/) { }

  /**
   * Lists supported data format extensions
   *
   * If multiple are supported, delimit them with a semicolon.
   *
   * @return a formats string
   */
  virtual QString supportedFormats() = 0;

  /**
   * Gets the ignore duplicates flag
   */
  bool getIgnoreDuplicates() { return _ignoreDuplicates; }

  /**
   * Set the ignore duplicates flag, when set to true, derived classes will ignore any element
   * who's ID is already being used.  This is useful when elements cross bounding box boundaries
   * and multiple bounding boxes are being merged together.
   */
  void setIgnoreDuplicates(bool ignore) { _ignoreDuplicates = ignore; }

  void setWarnOnVersionZeroElement(bool warn) { _warnOnVersionZeroElement = warn; }

protected:

  /**
   * Ignore the duplicate IDs or throw an error
   */
  bool _ignoreDuplicates;
  /** Url of the map to open and read */
  QString _url;
  // Its useful to be able to track zero version elements in ref datasets during changeset
  // derivation debugging. So far have only implemented this on some of the file format readers,
  // as data read out of an API DB will always have a positive version due to imposed constraints.
  bool _warnOnVersionZeroElement;
};

}

#endif // OSMMAPREADER_H
