
#ifndef NET_UTILS_H
#define NET_UTILS_H

// Qt
#include <QNetworkReply>
#include <QString>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

namespace hoot
{

/**
 * Various utilities for network requests
 */
class NetUtils
{

public:

  /**
   * Checks a net reply for error status
   *
   * @param reply reply to check
   */
  static void checkWebReplyForError(QNetworkReply* reply);

  /**
   * Converts a JSON net reply to a Boost property tree
   *
   * @param reply the reply to convert
   * @return a Boost property tree
   */
  static boost::shared_ptr<boost::property_tree::ptree> replyToPropTree(QNetworkReply* reply);
};

}

#endif // NET_UTILS_H
