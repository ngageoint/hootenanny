#!/bin/bash
set -e

function usage() {
  echo "Usage: ConflateDirectory.sh [OPTIONS...] [START POSITION] DIRECTORY..."
  echo ""
  echo "  -h, --help                show usage"
  echo "  --network                 use Hootenanny network conflation algorithm"
  echo "  --parallel                run conflation jobs in parallel"
  echo "  --remove-disconnected     remove small and completely disconnected roads"
  echo "  --remove-unlikely         remove features that are unlikely to be roads"
  echo "  --resolve                 resolve reviews automatically"
  echo "  --snap-unconnected        snap roads unconnected on one end to nearby roads"
  echo "  --quiet                   run Hootenanny in quiet mode"
}

START_POSITION=0
ALGORITHM_CONF="UnifyingAlgorithm.conf -D match.creators=hoot::HighwayMatchCreator -D merger.creators=hoot::HighwayMergerCreator"
FILE_PATH=
PARALLEL="no"
RESOLVE_REVIEWS=
REMOVE_DISCONNECTED=
SNAP_UNCONNECTED=
REMOVE_UNLIKELY=
QUIET="--status"

if [ $# -eq 0 ]
then
  usage
  exit
fi

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
  elif [ $ARGUMENT == "--remove-disconnected" ]
  then
    REMOVE_DISCONNECTED=" -D small.disconnected.way.remover.max.length=20.0 -D small.disconnected.way.remover.max.node.count=3 -D conflate.pre.ops+=hoot::SmallDisconnectedWayRemover"
  elif [ $ARGUMENT == "--parallel" ]
  then
    PARALLEL="yes"
  elif [ $ARGUMENT == "--resolve" ]
  then
    RESOLVE_REVIEWS=" -D resolve.review.type=resolve -D conflate.post.ops+=hoot::ResolveReviewsOp"
  elif [ $ARGUMENT == "--snap-unconnected" ]
  then
    SNAP_UNCONNECTED=" -D snap.unconnected.ways.snap.tolerance=7.0 -D snap.unconnected.ways.snap.way.statuses=Input1 -D snap.unconnected.ways.snap.to.way.statuses=Input1 -D conflate.post.ops+=hoot::UnconnectedWaySnapper"
  elif [ $ARGUMENT == "--remove-unlikely" ]
  then
    REMOVE_UNLIKELY=" -D unlikely.road.remover.max.heading.variance=60.0 -D unlikely.road.remover.max.length=25.0 -D conflate.pre.ops+=hoot::UnlikelyRoadRemover"
  elif [ $ARGUMENT == "-h" ] || [ $ARGUMENT == "--help" ]
  then
    usage
    exit
  elif [ $ARGUMENT == "--quiet" ]
  then
    QUIET="--error"
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

# The tag sources are the filenames of each file in the source directory
TAG_SOURCES=`echo $FILE_LIST | sed 's/.osm//g' | sed 's/ /;/g'`

CONFLATION_CONF=ReferenceConflation.conf

HOOT_OPTS="-C ${ALGORITHM_CONF} -C ${CONFLATION_CONF}"

# ops we don't need
HOOT_OPTS+=" -D conflate.pre.ops-=hoot::RemoveRoundabouts"
HOOT_OPTS+=" -D conflate.post.ops-=hoot::ReplaceRoundabouts"
HOOT_OPTS+=" -D conflate.post.ops-=hoot::RoadCrossingPolyReviewMarker"
HOOT_OPTS+=" -D conflate.post.ops-=hoot::AddHilbertReviewSortOrderOp"
HOOT_OPTS+=" -D writer.sort.tags.imagery.source=true"
HOOT_OPTS+=$RESOLVE_REVIEWS
HOOT_OPTS+=$REMOVE_DISCONNECTED
HOOT_OPTS+=$SNAP_UNCONNECTED
HOOT_OPTS+=$REMOVE_UNLIKELY

# for debugging only
# HOOT_OPTS+=" -D bounds=8.4762,12.0504,8.4793,12.0526 -D bounds.keep.entire.features.crossing.bounds=false"

# TODO: implement attribute transfer and divided highway handling

if [ $PARALLEL == "no" ]
then
  echo "-----------------------------------------------------------------------------------------------------------------------------------------"
  echo "conflate: ${FILE_ARRAY[$(( $START_POSITION + 1 ))]} with ${FILE_ARRAY[$START_POSITION]} to make conflation_$(( $START_POSITION + 1 )).osm"
  echo "-----------------------------------------------------------------------------------------------------------------------------------------"
  hoot conflate ${QUIET} ${HOOT_OPTS} \
    ${FILE_PATH}/${FILE_ARRAY[$(( $START_POSITION + 1 ))]} \
    ${FILE_PATH}/${FILE_ARRAY[$START_POSITION]} \
    ${OUTPUT_PATH}/conflation_$(( $START_POSITION + 1 )).osm

  for (( INDEX=$(( $START_POSITION + 2 )); INDEX<=$(( $LENGTH - 1 )); INDEX++ ))
  do
    echo "conflate: ${FILE_ARRAY[$INDEX]} with conflation_$(( $INDEX - 1 )).osm to make conflation_${INDEX}.osm"
    echo "-----------------------------------------------------------------------------------------------------------------------------------------"
    hoot conflate ${QUIET} ${HOOT_OPTS} \
      ${FILE_PATH}/${FILE_ARRAY[$INDEX]} \
      ${OUTPUT_PATH}/conflation_$(( $INDEX - 1 )).osm \
      ${OUTPUT_PATH}/conflation_${INDEX}.osm
  done
  CONVERT_SRC="${OUTPUT_PATH}/conflation_$(( $LENGTH - 1 )).osm"
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
    parallel --xapply hoot conflate ${QUIET} ${HOOT_OPTS} {1} {2} {3} ::: ${FILE_ARRAY_2[@]} ::: ${FILE_ARRAY_1[@]} ::: ${RESULT_ARRAY[@]}

    # After the parallel call is made, add in the "extra" file that wasn't conflated
    if [ $(( ${#SOURCE_ARRAY[@]} % 2 ))  == 1 ]
    then
      RESULT_ARRAY+=(${SOURCE_ARRAY[$(( ${#SOURCE_ARRAY[@]} - 1 ))]})
    fi

  done

  CONVERT_SRC=${RESULT_ARRAY[0]}
fi

hoot convert ${QUIET} -D convert.ops+="hoot::DataSummaryTagVisitor" -D data.summary.tag.sources="${TAG_SOURCES}" ${CONVERT_SRC} ${OUTPUT_PATH}/results.osm

