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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_NETWORK_REQUEST_H
#define HOOT_NETWORK_REQUEST_H

//  Boost
#include <boost/shared_ptr.hpp>

//  Qt
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>

namespace hoot
{

/** Class for sending HTTP network requests to an OSM API */
class HootNetworkRequest
{
public:
  /** Constructor */
  HootNetworkRequest();
  /**
   * @brief networkRequest Function to make the actual request
   * @param url URL for the request
   * @param http_op HTTP operation (GET, POST, or PUT), GET is default
   * @param data POST data as a QByteArray
   * @return success
   */
  bool networkRequest(QUrl url,
    QNetworkAccessManager::Operation http_op = QNetworkAccessManager::Operation::GetOperation,
    const QByteArray& data = QByteArray());
  /**
   * @brief networkRequest Function to make the actual request
   * @param url URL for the request
   * @param headers collection of known headers to set on the request
   * @param http_op HTTP operation (GET, POST, or PUT), GET is default
   * @param data POST data as a QByteArray
   * @return success
   */
  bool networkRequest(QUrl url,
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

private:
  bool _networkRequest(QUrl url, const QMap<QNetworkRequest::KnownHeaders, QVariant>& headers,
                       QNetworkAccessManager::Operation http_op, const QByteArray& data);
  /**
   * @brief _getHttpResponseCode Get the HTTP response code from the response object
   * @param reply Network reply object
   * @return HTTP response code as a number, 200 instead of "200"
   */
  int _getHttpResponseCode(QNetworkReply* reply);
  /** HTTP response body, if available */
  QByteArray _content;
  /** HTTP status response code  */
  int _status;
  /** Error string */
  QString _error;
};

typedef boost::shared_ptr<HootNetworkRequest> HootNetworkRequestPtr;

}

#endif  //  HOOT_NETWORK_REQUEST_H
