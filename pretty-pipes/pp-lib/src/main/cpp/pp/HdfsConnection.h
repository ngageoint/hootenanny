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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#ifndef HDFSCONNECTION_H
#define HDFSCONNECTION_H

// Hadoop
#include <hdfs.h>

// Qt
#include <QString>
#include <QHash>

namespace pp
{

class HdfsConnection
{
public:

  HdfsConnection();

  ~HdfsConnection();

  hdfsFS getConnection(std::string host = "default", int port = 0);

  static pp::HdfsConnection& getInstance() { return _instance; }

private:
  static pp::HdfsConnection _instance;

  QHash<QString, hdfsFS> _connections;

  QString _makeId(const std::string& host, int port);
};

}

#endif // HDFSCONNECTION_H
