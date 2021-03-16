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

#ifndef HOOT_NETWORK_REQUEST_H
#define HOOT_NETWORK_REQUEST_H

//  Qt
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>

//  OAuth forward declarations
namespace OAuth
{
class Consumer;
class Token;
class Client;
}

namespace hoot
{

class HootNetworkCookieJar;

/** Class for sending HTTP network requests to an OSM API */
class HootNetworkRequest
{
public:
  /** Constructor */
  HootNetworkRequest();
  /**
   * @brief HootNetworkRequest
   * @param consumer_key - OAuth consumer key
   * @param consumer_secret - OAuth consumer secret key
   * @param access_token - OAuth access token from consumer key authentication
   * @param access_secret - OAuth access secret key from consumer key authentication
   */
  HootNetworkRequest(const QString& consumer_key, const QString& consumer_secret,
                     const QString& access_token, const QString& access_secret);
  /**
   * @brief networkRequest Function to make the actual request
   * @param url URL for the request
   * @param timeout Timeout in seconds to block waiting for the reply
   * @param http_op HTTP operation (GET, POST, or PUT), GET is default
   * @param data POST data as a QByteArray
   * @return success
   */
  bool networkRequest(const QUrl& url, int timeout,
    QNetworkAccessManager::Operation http_op = QNetworkAccessManager::Operation::GetOperation,
    const QByteArray& data = QByteArray());
  /**
   * @brief networkRequest Function to make the actual request
   * @param url URL for the request
   * @param timeout Timeout in seconds to block waiting for the reply
   * @param headers collection of known headers to set on the request
   * @param http_op HTTP operation (GET, POST, or PUT), GET is default
   * @param data POST data as a QByteArray
   * @return success
   */
  bool networkRequest(const QUrl& url, int timeout,
    const QMap<QNetworkRequest::KnownHeaders, QVariant>& headers,
    QNetworkAccessManager::Operation http_op = QNetworkAccessManager::Operation::GetOperation,
    const QByteArray& data = QByteArray());
  /**
   * @brief getResponseContent
   * @return HTTP response content
   */
  const QByteArray& getResponseContent() { return _content; }
  /**
   * @brief getHttpStatus
   * @return HTTP status code
   */
  int getHttpStatus() { return _status; }

  /**
   * @brief getErrorString
   * @return Error string from the last network request
   */
  QString getErrorString() { return _error; }

  /**
   * @brief getCookies
   * @return the cookies used by the request
   */
  std::shared_ptr<HootNetworkCookieJar> getCookies() const { return _cookies; }

  /**
   * @brief setCookies
   * @param cookies sets the cookies for the request
   */
  void setCookies(const std::shared_ptr<HootNetworkCookieJar>& cookies) { _cookies = cookies; }
  /**
   * @brief setOAuthKeys Enable OAuth authentication
   * @param consumer_key - OAuth consumer key
   * @param consumer_secret - OAuth consumer secret key
   * @param access_token - OAuth access token from consumer key authentication
   * @param access_secret - OAuth access secret key from consumer key authentication
   */
  void setOAuthKeys(const QString& consumer_key, const QString& consumer_secret,
                    const QString& access_token, const QString& access_secret);

  /**
   * @brief removeIpFromUrlString Replace the IP address in a URL with <host-ip> for logging purposes
   * @param endpointUrl Endpoint URL as a string (probably contains full path of API call)
   * @param url URL object for extracting the host to compare as an IP address or a domain name
   */
  static void removeIpFromUrlString(QString& endpointUrl, const QUrl& url);

  /**
   * @brief logConnectionError Log an error with the connection based on the reponse error, not an HTTP error code
   */
  void logConnectionError();

private:
  /**
   * @brief _networkRequest Function to make the actual request
   * @param url URL for the request
   * @param timeout Timeout in seconds to block waiting for the reply
   * @param headers collection of known headers to set on the request
   * @param http_op HTTP operation (GET, POST, or PUT), GET is default
   * @param data POST data as a QByteArray
   * @return success
   */
  bool _networkRequest(const QUrl& url, int timeout,
                       const QMap<QNetworkRequest::KnownHeaders, QVariant>& headers,
                       QNetworkAccessManager::Operation http_op, const QByteArray& data);
  /**
   * @brief _getHttpResponseCode Get the HTTP response code from the response object
   *   negative response codes mean there was a socket connection error and not an HTTP error
   *   see QNetworkReply::NetworkError enumeration for values
   * @param reply Network reply object
   * @return HTTP response code as an integer instead of a string
   */
  int _getHttpResponseCode(QNetworkReply* reply);
  /**
   * @brief _setOAuthHeader Sets the "Authorize: OAuth" HTTP header for the specific request
   * @param http_op OAuth signatures are based off of the HTTP operation type (GET/PUT/POST)
   * @param request Reference to the actual network request object
   */
  void _setOAuthHeader(QNetworkAccessManager::Operation http_op, QNetworkRequest& request);
  /** HTTP response body, if available */
  QByteArray _content;
  /** HTTP status response code  */
  int _status;
  /** Error string */
  QString _error;
  /** cookies to pass in with the request */
  std::shared_ptr<HootNetworkCookieJar> _cookies;
  /** Flag for using OAuth 1.0, all four of the keys/tokens (two in each object)
   *  below must be valid for `_useOAuth` to be true.
   */
  bool _useOAuth;
  /** OAuth 1.0 consumer object */
  std::shared_ptr<OAuth::Consumer> _consumer;
  /** OAuth 1.0 request token object */
  std::shared_ptr<OAuth::Token> _tokenRequest;
  /** Flag set when the timer times out */
  bool _timedOut;
};

using HootNetworkRequestPtr = std::shared_ptr<HootNetworkRequest>;

}

#endif  //  HOOT_NETWORK_REQUEST_H
