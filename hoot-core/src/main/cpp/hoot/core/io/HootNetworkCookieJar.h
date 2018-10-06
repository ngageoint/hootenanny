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

#ifndef HOOT_NETWORK_COOKIE_JAR_H
#define HOOT_NETWORK_COOKIE_JAR_H

// Qt
#include <QNetworkCookieJar>

namespace hoot
{

/**
 * Customization of Qt's cookie store.  This primarily exists simply to check the contents of a
 * cookie store.  It may not be needed after upgrading to Qt 5.
 */
class HootNetworkCookieJar : public QNetworkCookieJar
{
public:

  HootNetworkCookieJar(QObject* parent = 0);

  int size() const { return allCookies().size(); }

  QString toString() const;
};

}

#endif // HOOT_NETWORK_COOKIE_JAR_H
