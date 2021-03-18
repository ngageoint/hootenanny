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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/auth/HootServicesLoginManager.h>

namespace hoot
{

class LoginCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::LoginCmd"; }

  LoginCmd() = default;

  virtual QString getName() const override { return "login"; }

  virtual QString getDescription() const override
  { return "Logs a user into the Hootenanny Web Services"; }

  virtual QString getType() const { return "rnd"; }

  virtual int runSimple(QStringList& args) override
  {
    if (!args.empty())
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes zero parameters.").arg(getName()));
    }

    HootServicesLoginManager loginManager;

    // get a request token and display the authorization url
    QString authUrl;
    const QString requestToken = loginManager.getRequestToken(authUrl);
    std::cout << std::endl << "Authorization URL: " << authUrl << std::endl << std::endl;

    // prompt user to auth through the 3rd party (OpenStreetMap, etc.)
    const QString verifier = loginManager.promptForAuthorizationVerifier();

    // verify the user's login, create the user, and get their id and user name
    QString userName;
    const long userId = loginManager.verifyUserAndLogin(requestToken, verifier, userName);

    // retrieve access tokens and display to the user
    QString accessToken;
    QString accessTokenSecret;
    loginManager.getAccessTokens(userId, accessToken, accessTokenSecret);
    std::cout << std::endl;
    std::cout << "Use the information below with all Hootenanny commands that require use of the Hootenanny Web Services:"
              << std::endl << std::endl;
    std::cout << "-D hoot.services.auth.user.name=" << userName << " ";
    std::cout << "-D hoot.services.auth.access.token=" << accessToken << " ";
    std::cout << "-D hoot.services.auth.access.token.secret=" << accessTokenSecret << std::endl
              << std::endl;
    std::cout << "The access tokens will continue to be valid until you log out of the Hootenanny Web Services with 'hoot logout' or they expire due to a session timeout." << std::endl;

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, LoginCmd)

}

