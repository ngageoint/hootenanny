This was previously failing b/c it was failing to find a subline match. Dug into it a little
and found that MaximalSublineStringMatcher::_findBestMatch was returning 0 in
this case.
