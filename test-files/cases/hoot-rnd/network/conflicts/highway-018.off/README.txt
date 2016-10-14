This is failing b/c it is failing to find a subline match. Dug into it a little
and found that MaximalSublineStringMatcher::_findBestMatch is returning 0 in
this case. This will require a bit more digging to figure out and is outside
the scope of network conflation.
