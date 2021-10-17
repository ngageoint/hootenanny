#!/bin/bash

OUT_DIR=test-output/cmd/glacial/SchemaCmdTest
mkdir -p $OUT_DIR
rm -f $OUT_DIR/*
CONFIG="-C Testing.conf"

# Due to the fact that the schema is constantly changing, we're merely testing that an output file
# is generated and not validating it in any way.

echo ""
echo "Writing asciidoc schema..."
echo ""
hoot schema $CONFIG > $OUT_DIR/schema.asciidoc
if [ ! -f "$OUT_DIR/schema.asciidoc" ]; then
  echo "$OUT_DIR/schema.asciidoc not created."
fi

echo ""
echo "Writing csv schema..."
echo ""
hoot schema $CONFIG > $OUT_DIR/schema.csv
if [ ! -f "$OUT_DIR/schema.csv" ]; then
  echo "$OUT_DIR/schema.csv not created."
fi
 
echo ""
echo "Writing html schema..."
echo ""
hoot schema $CONFIG > $OUT_DIR/schema.html
if [ ! -f "$OUT_DIR/schema.html" ]; then
  echo "$OUT_DIR/schema.html not created."
fi

# Only testing one image format here due to the runtime involved to generate the image.
echo ""
echo "Writing png schema..."
echo ""
hoot schema $CONFIG $OUT_DIR/schema.png
if [ ! -f "$OUT_DIR/schema.png" ]; then
  echo "$OUT_DIR/schema.png not created."
fi
