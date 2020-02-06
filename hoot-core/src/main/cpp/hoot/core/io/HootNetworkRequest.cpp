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

#include "HootNetworkRequest.h"

//  Hootenanny
#include <hoot/core/io/HootNetworkCookieJar.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

//  Qt
#include <QEventLoop>
#include <QHostAddress>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>

//  LibOAuthCpp
#include <liboauthcpp/liboauthcpp.h>

using namespace std;

namespace hoot
{

HootNetworkRequest::HootNetworkRequest()
  : _useOAuth(false)
{
}

HootNetworkRequest::HootNetworkRequest(const QString& consumer_key, const QString& consumer_secret,
                                       const QString& request_token, const QString& request_secret)
  : _useOAuth(true)
{
  setOAuthKeys(consumer_key, consumer_secret, request_token, request_secret);
}

void HootNetworkRequest::setOAuthKeys(const QString& consumer_key, const QString& consumer_secret,
                                      const QString& request_token, const QString& request_secret)
{
  //  Initialize the consumer key
  _consumer.reset(new OAuth::Consumer(consumer_key.toStdString(), consumer_secret.toStdString()));
  //  Initialize the request token
  _tokenRequest.reset(new OAuth::Token(request_token.toStdString(), request_secret.toStdString()));
  //  Set the OAuth flag
  _useOAuth = true;
}

bool HootNetworkRequest::networkRequest(const QUrl& url, QNetworkAccessManager::Operation http_op,
                                        const QByteArray& data)
{
  //  Call the actually network request function with empty headers map
  return _networkRequest(url, QMap<QNetworkRequest::KnownHeaders, QVariant>(), http_op, data);
}

bool HootNetworkRequest::networkRequest(const QUrl& url,
                                        const QMap<QNetworkRequest::KnownHeaders, QVariant>& headers,
                                        QNetworkAccessManager::Operation http_op,
                                        const QByteArray& data)
{
  //  Simple passthrough function for consistency
  return _networkRequest(url, headers, http_op, data);
}

bool HootNetworkRequest::_networkRequest(const QUrl& url,
                                         const QMap<QNetworkRequest::KnownHeaders, QVariant>& headers,
                                         QNetworkAccessManager::Operation http_op,
                                         const QByteArray& data)
{
  QUrl tempUrl(url);
  //  Reset status
  _status = 0;
  _content.clear();
  _error.clear();
  //  Do HTTP request
  std::shared_ptr<QNetworkAccessManager> pNAM(new QNetworkAccessManager());
  QNetworkRequest request(url);

  if (tempUrl.scheme().toLower() == "https")
  {
    //  Setup the SSL configuration
    QSslConfiguration config(QSslConfiguration::defaultConfiguration());
    config.setProtocol(QSsl::SslProtocol::AnyProtocol);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
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
  for (QMap<QNetworkRequest::KnownHeaders, QVariant>::const_iterator it = headers.begin(); it != headers.end(); ++it)
  {
    request.setHeader(it.key(), it.value());
  }
  //  Add any cookies if necessary
  if (_cookies)
  {
    pNAM->setCookieJar(_cookies.get());
    // don't want to take ownership of these cookies so they could potentially be shared across
    // different requests made by the same caller
    _cookies->setParent(0);
  }
  //  Setup the OAuth header on the request object
  if (_useOAuth && _consumer && _tokenRequest)
    _setOAuthHeader(http_op, request);
  //  Call the correct function on the network access manager
  QNetworkReply* reply = NULL;
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
  //  Wait for finished signal from reply object
  _blockOnReply(reply);
  //  Get the status and content of the reply if available
  _status = _getHttpResponseCode(reply);
  //  According to the documention this shouldn't ever happen
  if (reply == NULL)
  {
    throw HootException(QString("Network request error: GET/POST/PUT request failed to create reply object."));
    return false;
  }
  _content = reply->readAll();
  //  Check error status on our reply
  if (QNetworkReply::NoError != reply->error())
  {
    _error = reply->errorString();
    //  Remove authentication information if present
    if (request.url() != tempUrl)
      _error.replace(request.url().toString(), tempUrl.toString(QUrl::RemoveUserInfo), Qt::CaseInsensitive);
    //  Replace the IP address in the error string with <host-ip>
    HootNetworkRequest::removeIpFromUrlString(_error, request.url());
    return false;
  }

  //  return successfully
  return true;
}

void HootNetworkRequest::_blockOnReply(QNetworkReply* reply)
{
  if (reply != NULL)
  {
    //  Qt code to force the use of QNetworkReply to be synchronous
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
  }
}

int HootNetworkRequest::_getHttpResponseCode(QNetworkReply* reply)
{
  if (reply != NULL)
  {
    //  Get the status code
    QVariant status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //  Convert and return it if it is valid
    if (status.isValid())
      return status.toInt();
  }
  return -1;
}

void HootNetworkRequest::_setOAuthHeader(QNetworkAccessManager::Operation http_op, QNetworkRequest& request)
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

void HootNetworkRequest::removeIpFromUrlString(QString& endpointUrl, const QUrl& url)
{
  QHostAddress host(url.host());
  if (!host.isNull())
    endpointUrl.replace(url.host(), "<host-ip>");
}

}
