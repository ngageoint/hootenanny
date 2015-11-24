#include "IterativeNetworkMatcher.h"

namespace hoot
{

IterativeNetworkMatcher::IterativeNetworkMatcher()
{
}

void IterativeNetworkMatcher::matchNetworks(OsmNetworkPtr n1, OsmNetworkPtr n2)
{
  // create an initial estimation of node matching based on proximity & way count
  // create an initial estimation of edge match based on typical similarity scores

  // while criteria isn't met
    // create a more refined estimate of node match based on the proximity & way count as well
    // as the neighboring edge scores (product of neighboring scores?)

    // create a more refined estimate of edge match based on the typical similarity scores and
    // the neighboring node scores (product of neighboring scores?)

  // threshold all intersection scores by some threshold

  // threshold all edge scores by some threshold

  // determine all conflicting matches

  // optimize the set of intersection matches based on score

  // populate a network match data structure
}

}
