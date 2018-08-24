
#ifndef NET_UTILS_H
#define NET_UTILS_H

// Qt
#include <QNetworkReply>

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
};

}

#endif // NET_UTILS_H
