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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef PROGRESS_REPORTER_H
#define PROGRESS_REPORTER_H

// Hoot
#include <hoot/core/util/Progress.h>

namespace hoot
{

/**
 * Simple interface for reporting job status from commands
 */
class ProgressReporter
{
public:

  ProgressReporter() = default;
  virtual ~ProgressReporter() = default;

  /**
   * Allows for passing in progress info for status updates.
   *
   * @param progress progress info; A copy initialized to the most recent progress state is passed
   * in to avoid maintaining progress with the same information across the entire application.
   */
  virtual void setProgress(const Progress& progress) = 0;

  /**
   * The number of job task steps performed by the implementer related to status progress reporting
   *
   * @return number of job steps for the progress task
   */
  virtual unsigned int getNumSteps() const = 0;
};

}

#endif // PROGRESS_REPORTER_H
