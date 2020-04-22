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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "SystemInfo.h"

#ifdef _WIN32
// Windows Includes
# include <windows.h>
#elif defined(linux) || defined(__linux)
# include <unistd.h>
#endif

// standard
#include <unistd.h>
#include <iomanip>
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "sys/types.h"
#include "sys/sysinfo.h"

// tgs
#include <tgs/TgsException.h>

namespace Tgs
{
  int SystemInfo::getNumberOfProcessors()
  {
    int result;
#   ifdef _WIN32
      SYSTEM_INFO info;
      GetSystemInfo(&info);
      result = info.dwNumberOfProcessors;
#   elif defined(linux) || defined(__linux)
      result = sysconf( _SC_NPROCESSORS_ONLN );
#   else
#     warning("Warning: SystemInfo::getNumberOfProcessors() has not been implemented for this platform.")
      result = 1;
#   endif
    return result;
  }

# ifdef _WIN32
#   error "Memory usage stats not defined for WIN32"
# endif
  void SystemInfo::_getCurrentProcessMemoryUsage(long& vmUsage, long& residentSet)
  {
    using std::ios_base;
    using std::ifstream;
    using std::string;

    vmUsage     = 0.0;
    residentSet = 0.0;

    // 'file' stat seems to give the most reliable results
    ifstream stat_stream("/proc/self/stat", ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE); // in case x86-64 is configured to use 2MB pages
    vmUsage     = vsize;
    residentSet = rss * page_size_kb;

    if (vmUsage == 0.0 && residentSet == 0.0)
    {
      throw Tgs::Exception("Error retrieving memory usage.");
    }
  }

  std::string SystemInfo::humanReadableStorageSize(long bytes)
  {
    std::stringstream ss;

    ss << std::setprecision(4);
    if (bytes >= 1024l * 1024l * 1024l * 1024l)
    {
      ss << (double)(bytes / (1024.0 * 1024.0 * 1024.0 * 1024.0)) << "TB";
    }
    if (bytes >= 1024l * 1024l * 1024l)
    {
      ss << (double)(bytes / (1024.0 * 1024.0 * 1024.0)) << "GB";
    }
    else if (bytes >= 1024 * 1024)
    {
      ss << (double)(bytes / (1024.0 * 1024.0)) << "MB";
    }
    else if (bytes >= 1024)
    {
      ss << (double)(bytes / 1024.0) << "KB";
    }
    else
    {
      ss << bytes << "B";
    }

    return ss.str();
  }

  std::string SystemInfo::getCurrentProcessMemoryUsageString()
  {
    std::stringstream ss;
    long vm;
    long rss;
    _getCurrentProcessMemoryUsage(vm, rss);

    ss << "Process memory usage: virtual:\t" << humanReadableStorageSize(vm) << "\tphysical:\t" <<
          humanReadableStorageSize(rss);

    return ss.str();
  }

  std::string SystemInfo::getMemoryDetailString()
  {
    std::stringstream ss;

    ss << "Total system virtual memory: " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getTotalSystemVirtualMemory()) <<
          "\n";
    ss << "System virtual memory used: " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getSystemVirtualMemoryUsed()) <<
          "\n";
    ss << "Current process virtual memory usage (kb): " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getCurrentProcessVirtualMemoryUsage()) <<
          "\n";
    ss << "Virtual memory available: " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getVirtualMemoryAvailable()) << "\n";
    ss << "Percentage of virtual memory used: " <<
          SystemInfo::getPercentageOfVirtualMemoryUsed() << "%" << "\n";

    ss << "Total system physical memory: " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getTotalSystemPhysicalMemory()) <<
          "\n";
    ss << "System physical memory used: " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getSystemPhysicalMemoryUsed()) <<
          "\n";
    ss << "Current process physical memory usage: " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getCurrentProcessPhysicalMemoryUsage()) <<
          "\n";
    ss << "Physical memory available: " <<
          SystemInfo::humanReadableStorageSize(SystemInfo::getPhysicalMemoryAvailable()) <<
          "\n";
    ss << "Percentage of physical memory used: " <<
          SystemInfo::getPercentageOfPhysicalMemoryUsed() << "%" << "\n";

    return ss.str();
  }

  long SystemInfo::getTotalSystemVirtualMemory()
  {
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long totalVirtualMem = memInfo.totalram;
    //Add other values in next statement to avoid int overflow on right hand side...
    totalVirtualMem += memInfo.totalswap;
    totalVirtualMem *= memInfo.mem_unit;
    return (long)totalVirtualMem;
  }

  long SystemInfo::getSystemVirtualMemoryUsed()
  {
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long virtualMemUsed = memInfo.totalram - memInfo.freeram;
    //Add other values in next statement to avoid int overflow on right hand side...
    virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
    virtualMemUsed *= memInfo.mem_unit;
    return (long)virtualMemUsed;
  }

  long SystemInfo::getCurrentProcessVirtualMemoryUsage()
  {
    long v;
    long rss;
    _getCurrentProcessMemoryUsage(v, rss);
    return v;
  }

  long SystemInfo::getVirtualMemoryAvailable()
  {
    return getTotalSystemVirtualMemory() - getSystemVirtualMemoryUsed();
  }

  double SystemInfo::getPercentageOfVirtualMemoryUsed()
  {
    return (double)getSystemVirtualMemoryUsed() /  (double)getTotalSystemVirtualMemory();
  }

  double SystemInfo::getPercentageOfVirtualMemoryUsedByCurrentProcess()
  {
    return (double)getCurrentProcessVirtualMemoryUsage() /  (double)getTotalSystemVirtualMemory();
  }

  long SystemInfo::getTotalSystemPhysicalMemory()
  {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long long totalPhysMem = memInfo.totalram;
    // Multiply in next statement to avoid int overflow on right hand side...
    totalPhysMem *= memInfo.mem_unit;
    return (long)totalPhysMem;
  }

  long SystemInfo::getSystemPhysicalMemoryUsed()
  {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    // Multiply in next statement to avoid int overflow on right hand side...
    physMemUsed *= memInfo.mem_unit;
    return (long)physMemUsed;
  }

  long SystemInfo::getCurrentProcessPhysicalMemoryUsage()
  {
    long v;
    long rss;
    _getCurrentProcessMemoryUsage(v, rss);
    return rss;
  }

  long SystemInfo::getPhysicalMemoryAvailable()
  {
    return getTotalSystemPhysicalMemory() - getSystemPhysicalMemoryUsed();
  }

  double SystemInfo::getPercentageOfPhysicalMemoryUsed()
  {
    return (double)getSystemPhysicalMemoryUsed() /  (double)getTotalSystemPhysicalMemory();
  }

  double SystemInfo::getPercentageOfPhysicalMemoryUsedByCurrentProcess()
  {
    return (double)getCurrentProcessPhysicalMemoryUsage() /  (double)getTotalSystemPhysicalMemory();
  }
}
