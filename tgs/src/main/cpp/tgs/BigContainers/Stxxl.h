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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef STXXL_H
#define STXXL_H

// Qt
#include <QTemporaryFile>
#include <QString>

// Standard
#include <memory>

namespace Tgs
{

/**
 * A convenience class for initializing STXXL in a multiple processes safe way (Singleton).
 */
class Stxxl
{
public:

  /**
   * Initializes the STXXL container.
   */
  static Stxxl& getInstance();

  /**
   * Sets the configuration file in such a way that ### will be replaced with the process ID. This
   * works with both v1.3 (where this is natively unsupported) and should work with v1.4 (untested).
   *
   * This must be called before any use of STXXL. If you call it after STXXL is initialized the
   * results are undefined.
   */
  void setConfig(QString configFile);

private:

  void _init() const;
  QString _removeComments(QString s) const;

  QTemporaryFile _configFileTmp;

  Stxxl();
  /** Default destructor */
  ~Stxxl() = default;
  /** Delete copy constructor and assignment operator */
  Stxxl(const Stxxl&) = delete;
  Stxxl& operator=(const Stxxl&) = delete;
};

}

#endif // STXXL_H
