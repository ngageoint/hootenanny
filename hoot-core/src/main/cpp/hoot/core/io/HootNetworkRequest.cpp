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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "HootNetworkRequest.h"

//  Hootenanny
#include <hoot/core/io/HootNetworkCookieJar.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootNetworkUtils.h>

//  Qt
#include <QEventLoop>
#include <QHostAddress>
#include <QTimer>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslSocket>

//  LibOAuthCpp
#include <liboauthcpp/liboauthcpp.h>

using namespace std;

namespace hoot
{

HootNetworkRequest::HootNetworkRequest()
  : _useOAuth1(false),
    _useOAuth2(false),
    _timedOut(false),
    _key_path(ConfigOptions().getHootPkcs12KeyPath()),
    _pass_phrase(ConfigOptions().getHootPkcs12KeyPhrase())
{
}

HootNetworkRequest::HootNetworkRequest(const QString& key_path, const QString& pass_phrase)
  : _useOAuth1(false),
    _useOAuth2(false),
    _timedOut(false),
    _key_path(key_path),
    _pass_phrase(pass_phrase)
{
}

HootNetworkRequest::HootNetworkRequest(const QString& consumer_key, const QString& consumer_secret,
                                       const QString& request_token, const QString& request_secret)
  : _useOAuth1(true),
    _useOAuth2(false),
    _timedOut(false),
    _key_path(ConfigOptions().getHootPkcs12KeyPath()),
    _pass_phrase(ConfigOptions().getHootPkcs12KeyPhrase())
{
  setOAuthKeys(consumer_key, consumer_secret, request_token, request_secret);
}

void HootNetworkRequest::setOAuthKeys(const QString& consumer_key, const QString& consumer_secret,
                                      const QString& request_token, const QString& request_secret)
{
  //  Initialize the consumer key
  _consumer = std::make_shared<OAuth::Consumer>(consumer_key.toStdString(), consumer_secret.toStdString());
  //  Initialize the request token
  _tokenRequest = std::make_shared<OAuth::Token>(request_token.toStdString(), request_secret.toStdString());
  //  Set the OAuth flag
  _useOAuth1 = true;
  _useOAuth2 = false;
}

HootNetworkRequest::HootNetworkRequest(const QString& access_token)
  : _useOAuth1(false),
    _useOAuth2(true),
    _oauth2AccessToken(access_token),
    _timedOut(false)
{
}

bool HootNetworkRequest::networkRequest(const QUrl& url, int timeout,
                                        QNetworkAccessManager::Operation http_op, const QByteArray& data)
{
  //  Call the actually network request function with empty headers map
  return _networkRequest(url, timeout, QMap<QNetworkRequest::KnownHeaders, QVariant>(), http_op, data);
}

bool HootNetworkRequest::networkRequest(const QUrl& url, int timeout,
                                        const QMap<QNetworkRequest::KnownHeaders, QVariant>& headers,
                                        QNetworkAccessManager::Operation http_op,
                                        const QByteArray& data)
{
  //  Simple passthrough function for consistency
  return _networkRequest(url, timeout, headers, http_op, data);
}

bool HootNetworkRequest::_networkRequest(const QUrl& url, int timeout,
                                         const QMap<QNetworkRequest::KnownHeaders, QVariant>& headers,
                                         QNetworkAccessManager::Operation http_op,
                                         const QByteArray& data)
{
  QUrl tempUrl(url);
  //  Reset status
  _status = 0;
  _content.clear();
  _error.clear();
  _timedOut = false;
  //  Do HTTP request
  std::shared_ptr<QNetworkAccessManager> pNAM = std::make_shared<QNetworkAccessManager>();
  QNetworkRequest request(url);

  if (tempUrl.scheme().toLower() == "https")
  {
    //  Setup the SSL configuration
    QSslConfiguration config(QSslConfiguration::defaultConfiguration());
    config.setProtocol(QSsl::SslProtocol::AnyProtocol);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    if (!_key_path.isEmpty())
    {
      if (!QFile::exists(_key_path))
      {
        LOG_ERROR("Unknown SSL key file path specified.");
        return false;
      }
      QFile file(_key_path);
      file.open(QFile::ReadOnly);

      QSslKey key;
      QSslCertificate certificate;
      QList<QSslCertificate> certChain;

      bool imported = QSslCertificate::importPkcs12(&file, &key, &certificate, &certChain, _pass_phrase.toUtf8());
      if (imported)
      {
        config.setCaCertificates(certChain);
        config.setLocalCertificate(certificate);
        config.setPrivateKey(key);
      }
    }
    request.setSslConfiguration(config);
  }
  //  Setup username/password authentication
  if (tempUrl.userInfo() != "")
  {
    //  Using basic authentication
    QString base64 = tempUrl.userInfo().toUtf8().toBase64();
    request.setRawHeader("Authorization", QString("Basic %1").arg(base64).toUtf8());
    tempUrl.setUserInfo("");
  }
  //  Add the known headers
  for (auto it = headers.begin(); it != headers.end(); ++it)
    request.setHeader(it.key(), it.value());
  //  Add any cookies if necessary
  if (_cookies)
  {
    pNAM->setCookieJar(_cookies.get());
    // don't want to take ownership of these cookies so they could potentially be shared across
    // different requests made by the same caller
    _cookies->setParent(nullptr);
  }
  //  Setup the OAuth header on the request object
  if (_useOAuth1 && _consumer && _tokenRequest)
    _setOAuth1Header(http_op, request);
  else if (_useOAuth2 && !_oauth2AccessToken.isEmpty())
    _setOAuth2Header(request);
  //  Setup timeout
  QEventLoop loop;
  QTimer timeoutTimer;
  //  Call the correct function on the network access manager
  QNetworkReply* reply = nullptr;
  switch (http_op)
  {
  case QNetworkAccessManager::GetOperation:
    reply = pNAM->get(request);
    break;
  case QNetworkAccessManager::PutOperation:
    reply = pNAM->put(request, data);
    break;
  case QNetworkAccessManager::PostOperation:
    reply = pNAM->post(request, data);
    break;
  default:
    return false;
    break;
  }
  //  Start the timer
  timeoutTimer.start(timeout * 1000);
  //  Connect the timeout lambda
  QObject::connect(&timeoutTimer, &QTimer::timeout,
  [this, &timeoutTimer, &reply, &tempUrl, &loop]()
  {
    //  Stop the timer first so the lambda isn't called again
    timeoutTimer.stop();
    //  Set the timed out flag before aborting the reply
    _timedOut = true;
    //  Abort will trigger QNetworkAccessManager::finished()
    reply->abort();
    //  Set the status to negative TimeoutError, negative so that it doesn't
    //  collide with HTTP status numbers
    _status = -1 * (int)QNetworkReply::TimeoutError;
    _error = QString("HTTP Request to %1 timed-out.").arg(tempUrl.toString(QUrl::RemoveUserInfo));
    //  Exit with a non-zero status
    loop.exit(_status);
  });
  //  Connect the finished lambda
  QObject::connect(reply, &QNetworkReply::finished,
  [this, &timeoutTimer, &loop]()
  {
    //  The finished signal is emitted when the reply is aborted in the
    //  timeout lambda, so don't do anything
    if (!_timedOut)
    {
      //  Stop the timer
      timeoutTimer.stop();
      _timedOut = false;
      //  Exit with a zero status
      loop.exit();
    }
  });
  //  Actually run the event loop waiting for either lambda to exit the loop
  if (loop.exec() != 0)
    return false;
  //  Read all of the data
  _content = reply->readAll();
  //  Get the status and content of the reply if available
  _status = _getHttpResponseCode(reply);
  //  Save off the reply headers
  QList<QByteArray> headerList = reply->rawHeaderList();
  for (const auto& header : qAsConst(headerList))
    _responseHeaders[QString(header)] = QString(reply->rawHeader(header));
  //  Check error status on our reply
  if (_status != HttpResponseCode::HTTP_OK)
  {
    _error = reply->errorString();
    //  Remove authentication information if present
    if (request.url() != tempUrl)
      _error.replace(request.url().toString(), tempUrl.toString(QUrl::RemoveUserInfo), Qt::CaseInsensitive);
    //  Replace the IP address in the error string with <host-ip>
    HootNetworkRequest::removeIpFromUrlString(_error, request.url());
    //  Actual non-HTTP response code errors (i.e. network errors, etc.) are handled differently
    if (_status == 0 && QNetworkReply::NoError != reply->error())
    {
      //  Negate the connection error as the status
      _status = -1 * (int)reply->error();
      return false;
    }
  }
  //  return successfully
  return true;
}

int HootNetworkRequest::_getHttpResponseCode(const QNetworkReply* reply) const
{
  if (reply != nullptr)
  {
    //  Get the status code
    QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //  Convert and return it if it is valid
    if (status.isValid())
      return status.toInt();
  }
  return 0;
}

void HootNetworkRequest::_setOAuth1Header(QNetworkAccessManager::Operation http_op, QNetworkRequest& request) const
{
  //  Convert the operation format
  OAuth::Http::RequestType op;
  switch (http_op)
  {
  case QNetworkAccessManager::GetOperation:   op = OAuth::Http::Get;    break;
  case QNetworkAccessManager::PutOperation:   op = OAuth::Http::Put;    break;
  case QNetworkAccessManager::PostOperation:  op = OAuth::Http::Post;   break;
  default:                                                              return;
  }
  //  Create the client object and get the HTTP header
  OAuth::Client requestClient(_consumer.get(), _tokenRequest.get());
  string header = requestClient.getHttpHeader(op, request.url().toString(QUrl::RemoveUserInfo).toStdString());
  //  Set the Authorization header for OAuth
  request.setRawHeader("Authorization", QString(header.c_str()).toUtf8());
}

void HootNetworkRequest::_setOAuth2Header(QNetworkRequest& request) const
{
  //  Set the Authorization header for OAuth2
  request.setRawHeader("Authorization", QString("Bearer ").append(_oauth2AccessToken).toUtf8());
}

void HootNetworkRequest::removeIpFromUrlString(QString& endpointUrl, const QUrl& url)
{
  QHostAddress host(url.host());
  if (!host.isNull())
    endpointUrl.replace(url.host(), "<host-ip>");
}

void HootNetworkRequest::logConnectionError() const
{
  if (_status < 0)
  {
    //  Negative status error messages are connection errors from the socket and not from the HTTP server
    LOG_ERROR("Connection Error: " << getErrorString() << " (" << (_status * -1) << ")");
  }
  else
  {
    LOG_ERROR("Unexpected Error: HTTP " << _status << " : " << getErrorString());
  }
}

}
