These should conflate as expected without any unusual hitches.

The original error was occuring in NetworkDetails::getEdgeStringMatchScore.
The HausdorffDistanceExtractor test was using the whole relation, not the
subset specified in the way string.
