
// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/io/HootNetworkRequest.h>

namespace hoot
{

class LogoutCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::LogoutCmd"; }

  LogoutCmd() { }

  virtual QString getName() const { return "logout"; }

  virtual QString getDescription() const
  { return "Logs a user out of the Hootenanny Web Services"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    const QString userName = args[0];
    const QString accessToken = args[1];
    const QString accessTokenSecret = args[2];

    HootApiDb db;
    db.open(HootApiDb::getBaseUrl());
    if (!db.accessTokensAreValid(userName, accessToken, accessTokenSecret))
    {
      throw HootException("Unable to log out user: " + userName + ".  Invalid access tokens.");
    }

    // log the user out
    HootNetworkRequest logoutRequest;
    try
    {
      logoutRequest.networkRequest(ConfigOptions().getAuthLogoutEndpoint());
    }
    catch (const std::exception& e)
    {
      throw HootException("Error logging out user: " + userName + ". error: " + e.what());
    }
    if (logoutRequest.getHttpStatus() != 200)
    {
      throw HootException(
        "Error retrieving logging out user: " + userName + ". error: " +
        logoutRequest.getErrorString());
    }

    std::cout << "User: " << userName << " logged out of Hootenanny." << std::endl;

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, LogoutCmd)

}

