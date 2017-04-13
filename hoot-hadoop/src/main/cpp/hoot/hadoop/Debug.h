#ifndef DEBUG_H
#define DEBUG_H

// Boost
#include <boost/shared_ptr.hpp>

// Standard
#include <set>

namespace hoot
{
class OsmMap;

using namespace std;
using namespace boost;

class Debug
{
public:

  static const set<long>& getTroubledNodes() { init(); return _troubledNodes; }

  static const set<long>& getTroubledWays() { init(); return _troubledWays; }

  /**
   * inline wrapper for _init()
   */
  static void init() { if (_initialized == false) { _init(); } }

#ifdef DEBUG
  static bool isTroubledNode(long nid)
  {
    init();
    return getTroubledNodes().find(nid) != getTroubledNodes().end();
  }

  static bool isTroubledWay(long wid)
  {
    init();
    return getTroubledWays().find(wid) != getTroubledWays().end();
  }

  /**
   * Logs any troubled nodes.
   * @returns true if a troubled node or way is found.
   */
  static bool printTroubled(const boost::shared_ptr<const OsmMap>& map);

#else
  static bool isTroubledNode(long) { return false; }

  static bool isTroubledWay(long) { return false; }

  static bool printTroubled(const boost::shared_ptr<const OsmMap>&) { return false; }

#endif

private:
  static bool _initialized;
  static set<long> _troubledNodes;
  static set<long> _troubledWays;

  static void _init();
};

}

#endif // DEBUG_H
