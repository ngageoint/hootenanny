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

#ifndef COMMAND_H
#define COMMAND_H

// Qt
#include <QString>

// Standard
#include <string>

namespace hoot
{

using namespace std;

/**
 * @defgroup cmd Command Line Interface
 */

class Command
{
public:

  static std::string className() { return "hoot::Command"; }

  Command();

  virtual ~Command() {}

  /**
   * Returns true if the command should be displayed in the help list.
   */
  virtual bool displayInHelp() const { return true; }

  /**
   * Returns a help message for the command. The help message can span multiple lines, but each
   * line should be no longer than 80 characters. The final line must _not_ end with a endl.
   */
  virtual QString getHelp() const = 0;

  /**
   * Returns the name of the command. This should take a form similar to "--my-command".
   */
  virtual QString getName() const = 0;

  virtual int run(char* argv[], int argc) = 0;

};

}

#endif // COMMAND_H
