#!/bin/bash
set -e

hoot convert -D convert.ops=hoot::NonEnglishLanguageDetectionVisitor -D language.detection.detector=hoot::HootServicesLanguageDetectorMockClient -D language.info.provider=hoot::HootServicesTranslationInfoMockClient -D language.tag.keys="name;alt_name" test-files/visitors/ToEnglishTranslationVisitorTest/ToEnglishTranslationVisitorTest.osm test-output/cmd/quick/NonEnglishLanguageDetectionVisitorTest/output.osm
