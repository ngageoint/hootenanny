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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "HdfsConnection.h"

// Standard
#include <iostream>

namespace pp
{

HdfsConnection HdfsConnection::_instance;

HdfsConnection::HdfsConnection()
{

}

HdfsConnection::~HdfsConnection()
{
  // Disconnecting seems to cause seg faults. Ah, we tried.
//  QHashIterator<QString, hdfsFS> i(_connections);
//  while (i.hasNext())
//  {
//    i.next();
//    if (hdfsDisconnect(i.value()) == -1)
//    {
//      std::cerr << "Error disconnecting HDFS " << i.key().toStdString() << std::endl;
//    }
//  }
}

hdfsFS HdfsConnection::getConnection(std::string host, int port)
{
  QString id = _makeId(host, port);
  if (_connections.contains(id))
  {
    return _connections[id];
  }
  else
  {
    hdfsFS fs = hdfsConnect(host.c_str(), port);
    if (fs == NULL)
    {
      throw std::ios_base::failure("Error connecting to HDFS.");
    }
    _connections[id] = fs;
    return fs;
  }
}

QString HdfsConnection::_makeId(const std::string& host, int port)
{
  return QString("%1:%2").arg(QString::fromStdString(host)).arg(port);
}

}
