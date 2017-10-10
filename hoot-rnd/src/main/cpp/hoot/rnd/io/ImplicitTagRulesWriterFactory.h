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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef IMPLICITTAGRULESWRITERFACTORY_H
#define IMPLICITTAGRULESWRITERFACTORY_H

// Qt
#include <QString>

namespace hoot
{
class ImplicitTagRulesWriter;

/**
 * A factory for constructing implicit tag rule writers based on an output URL
 */
class ImplicitTagRulesWriterFactory
{
public:

  ImplicitTagRulesWriterFactory();

  /**
   * Creates an implicit tag rules writer, given an output URL
   *
   * @param url output location
   * @return an ImplicitTagRulesWriter object
   */
  boost::shared_ptr<ImplicitTagRulesWriter> createWriter(QString url);

  /**
   * Retrieves the Singleton instance for this factory
   *
   * @return a factory instance
   */
  static ImplicitTagRulesWriterFactory& getInstance();

private:

  static boost::shared_ptr<ImplicitTagRulesWriterFactory> _theInstance;
};

}

#endif // IMPLICITTAGRULESWRITERFACTORY_H
