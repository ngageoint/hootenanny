#!/bin/bash

mkdir -p test-output/cmd/quick/
INPATH=test-files/cmd/quick/MetadataOpsTest

# export test
INPUTFILE_EXPORT=$INPATH/MetadataExportOpsTestIn.osm
COMPAREFILE_EXPORT=$INPATH/MetadataExportOpsTestOut.osm
OUTPUTFILE_EXPORT=test-output/cmd/quick/MetadataExportOpsTestOut.osm

EXPORT_CONF="
-D convert.ops=hoot::MetadataExport
-D metadata.dataset.indicator.tag=testtag;testvalue
-D metadata.tags=source;unknown;surface;ground
-D metadata.grid.cell.size=0.05"

hoot convert -C Testing.conf $EXPORT_CONF $INPUTFILE_EXPORT $OUTPUTFILE_EXPORT
hoot diff -C Testing.conf $COMPAREFILE_EXPORT $OUTPUTFILE_EXPORT

# import test
INPUTFILE_IMPORT=$INPATH/MetadataImportOpsTestIn.osm
COMPAREFILE_IMPORT=$INPATH/MetadataImportOpsTestOut.osm
OUTPUTFILE_IMPORT=test-output/cmd/quick/MetadataImportOpsTestOut.osm

IMPORT_CONF="
-D convert.ops=hoot::MetadataImport
-D metadata.dataset.indicator.tag=testtag;testvalue
-D metadata.tags=source;unknown;surface;ground"

hoot convert -C Testing.conf $IMPORT_CONF $INPUTFILE_IMPORT $OUTPUTFILE_IMPORT
hoot diff -C Testing.conf $COMPAREFILE_IMPORT $OUTPUTFILE_IMPORT
