#!/bin/bash
set -e

START_POSITION=0
ALGORITHM_CONF="UnifyingAlgorithm.conf -D match.creators=hoot::HighwayMatchCreator -D merger.creators=hoot::HighwayMergerCreator"
FILE_PATH=
PARALLEL="no"

for ARGUMENT in "$@"
do
  if [ $ARGUMENT == "--network" ]
  then
    ALGORITHM_CONF="NetworkAlgorithm.conf -D match.creators=hoot::NetworkMatchCreator -D merger.creators=hoot::NetworkMergerCreator"
  elif [ -d $ARGUMENT ]
  then
    FILE_PATH=$ARGUMENT
    FILE_NAME=${FILE_PATH##*/}
    OUTPUT_PATH=$FILE_PATH/conflated
  elif [ $(( ARGUMENT )) -gt 0 ]
  then
    START_POSITION=$ARGUMENT
  elif [ $ARGUMENT == "--parallel" ]
  then
    PARALLEL="yes"
  else
    echo "Invalid argument: $ARGUMENT"
  fi
done

if [ -z $FILE_PATH ]
then
  echo "Invalid directory path: $FILE_PATH"
  exit -1
fi

mkdir -p $OUTPUT_PATH

FILE_LIST=`ls -l $FILE_PATH | grep .osm | awk '{print $9}'`
FILE_ARRAY=()

for FILE in $FILE_LIST; do
  FILE_ARRAY+=($FILE)
done

LENGTH=${#FILE_ARRAY[@]}

CONFLATION_CONF=ReferenceConflation.conf

HOOT_OPTS="--status -C ${ALGORITHM_CONF} -C ${CONFLATION_CONF}"
HOOT_OPTS+=" -D conflate.pre.ops=hoot::MapCleaner"
#TODO: Figure out how to make these work with parallel
#HOOT_OPTS+=" -D map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::SmallHighwayMerger"
#HOOT_OPTS+=" -D conflate.post.ops=hoot::RemoveMissingElementsVisitor;hoot::SuperfluousNodeRemover;hoot::SmallHighwayMerger;hoot::RemoveMissingElementsVisitor;hoot::RemoveInvalidReviewRelationsVisitor;hoot::RemoveDuplicateReviewsOp;hoot::RemoveInvalidRelationVisitor;hoot::RemoveInvalidMultilineStringMembersVisitor;hoot::SuperfluousWayRemover;hoot::RemoveDuplicateWayNodesVisitor;hoot::DuplicateWayRemover;hoot::RemoveDuplicateRelationMembersVisitor;hoot::RemoveEmptyRelationsOp;hoot::RelationCircularRefRemover;hoot::AddHilbertReviewSortOrderOp"
HOOT_OPTS+=" -D highway.review.threshold=1.0"
HOOT_OPTS+=" -D highway.match.threshold=0.5"
HOOT_OPTS+=" -D highway.miss.threshold=0.7"

if [ $PARALLEL == "no" ]
then
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
else
  CONFLATION=1

  RESULT_ARRAY=()
  for (( INDEX=$START_POSITION; INDEX <= $(( $LENGTH - 1)); INDEX++ ))
  do
    RESULT_ARRAY+=("$FILE_PATH/${FILE_ARRAY[$INDEX]}")
  done

  while [ ${#RESULT_ARRAY[@]} -gt 1 ]
  do
    FILE_ARRAY_1=()
    FILE_ARRAY_2=()
    SOURCE_ARRAY=("${RESULT_ARRAY[@]}")
    RESULT_ARRAY=()
    # Split the result array in half
    for (( INDEX=0; INDEX<$(( ${#SOURCE_ARRAY[@]} - 1 )); INDEX+=2 ))
    do
      FILE_ARRAY_1+=(${SOURCE_ARRAY[$INDEX]})
      FILE_ARRAY_2+=(${SOURCE_ARRAY[$(( $INDEX + 1 ))]})
      RESULT_ARRAY+=(${OUTPUT_PATH}/conflation_${CONFLATION}.osm)
      CONFLATION=$(( $CONFLATION + 1))
    done

    # Run hoot conflate in parallel with newer file as the reference dataset with the older as the secondary
    parallel --xapply hoot conflate ${HOOT_OPTS} {1} {2} {3} ::: ${FILE_ARRAY_2[@]} ::: ${FILE_ARRAY_1[@]} ::: ${RESULT_ARRAY[@]}

    # After the parallel call is made, add in the "extra" file that wasn't conflated
    if [ $(( ${#SOURCE_ARRAY[@]} % 2 ))  == 1 ]
    then
      RESULT_ARRAY+=(${SOURCE_ARRAY[$(( ${#SOURCE_ARRAY[@]} - 1 ))]})
    fi

  done

fi

