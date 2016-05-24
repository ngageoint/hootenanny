#ifndef MATCHSCOREPROVIDER_H
#define MATCHSCOREPROVIDER_H

namespace hoot
{

/**
 * Provides match scores for specific aspects of a network.
 */
class MatchScoreProvider
{
public:

  /**
   * Returns the best estimate for the match score for the specified edges. Depending on the
   * class that implements this method the value may change over time.
   *
   * The same value should be returned regardless of the ordering of e1/e2.
   *
   * If there is no match or no score is available a zero is returned.
   */
  virtual double getEdgeMatchScore(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2) const = 0;

  /**
   * Returns the best estimate for the match score for the specified vertices. Depending on the
   * class that implements this method the value may change over time.
   *
   * The same value should be returned regardless of the ordering of v1/v2.
   *
   * If there is no match or no score is available a zero is returned.
   */
  virtual double getVertexMatchScore(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2) const = 0;
};

typedef shared_ptr<MatchScoreProvider> MatchScoreProviderPtr;
typedef shared_ptr<const MatchScoreProvider> ConstMatchScoreProviderPtr;

}

#endif // MATCHSCOREPROVIDER_H
