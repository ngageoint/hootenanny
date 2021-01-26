#!/bin/bash
set -e

START_POSITION=0
if [ $# -eq 2 ]
then
  START_POSITION=$2
fi

FILE_PATH=$1
FILE_NAME=${FILE_PATH##*/}
OUTPUT_PATH=$FILE_PATH/conflated

mkdir -p $OUTPUT_PATH

FILE_LIST=`ls -l $FILE_PATH | grep .osm | awk '{print $9}'`
FILE_ARRAY=()

for FILE in $FILE_LIST; do
  FILE_ARRAY+=($FILE)
done

LENGTH=${#FILE_ARRAY[@]}

CONFLATION_CONF=ReferenceConflation.conf
#ALGORITHM_CONF=NetworkAlgorithm.conf
ALGORITHM_CONF="UnifyingAlgorithm.conf -D match.creators=hoot::HighwayMatchCreator -D merger.creators=hoot::HighwayMergerCreator"

HOOT_OPTS="--status -C ${ALGORITHM_CONF} -C ${CONFLATION_CONF}"
HOOT_OPTS+=" -D conflate.pre.ops=hoot::MapCleaner"
HOOT_OPTS+=" -D map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::SmallHighwayMerger"
HOOT_OPTS+=" -D conflate.post.ops=hoot::RemoveMissingElementsVisitor;hoot::SuperfluousNodeRemover;hoot::SmallHighwayMerger;hoot::RemoveMissingElementsVisitor;hoot::RemoveInvalidReviewRelationsVisitor;hoot::RemoveDuplicateReviewsOp;hoot::RemoveInvalidRelationVisitor;hoot::RemoveInvalidMultilineStringMembersVisitor;hoot::SuperfluousWayRemover;hoot::RemoveDuplicateWayNodesVisitor;hoot::DuplicateWayRemover;hoot::RemoveDuplicateRelationMembersVisitor;hoot::RemoveEmptyRelationsOp;hoot::RelationCircularRefRemover;hoot::AddHilbertReviewSortOrderOp"
HOOT_OPTS+=" -D highway.review.threshold=1.0"
HOOT_OPTS+=" -D highway.match.threshold=0.5"
HOOT_OPTS+=" -D highway.miss.threshold=0.7"

echo "-----------------------------------------------------------------------------------------------------------------------------------------"
echo "conflate: ${FILE_ARRAY[$(( $START_POSITION + 1 ))]} with ${FILE_ARRAY[$START_POSITION]} to make conflation_$(( $START_POSITION + 1 )).osm"
echo "-----------------------------------------------------------------------------------------------------------------------------------------"
hoot conflate ${HOOT_OPTS} \
  ${FILE_PATH}/${FILE_ARRAY[$(( $START_POSITION + 1 ))]} \
  ${FILE_PATH}/${FILE_ARRAY[$START_POSITION]} \
  ${OUTPUT_PATH}/conflation_$(( $START_POSITION + 1 )).osm

for (( INDEX=$(( $START_POSITION + 2 )); INDEX<=$(( $LENGTH - 1 )); INDEX++ ))
do
  echo "-----------------------------------------------------------------------------------------------------------------------------------------"
  echo "conflate: ${FILE_ARRAY[$INDEX]} with conflation_$(( $INDEX - 1 )).osm to make conflation_${INDEX}.osm"
  echo "-----------------------------------------------------------------------------------------------------------------------------------------"
  hoot conflate ${HOOT_OPTS} \
    ${FILE_PATH}/${FILE_ARRAY[$INDEX]} \
    ${OUTPUT_PATH}/conflation_$(( $INDEX - 1 )).osm \
    ${OUTPUT_PATH}/conflation_${INDEX}.osm
done

