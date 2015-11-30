#include "NetworkDetails.h"

namespace hoot
{

NetworkDetails::NetworkDetails(ConstOsmMapPtr map, ConstOsmNetworkPtr network) :
  _map(map),
  _network(network)
{
}

}
