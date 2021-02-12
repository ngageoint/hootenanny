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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef COMMAND_H
#define COMMAND_H

// Qt
#include <QString>

// Standard
#include <string>

namespace hoot
{

/**
 * @defgroup cmd Command Line Interface
 *
 * Generally you will want to subclass BaseCommand rather than implementing this interface directly.
 */
class Command
{
public:

  static QString className() { return "hoot::Command"; }

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
   * Returns the name of the command. This should take a form similar to "my-command" and ideally
   * be 30 characters or less.
   */
  virtual QString getName() const = 0;

  /**
   * Returns a one sentence description for the command.
   *
   * Keep this as short as possible, capitalize the first letter, and check to see that it stays
   * on one line when displayed when typing 'hoot'.
   */
  virtual QString getDescription() const = 0;

  /**
   * BaseCommand for an example implementation.
   */
  virtual int run(char* argv[], int argc) = 0;

  /**
   * The 'type' of command; e.g. 'core' or 'rnd'
   *
   * @return the command typedef
   */
  virtual QString getType() const { return "core"; }
};

typedef std::shared_ptr<Command> CommandPtr;
typedef std::shared_ptr<const Command> ConstCommandPtr;

}

#endif // COMMAND_H
