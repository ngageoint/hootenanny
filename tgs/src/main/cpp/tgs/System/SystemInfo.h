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
 * @copyright Copyright (C) 2015, 2017, 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __SYSTEM_INFO_H__
#define __SYSTEM_INFO_H__

// Standard
#include <string>

#include <tgs/TgsExport.h>

namespace Tgs
{
  /**
   * Utilities for obtaining operating system info
   *
   * Portions of this source code are derived from the following:
   *
   * http://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-run-time-in-c
   * https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
   */
  class TGS_EXPORT SystemInfo
  {
  public:

    /**
     * Determines the number of processors available in the system
     *
     * @return the number of processors
     */
    static int getNumberOfProcessors();

    /**
     * Returns the amount of total configured system virtual memory
     *
     * @return memory size in bytes
     */
    static long getTotalSystemVirtualMemory();

    /**
     * Determines the amount of total system virtual memory in use
     *
     * @return memory size in bytes
     */
    static long getSystemVirtualMemoryUsed();

    /**
     * Determines the amount of virtual memory used by the calling process
     *
     * @return memory size in bytes
     */
    static long getCurrentProcessVirtualMemoryUsage();

    /**
     * Determines the amount of virtual memory available
     *
     * @return memory size in bytes
     */
    static long getVirtualMemoryAvailable();

    /**
     * Determines the percentage of total virtual memory in use out of the total configured system
     * virtual memory
     *
     * @return memory size in bytes
     */
    static double getPercentageOfVirtualMemoryUsed();

    /**
     * Determines the percentage of virtual memory in use by the calling process out of the total
     * configured system virtual memory
     *
     * @return memory size in bytes
     */
    static double getPercentageOfVirtualMemoryUsedByCurrentProcess();

    /**
     * Returns the amount of total configured system physical memory
     *
     * @return memory size in bytes
     */
    static long getTotalSystemPhysicalMemory();

    /**
     * Determines the amount of total system physical memory in use
     *
     * @return memory size in bytes
     */
    static long getSystemPhysicalMemoryUsed();

    /**
     * Determines the amount of physical memory used by the calling process
     *
     * @return memory size in bytes
     */
    static long getCurrentProcessPhysicalMemoryUsage();

    /**
     * Determines the amount of physicalv memory available
     *
     * @return memory size in bytes
     */
    static long getPhysicalMemoryAvailable();

    /**
     * Determines the percentage of total physical memory in use out of the total configured system
     * physical memory
     *
     * @return memory size in bytes
     */
    static double getPercentageOfPhysicalMemoryUsed();

    /**
     *
     * Determines the percentage of physical memory in use by the calling process out of the total
     * configured system physicalv memory
     *
     * @return memory size in bytes
     */
    static double getPercentageOfPhysicalMemoryUsedByCurrentProcess();

    /**
     * A human readable brief summary of the memory used by the calling process
     *
     * @return a string
     */
    static std::string getCurrentProcessMemoryUsageString();

    /**
     * A human readable detailed summary of the memory used by the calling process
     *
     * @return a string
     */
    static std::string getMemoryDetailString();

    /**
     * Converts bytes to human readable string
     *
     * @param bytes the number of bytes to convert
     * @return a string
     */
    static std::string humanReadableStorageSize(long bytes);

  private:

    friend class SystemInfoTest;

    /*
     * Attempts to read the system-dependent data for a process's virtual memory
     * size and resident set size, and return the results in bytes.
     *
     * @param vmUsage populates the current process's virtual memory size in bytes; on failure,
     * returns 0
     * @param residentSet populates the current process's resident set size in bytes; on failure,
     * returns 0
     */
    static void _getCurrentProcessMemoryUsage(long& vmUsage, long& residentSet);
  };
}

#endif
