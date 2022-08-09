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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#ifndef TEST_COMMAND_H
#define TEST_COMMAND_H

// Hoot
#include <hoot/core/cmd/BaseCommand.h>

namespace hoot
{

/**
 * @brief The Command class is the hoot-test version of the Command class.
 *
 * It exists to distinguish HootTest commands from hoot commands.
 */
class TestCommand : public BaseCommand
{
public:

  static QString className() { return "TestCommand"; }

  TestCommand() = default;
  ~TestCommand() override = default;
};

}

#endif // TEST_COMMAND_H
