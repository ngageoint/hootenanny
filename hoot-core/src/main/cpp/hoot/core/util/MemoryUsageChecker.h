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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef MEMORY_USAGE_CHECKER_H
#define MEMORY_USAGE_CHECKER_H

namespace hoot
{

/**
 * Utility for checking available memory (Singleton)
 *
 * This has been put to use so far in the most likely locations to use large amounts of memory but
 * could be potentially added anywhere in the code. Use memory.usage.checker.interval if calling
 * in a loop.
 *
 * @todo This should probably be integrated with max.memory.usage setting to where the warning is
 * logged based on what percentage the process memory is of that value instead of the total
 * available.
 */
class MemoryUsageChecker
{

public:

  static MemoryUsageChecker& getInstance();

  /**
   * Checks the available physical and virtual memory and logs a single message when it drops below
   * a configurable threshold
   */
  void check();

private:

  MemoryUsageChecker();
  /** Default constructor/destructor */
  ~MemoryUsageChecker() = default;
  /** Delete copy constructor and assignment operator */
  MemoryUsageChecker(const MemoryUsageChecker&) = delete;
  MemoryUsageChecker& operator=(const MemoryUsageChecker&) = delete;

  bool _enabled;
  // memory usage percentage threshold above which a notification is logged
  double _notificationThreshold;

  bool _loggedPhysicalMemNotification;
  bool _loggedVirtualMemNotification;
};

}

#endif // MEMORY_USAGE_CHECKER_H
