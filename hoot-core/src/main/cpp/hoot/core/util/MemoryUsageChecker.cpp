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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MemoryUsageChecker.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// tgs
#include <tgs/System/SystemInfo.h>

namespace hoot
{

MemoryUsageCheckerPtr MemoryUsageChecker::_theInstance;

MemoryUsageChecker::MemoryUsageChecker() :
_enabled(true),
_notificationThreshold(0.95),
_loggedPhysicalMemNotification(false),
_loggedVirtualMemNotification(false)
{
  setConfiguration(conf());
}

const MemoryUsageCheckerPtr& MemoryUsageChecker::getInstance()
{
  if (_theInstance.get() == 0)
  {
    _theInstance.reset(new MemoryUsageChecker());
  }
  return _theInstance;
}

void MemoryUsageChecker::setConfiguration(const Settings& conf)
{
  if (this == _theInstance.get())
  {
    throw HootException("Please do not set the configuration on the singleton instance.");
  }

  ConfigOptions opts(conf);
  _enabled = opts.getMemoryUsageCheckerEnabled();
  const int thresholdVal = opts.getMemoryUsageCheckerThreshold();
  if (thresholdVal < 1 || thresholdVal > 100)
  {
    throw IllegalArgumentException(
      "MemoryUsageChecker notification threshold must a percentage value from 1 to 100.");
  }
  _notificationThreshold = (double)thresholdVal / 100.0;
}

void MemoryUsageChecker::check()
{
  // Check the amount of memory available that's left in the environment (after what the OS uses,
  // plus all running hoot jobs, etc.). If its less than the threshold, log a warning and also let
  // the user know what percentage of the total memory their job was taking up. Only logging once so
  // the logs don't get cluttered up for jobs whose memory consumption goes repeatedly above and
  // below the threshold. We may end up really needing to only check physical memory but will check
  // both it and virtual memory consumption for now.

  if (_enabled && !_loggedPhysicalMemNotification &&
      Tgs::SystemInfo::getPercentageOfPhysicalMemoryUsed() >= _notificationThreshold)
  {
    LOG_WARN(
      "Total physical memory usage has exceeded a threshold of " <<
      QString::number(_notificationThreshold * 100) << "% available. This Hootenanny process is " <<
      "using " <<
      QString::number(
        Tgs::SystemInfo::getPercentageOfPhysicalMemoryUsedByCurrentProcess() * 100, 'g', 2) +
      "% of the total available physical memory.");
    _loggedPhysicalMemNotification = true;
  }

  if (_enabled && !_loggedVirtualMemNotification &&
      Tgs::SystemInfo::getPercentageOfVirtualMemoryUsed() >= _notificationThreshold)
  {
    LOG_WARN(
      "Total virtual memory usage has exceeded a threshold of " <<
      QString::number(_notificationThreshold * 100) << "% available. This Hootenanny process is " <<
      "using " <<
      QString::number(
        Tgs::SystemInfo::getPercentageOfVirtualMemoryUsedByCurrentProcess() * 100, 'g', 2) +
      "% of the total available virtual memory.");
    _loggedVirtualMemNotification = true;
  }
}

}
