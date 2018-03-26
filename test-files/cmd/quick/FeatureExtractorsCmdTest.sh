#!/bin/bash
set -e

# Since the extractors will constantly be changing, just going to grep out one that is unlikely to change any time soon.

hoot feature-extractors | grep "SampledAngleHistogramExtractor"
