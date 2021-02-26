#!/bin/bash
set -e

FILE_PATH=$1
OUTPUT_PATH=${FILE_PATH%.*}
FILE_NAME=${OUTPUT_PATH##*/}

mkdir -p $OUTPUT_PATH

LLIST=`ogrinfo -ro -so $FILE_PATH | grep Line | awk '{print $2}'`

for LAYER in $LLIST; do
  echo "Converting: ${FILE_NAME} ${LAYER}"
  LAYER_DATE=`echo $LAYER | sed --regexp-extended 's|([0-9]+)-([0-9]+)-([0-9]+)_.*|\1-\2-\3|g'`
  LAYER_SRC=$LAYER

  hoot convert --error $HOOT_OPT \
    -D convert.ops=hoot::SetTagValueVisitor \
    -D convert.ops+=hoot::WayJoinerOp \
    -D way.joiner=hoot::NonIntersectionWayJoiner \
    -D set.tag.value.visitor.keys="highway;source:datetime;source:imagery" \
    -D set.tag.value.visitor.values="road;${LAYER_DATE}T00:00:00Z;${LAYER_SRC}" \
    -D element.criterion.negate=true \
    -D set.tag.value.visitor.element.criteria="hoot::NoInformationCriterion" \
    $FILE_PATH\;$LAYER "${OUTPUT_PATH}/${LAYER}.osm"

done
