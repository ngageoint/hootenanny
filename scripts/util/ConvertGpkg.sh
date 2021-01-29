#!/bin/bash
set -e

FILE_PATH=$1
OUTPUT_PATH=${FILE_PATH%.*}
FILE_NAME=${OUTPUT_PATH##*/}

mkdir -p $OUTPUT_PATH

LLIST=`ogrinfo -ro -so $FILE_PATH | grep Line | awk '{print $2}'`

for LAYER in $LLIST; do
  echo "Converting: ${FILE_NAME} ${LAYER}"
  hoot convert --error $HOOT_OPT \
    -D convert.ops=hoot::SetTagValueVisitor \
    -D set.tag.value.visitor.keys="highway" \
    -D set.tag.value.visitor.values="road" \
    -D element.criterion.negate=true \
    -D set.tag.value.visitor.element.criteria="hoot::NoInformationCriterion" \
    $FILE_PATH\;$LAYER "${OUTPUT_PATH}/${LAYER}.osm"
done
