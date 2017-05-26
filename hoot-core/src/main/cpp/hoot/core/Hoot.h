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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HOOT_H
#define HOOT_H

// Standard
#include <memory>

class QString;

namespace hoot
{

/**
 * A singleton to initialize hootenanny. You should call getInstance() on this before any other
 * use of hoot. You can call getInstance multiple times without any ill effects.
 */
class Hoot
{
public:

  static Hoot& getInstance();

  /**
   * A no-op that makes the code a little easier to read.
   */
  void init() {}

  /**
   * Load the specified library if it exists using QLibrary.
   */
  void loadLibrary(QString name);

  /**
   * Reinitialize hootenanny based on configuration changes.
   */
  void reinit();

private:
  static std::auto_ptr<Hoot> _theInstance;

  Hoot();

  void _init();

  long _toBytes(const QString& str) const;
};

}

#endif // HOOT_H
