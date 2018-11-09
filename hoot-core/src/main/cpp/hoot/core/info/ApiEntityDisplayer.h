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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef APIENTITYDISPLAYER_H
#define APIENTITYDISPLAYER_H

// Qt
#include <QString>

namespace hoot
{

/**
 * ApiEntity refers to aspects of the hoot API; e.g. FeatureExtractor.
 *
 * See notes in InfoCmd about how registration of associated classes could be made simpler for this.
 */
class ApiEntityDisplayer
{
public:

  /**
   * Displays information about an ApiEntity
   *
   * @param apiEntityType the type of entity to display
   */
  static void display(const QString apiEntityType);
};

}

#endif // APIENTITYDISPLAYER_H
