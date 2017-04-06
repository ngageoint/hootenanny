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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef BASECOMMAND_H
#define BASECOMMAND_H

#include "Command.h"

// geos
#include <geos/geom/Envelope.h>

// Hoot
#include <hoot/core/elements/Status.h>

namespace hoot
{

class OsmMap;

using namespace geos::geom;

class BaseCommand : public Command
{
public:
  BaseCommand();

  virtual ~BaseCommand() {}

  virtual QString getHelp() const;

  void loadMap(boost::shared_ptr<OsmMap> map, QString path, bool useFileId,
    Status defaultStatus = Status::Invalid);

  /**
   * Parses a comma delimited envelope in the form minx,miny,maxx,maxy.
   */
  Envelope parseEnvelope(QString envStr) const;

  /**
   * This method will pull out common arguments (e.g. --conf), convert the args to a QStringList
   * and pass it to runSimple.
   */
  virtual int run(char* argv[], int argc);

  /**
   * This is the preferred method to override.
   */
  virtual int runSimple(QStringList args) = 0;

  void saveMap(boost::shared_ptr<const OsmMap> map, QString path);

  QStringList toQStringList(char* argv[], int argc);

protected:
  virtual QString _getHelpPath() const;

};

}

#endif // BASECOMMAND_H
