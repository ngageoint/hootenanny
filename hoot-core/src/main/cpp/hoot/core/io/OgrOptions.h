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
#ifndef OGROPTIONS_H
#define OGROPTIONS_H

// Qt
#include <QMap>
#include <QString>

// Standard
#include <stdlib.h>

namespace hoot
{

/**
 * OGR uses an old school C approach to encoding options as parameters. This class exposes a simple
 * interface to manipulating the options and then a function to encode the options for OGR. The
 * encoded options are only good for the life time of this class or until a new set is generated
 * again.
 */
class OgrOptions : public QMap<QString, QString>
{
public:

  OgrOptions();

  ~OgrOptions();

  void deleteCrypticOptions();

  /**
   * Returns an old school C encoding of the options suitable for OGR parameters.
   */
  char** getCrypticOptions();

private:

  char** _cryptic;
};

}

#endif // OGROPTIONS_H
