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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "LibPostalInit.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>

// libpostal
#include <libpostal/libpostal.h>

namespace hoot
{

LibPostalInit::LibPostalInit()
{
  // failsafe
  if (!ConfigOptions().getAddressMatchEnabled())
  {
    throw IllegalArgumentException(
      "libpostal cannot be initialized due to address matching being disabled.");
  }

  // This init takes ~5 seconds normally.
  if (!libpostal_setup_datadir(ConfigOptions().getLibpostalDataDir().toUtf8().data()) ||
      !libpostal_setup_parser_datadir(ConfigOptions().getLibpostalDataDir().toUtf8().data()) ||
      !libpostal_setup_language_classifier_datadir(
        ConfigOptions().getLibpostalDataDir().toUtf8().data()))
  {
    throw HootException("libpostal setup failed.");
  }
}

LibPostalInit::~LibPostalInit()
{
  libpostal_teardown();
  libpostal_teardown_parser();
  libpostal_teardown_language_classifier();
}

LibPostalInit& LibPostalInit::getInstance()
{
  //  Local static singleton instance
  static LibPostalInit instance;
  return instance;
}

}
