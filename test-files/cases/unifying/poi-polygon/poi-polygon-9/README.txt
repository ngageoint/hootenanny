There was a bug causing all the parks to be conflated into buildings. I've fixed
that now, but the test remains.

Because the tennis court name matches well enough with the main park polygon
we have matches between:

 * tennis POI <-> park polygon
 * tennis POI <-> tennis polygon

However,

  * tennis polygon does not match park polygon

The Tennis POI to tennis polygon is clearly the more correct match, so we match them.
