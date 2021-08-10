This test checks to see that appropriate roads crossing polygons are flagged for review.

You should not see footways crossing parks or parking lots flagged for review (see the shorter 
Missipillion Riverwalk footway). You should not see any roads that merely touch a building, etc. 
without crossing flagged for review, as that is probably just a minor mapping mistake (see the longer 
Missipillion Riverwalk footway).

For the most part, highway=secondary and more general highways crossing buildings or parking lots 
are fair game to be flagged for review, and there are three instances in the output (Foster Street, 
a short highway=service near the end of Foster Street, and Pearl Alley).

TODO: This suffers from the problem described in #4149, however the workaround in the Line.js 
isMatchCandidate method prevents it temporarily.
