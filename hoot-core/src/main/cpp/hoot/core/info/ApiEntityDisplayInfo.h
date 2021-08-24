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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef APIENTITYDISPLAYINFO_H
#define APIENTITYDISPLAYINFO_H

namespace hoot
{

/**
 * ApiEntity refers to aspects of the hoot API; e.g. FeatureExtractor.
 *
 * This class can probably be simplified (too much if/else logic).  See notes in InfoCmd about how
 * registration of associated classes could be made simpler for this.
 *
 * TODO: I think we can simplify some of the calls to factory code to instantiate elements to
 * create them by class name instead of type. Should get the big if/else statement in getDisplayInfo
 * down to something more reasonable.
 */
class ApiEntityDisplayInfo
{
public:

  static const int MAX_NAME_SIZE;
  static const int MAX_TYPE_SIZE;

  /**
   * Gets display information about an ApiEntity as a string
   *
   * @param apiEntityType the type of entity to display
   */
  static QString getDisplayInfo(const QString& apiEntityType);

  /**
   * Gets display information on the currently configured operations by the given name
   *
   * @param optName a Hootenanny configuration option name identifying a list of operations
   */
  static QString getDisplayInfoOps(const QString& optName);

private:

  static QString _apiEntityTypeForBaseClass(const QString& baseClassName);

  template<typename ApiEntity, typename ApiEntityChild>
  static QString _getApiEntities(
    const QString& apiEntityBaseClassName, const QString& apiEntityType, const bool displayType,
    const int maxNameSize);

  template<typename ApiEntity>
  static QString _getApiEntitiesForMatchMergerCreators(const QString& apiEntityClassName);

  static QString _getWaySnapCriteria();
};

}

#endif // APIENTITYDISPLAYINFO_H
