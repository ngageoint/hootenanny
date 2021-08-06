#!/bin/bash
set -e

FILE_PATH=$1
OUTPUT_PATH=${FILE_PATH%.*}
FILE_NAME=${OUTPUT_PATH##*/}

mkdir -p $OUTPUT_PATH

LLIST=`ogrinfo -ro -so $FILE_PATH | grep Line | awk '{print $2}'`
CONVERT_OPS=" -D convert.ops=hoot::SetTagValueVisitor"
CONVERT_OPS+=" -D convert.ops+=hoot::WayJoinerOp"
# Add DuplicateNodeRemover and RemoveDuplicateWayNodesVisitor before OffsetIntersectionMergerOp
CONVERT_OPS+=" -D convert.ops+=hoot::DuplicateNodeRemover"
CONVERT_OPS+=" -D convert.ops+=hoot::RemoveDuplicateWayNodesVisitor"
CONVERT_OPS+=" -D convert.ops+=hoot::OffsetIntersectionMergerOp"

for LAYER in $LLIST; do
  echo "Converting: ${FILE_NAME} ${LAYER}"
  LAYER_DATE=`echo $LAYER | sed --regexp-extended 's|([0-9]+)-([0-9]+)-([0-9]+)[_-].*|\1-\2-\3|g'`
  LAYER_SRC=$LAYER

  hoot convert --error $HOOT_OPT \
    ${CONVERT_OPS} \
    -D way.joiner=hoot::NonIntersectionWayJoiner \
    -D set.tag.value.visitor.keys="highway;source:datetime;source:imagery" \
    -D set.tag.value.visitor.values="road;${LAYER_DATE}T00:00:00Z;${LAYER_SRC}" \
    -D element.criterion.negate=true \
    -D set.tag.value.visitor.element.criteria="hoot::NoInformationCriterion" \
    -D writer.sort.tags.by.key=true \
    $FILE_PATH\;$LAYER "${OUTPUT_PATH}/${LAYER}.osm"

done
