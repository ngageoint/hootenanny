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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/auth/HootServicesLoginManager.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

class LogoutCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::LogoutCmd"; }

  LogoutCmd() = default;

  virtual QString getName() const override { return "logout"; }

  virtual QString getDescription() const override
  { return "Logs a user out of the Hootenanny Web Services"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 0)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes zero parameters.").arg(getName()));
    }

    ConfigOptions config(conf());
    const QString userName = config.getHootServicesAuthUserName();
    const QString accessToken = config.getHootServicesAuthAccessToken();
    const QString accessTokenSecret = config.getHootServicesAuthAccessTokenSecret();

    const bool success =
      HootServicesLoginManager().logout(userName, accessToken, accessTokenSecret);

    if (success)
    {
      std::cout << "User: " << userName << " logged out of the Hootenanny Web Services." <<
        std::endl;
    }
    else
    {
      std::cout << "Unable to log user: " << userName << " out of the Hootenanny Web Services." <<
        std::endl;
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, LogoutCmd)

}

