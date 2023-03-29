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
 * @copyright Copyright (C) 2022-2023 Maxar (http://www.maxar.com/)
 */

#ifndef OVERPASS_READER_INTERFACE_H
#define OVERPASS_READER_INTERFACE_H

//  Qt
#include <QString>

namespace hoot
{

/**
 * @brief The OverpassReaderInterface contains all Overpass related functions for readers.
 */
class OverpassReaderInterface
{
public:

  OverpassReaderInterface();
  virtual ~OverpassReaderInterface() = default;

  bool isOverpassUrl(const QString& url) const;

  QString parseOverpassError(const QString& result) const;

  bool isOverpassXml(const QString& url) const;
  bool isOverpassJson(const QString& url) const;

  void setOverpassUrl(const QString& url);

  void setQueryFilePath(const QString& path) { _queryFilepath = path; }

protected:

  /** Flag indicating this query is an Overpass API query */
  bool _isOverpass;

  /** Overpass */
  QString _queryFilepath;

  QString _readOverpassQueryFile(const QString& path, bool stripNewLines = true) const;

  /**  Allow test class to access protected members for white box testing */
  friend class OverpassReaderInterfaceTest;
};

}

#endif  //  OVERPASS_READER_INTERFACE_H
