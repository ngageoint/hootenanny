#!/bin/bash
set -e

hoot convert -D convert.ops=hoot::NonEnglishLanguageDetectionVisitor -D language.detection.detector=hoot::HootServicesLanguageDetectorMockClient test-files/visitors/ToEnglishTranslationVisitorTest/ToEnglishTranslationVisitorTest.osm test-output/cmd/quick/NonEnglishLanguageDetectionVisitorTest/output.osm
