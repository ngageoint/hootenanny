There was a bug causing all the parks to be conflated into buildings. I've fixed
that now, but the test remains.

Because the tennis court name matches well enough with the main park polygon
we have matches between:

 * tennis POI <-> park polygon
 * tennis POI <-> tennis polygon

However,

  * tennis polygon does not match park polygon

The Tennis POI to park polygon is clearly the more correct match, and originally due to
the conflict we labelled the whole thing as needing review.  This lead to a large increase
in unecessary reviews.  Adding a restriction on not matching sport fields to parks allows the
sport POI to match the sport poly and solves this issue.
