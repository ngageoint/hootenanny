#!/bin/bash
set -e

# example: 
#
# ./scripts/conflate/ConflateDirectory.sh 1 bison/kano_033133330302/ --quiet --parallel --reverse-inputs \
#   --remove-disconnected --remove-unlikely --resolve --snap-unconnected --suppress-divided-roads \
#   --attribute-file=bison/kano_033133330302-osm-cropped.osm
#
# Convert *.gpkg inputs to a file directory beforehand with scripts/util/ConvertGpkg.sh.

function usage() {
  echo "Usage: ConflateDirectory.sh [OPTIONS...] [START POSITION (1-based index)] DIRECTORY..."
  echo ""
  echo "  -h, --help                show usage"
  echo "  --attribute-file=[FILE]   full path to file to be used for attribute transfer; required by --suppress-divided-roads"
  echo "  --average                 merge features by averaging"
  echo "  --network                 use Hootenanny network conflation algorithm"
  echo "  --parallel                run conflation jobs in parallel"
  echo "  --quiet                   run Hootenanny in quiet mode"
  echo "  --remove-disconnected     remove small and completely disconnected roads"
  echo "  --remove-merge-relations  remove any multilinestring relations created as a result of merging"
  echo "  --remove-unlikely         remove features that are unlikely to be roads"
  echo "  --resolve                 resolve reviews automatically"
  echo "  --reverse-inputs          process the inputs in reverse order"
  echo "  --snap-unconnected        snap roads unconnected on one end to nearby roads"
  echo "  --suppress-divided-roads  keep divided roads from the starting input only"
}

START_POSITION=0
FILE_PATH=

QUIET="--status"
PARALLEL="no"
PARALLEL_DEBUG="--will-cite"
AVERAGE_MERGING="no"
# REVERSE_INPUTS=yes may be useful when using --suppress-divided-roads if the files are default ordered from oldest to newest 
# and the newest file has the best divided roads.
REVERSE_INPUTS="no"
RESOLVE_REVIEWS=
REMOVE_DISCONNECTED=
SNAP_UNCONNECTED=
REMOVE_UNLIKELY=
SUPRESS_DIVIDED_ROADS="no"
REMOVE_MERGE_RELATIONS=

ALGORITHM_CONF="UnifyingAlgorithm.conf -D match.creators=hoot::HighwayMatchCreator -D merger.creators=hoot::HighwayMergerCreator"

ATTRIBUTE_FILE=
INPUT_TAG_KEYS=
ATTRIBUTE_TRANSFER_OUTPUT_PATH=

# for debugging smaller AOIs only
AOI_OPTS= #" -D bounds=8.4762,12.0504,8.4793,12.0526 -D bounds.keep.entire.features.crossing.bounds=false"

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
    echo "Using the Network Roads Algorithm."
  elif [ -d $ARGUMENT ]
  then
    FILE_PATH=$ARGUMENT
    FILE_NAME=${FILE_PATH##*/}
    OUTPUT_PATH=$FILE_PATH/conflated
    ATTRIBUTE_TRANSFER_OUTPUT_PATH=$FILE_PATH/attribute-transfer
  elif [ $ARGUMENT == "--remove-disconnected" ]
  then
    REMOVE_DISCONNECTED=" -D small.disconnected.way.remover.max.length=20.0 -D small.disconnected.way.remover.max.node.count=3 -D conflate.pre.ops+=hoot::SmallDisconnectedWayRemover"
    echo "Configured to remove small disconnected ways."
  elif [ $ARGUMENT == "--parallel" ]
  then
    PARALLEL="yes"
  elif [ $ARGUMENT == "--reverse-inputs" ]
  then
    REVERSE_INPUTS="yes"
    echo "Configured to reverse inputs."
  elif [ $ARGUMENT == "--resolve" ]
  then
    RESOLVE_REVIEWS=" -D resolve.review.type=resolve -D conflate.post.ops+=hoot::ResolveReviewsOp"
    echo "Configured to automatically resolve reviews."
  elif [ $ARGUMENT == "--snap-unconnected" ]
  then
    SNAP_UNCONNECTED=" -D snap.unconnected.ways.snap.tolerance=7.0 -D snap.unconnected.ways.snap.way.statuses=Input1 -D snap.unconnected.ways.snap.to.way.statuses=Input1 -D conflate.post.ops+=hoot::UnconnectedWaySnapper"
    echo "Configured to snap unconnected roads."
  elif [ $ARGUMENT == "--remove-unlikely" ]
  then
    REMOVE_UNLIKELY=" -D unlikely.road.remover.max.heading.variance=60.0 -D unlikely.road.remover.max.length=25.0 -D conflate.pre.ops+=hoot::UnlikelyRoadRemover"
    echo "Configured to remove unlikely roads."
  elif [ $ARGUMENT == "--suppress-divided-roads" ]
  then
    SUPRESS_DIVIDED_ROADS="yes"
    echo "Configured to keep divided roads from starting input only."
  elif [[ $ARGUMENT == "--attribute-file"* ]]; then
    IFS='=' read -ra tmpArr <<< "$ARGUMENT"
    ATTRIBUTE_FILE=${tmpArr[1]}
    echo "Using attribute file: "$ATTRIBUTE_FILE
  elif [ $ARGUMENT == "--average" ]
  then
    AVERAGE_MERGING="yes"
    echo "Configured to merge features by averaging."
  elif [ $ARGUMENT == "--remove-merge-relations" ]
  then
    REMOVE_MERGE_RELATIONS=" -D multilinestring.relation.collapser.types=highway"
    echo "Configured to remove multilinestring relations created by merging."
  elif [ $ARGUMENT == "-h" ] || [ $ARGUMENT == "--help" ]
  then
    usage
    exit
  elif [ $ARGUMENT == "--quiet" ]
  then
    QUIET="--error"
  elif [ $(( ARGUMENT )) -gt 0 ]
  then
    START_POSITION=$ARGUMENT
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

LIST_OPTIONS="-l"
if [ $REVERSE_INPUTS == "yes" ]
then
  LIST_OPTIONS+=" -r"
fi
FILE_LIST=`ls $LIST_OPTIONS $FILE_PATH | grep .osm | awk '{print $9}'`
FILE_ARRAY=()
for FILE in $FILE_LIST; do
  FILE_ARRAY+=($FILE)
done
LENGTH=${#FILE_ARRAY[@]}

# The tag sources are the filenames of each file in the source directory.
TAG_LIST=`ls -l $FILE_PATH | grep .osm | awk '{print $9}'`
TAG_SOURCES=`echo $TAG_LIST | sed 's/.osm//g' | sed 's/ /;/g'`

# ATTRIBUTE TRANSFER

if [ $SUPRESS_DIVIDED_ROADS == "yes" ]
then
  if [ -z $ATTRIBUTE_FILE ]
  then
    echo "No attribute file specified for divided roads attribute transfer."
    exit -1
  else
    # Put the files with attributes transferred into their own directory, so that we can check them separately during debugging.
    mkdir -p $ATTRIBUTE_TRANSFER_OUTPUT_PATH
    # Clean out previous files with attributes transferred to them.
    rm -f ${ATTRIBUTE_TRANSFER_OUTPUT_PATH}/attribute_transfer_*
  
    # Get all tag keys from the starting input. We'll use those later to remove any attributes added as part of this 
    # attribute transfer process so that the output data looks like the input data. We're assuming all inputs have the same 
    # tag keys and only grabbing the list of tag keys from the starting input. If that ever isn't the case, this will have to 
    # be modified to get tag keys from all of the inputs instead.

    echo "Retrieving tag keys from starting input: ${FILE_ARRAY[$(( $START_POSITION - 1 ))]} ..."
    # list of tag keys delimited with ';'
    INPUT_TAG_KEYS=`hoot tag-info --warn --keys-only --delimited-text ${FILE_PATH}/${FILE_ARRAY[$(( $START_POSITION - 1 ))]}`
    
    # Run Attribute Conflation between the attribute file and every input to transfer attributes from the attribute file to 
    # the inputs.
  
    HOOT_ATTRIBUTE_TRANSFER_OPTS=" -C AttributeConflation.conf -C ${ALGORITHM_CONF}"
    # conflate ops we don't need
    HOOT_ATTRIBUTE_TRANSFER_OPTS+=" -D conflate.post.ops-=hoot::RoadCrossingPolyReviewMarker"
    HOOT_ATTRIBUTE_TRANSFER_OPTS+=" -D conflate.post.ops-=hoot::AddHilbertReviewSortOrderOp"
    HOOT_ATTRIBUTE_TRANSFER_OPTS+=$AOI_OPTS
    
    if [ $PARALLEL == "no" ]
    then
            
      for (( INDEX=0; INDEX<=$(( $LENGTH - 1 )); INDEX++ ))
      do
        ATTRIBUTE_TRANSFER_OUTPUT_FILE_NAME=attribute_transfer_${FILE_ARRAY[$INDEX]}
        ATTRIBUTE_TRANSFER_OUTPUT_FILE=${ATTRIBUTE_TRANSFER_OUTPUT_PATH}/$ATTRIBUTE_TRANSFER_OUTPUT_FILE_NAME
        INPUT_FILE=${FILE_PATH}/${FILE_ARRAY[$INDEX]}
        echo "Transferring attributes ($(( $INDEX + 1))/$LENGTH) to: ${FILE_ARRAY[$INDEX]} and writing output to: ${ATTRIBUTE_TRANSFER_OUTPUT_FILE_NAME} ..."
        echo   "-----------------------------------------------------------------------------------------------------------------------------------------"
        hoot conflate ${QUIET} ${HOOT_ATTRIBUTE_TRANSFER_OPTS} \
          ${INPUT_FILE} ${ATTRIBUTE_FILE} ${ATTRIBUTE_TRANSFER_OUTPUT_FILE}
      done
      
    else
    
      INPUT_ARRAY=()
      ATTRIBUTE_FILE_ARRAY=()
      RESULT_ARRAY=()
      BASE_FILE_INDEX=
      for (( INDEX=0; INDEX <= $(( $LENGTH - 1)); INDEX++ ))
      do
        INPUT_FILE=${FILE_PATH}/${FILE_ARRAY[$INDEX]}
        INPUT_ARRAY+=("${INPUT_FILE}")
        ATTRIBUTE_FILE_ARRAY+=("${ATTRIBUTE_FILE}")
        ATTRIBUTE_TRANSFER_OUTPUT_FILE_NAME=attribute_transfer_${FILE_ARRAY[$INDEX]}
        ATTRIBUTE_TRANSFER_OUTPUT_FILE=${ATTRIBUTE_TRANSFER_OUTPUT_PATH}/${ATTRIBUTE_TRANSFER_OUTPUT_FILE_NAME}
        RESULT_ARRAY+=("${ATTRIBUTE_TRANSFER_OUTPUT_FILE}")
      done
    
      echo "Transferring attributes to $LENGTH maps in parallel..."
      parallel $PARALLEL_DEBUG --xapply hoot conflate ${QUIET} ${HOOT_ATTRIBUTE_TRANSFER_OPTS} {1} {2} {3} ::: ${INPUT_ARRAY[@]} ::: ${ATTRIBUTE_FILE_ARRAY[@]} ::: ${RESULT_ARRAY[@]}
    fi
    
    # Update the conflate inputs to be the output files that were a result of the attribute transfer operation.
    FILE_LIST=`ls $LIST_OPTIONS $ATTRIBUTE_TRANSFER_OUTPUT_PATH | grep .osm | awk '{print $9}'`
    FILE_ARRAY=()
    for FILE in $FILE_LIST; do
      FILE_ARRAY+=($FILE)
    done
    LENGTH=${#FILE_ARRAY[@]}
    
  fi
fi

# CONFLATION

if [ $AVERAGE_MERGING == "no" ]
then
  CONFLATION_CONF=ReferenceConflation.conf
else
  CONFLATION_CONF=AverageConflation.conf
fi

HOOT_CONFLATE_OPTS="-C ${ALGORITHM_CONF} -C ${CONFLATION_CONF}"

# conflate ops we don't need
HOOT_CONFLATE_OPTS+=" -D conflate.post.ops-=hoot::RoadCrossingPolyReviewMarker"
HOOT_CONFLATE_OPTS+=" -D conflate.post.ops-=hoot::AddHilbertReviewSortOrderOp"

HOOT_CONFLATE_OPTS+=" -D writer.sort.tags.imagery.source=true"
HOOT_CONFLATE_OPTS+=$RESOLVE_REVIEWS
HOOT_CONFLATE_OPTS+=$REMOVE_DISCONNECTED
HOOT_CONFLATE_OPTS+=$SNAP_UNCONNECTED
HOOT_CONFLATE_OPTS+=$REMOVE_UNLIKELY
HOOT_CONFLATE_OPTS+=$REMOVE_MERGE_RELATIONS
if [ $SUPRESS_DIVIDED_ROADS == "yes" ]
then
  # Use these opts to mark all div roads and roads crossing div roads in both inputs and remove them from the secondary 
  # input only. This assures us that the only divided roads left in the final conflated output will be those from the 
  # starting input. Its strange to have two version of these opts here, but the parallel call complains if the hoot list
  # opts are quoted one way and the non-parallel call complains when they're quoted the other way.
  if [ $PARALLEL == "no" ]
  then
    HOOT_CONFLATE_OPTS+=" -D map.cleaner.transforms+=hoot::DualHighwayMarker;hoot::RemoveElementsVisitor -D remove.elements.visitor.element.criteria=hoot::TagCriterion;hoot::StatusCriterion -D remove.elements.visitor.recursive=true -D remove.elements.visitor.chain.element.criteria=true -D tag.criterion.kvps=hoot:dual_highway=yes;hoot:dual_highway_crossing=yes -D status.criterion.status=Unknown2"
  else
    HOOT_CONFLATE_OPTS+=' -D map.cleaner.transforms+="hoot::DualHighwayMarker;hoot::RemoveElementsVisitor" -D remove.elements.visitor.element.criteria="hoot::TagCriterion;hoot::StatusCriterion" -D remove.elements.visitor.recursive=true -D remove.elements.visitor.chain.element.criteria=true -D tag.criterion.kvps="hoot:dual_highway=yes;hoot:dual_highway_crossing=yes" -D status.criterion.status=Unknown2'
  fi
fi
HOOT_CONFLATE_OPTS+=$AOI_OPTS

# Clean out previously conflated files.
rm -f ${OUTPUT_PATH}/conflation_*

if [ $PARALLEL == "no" ]
then
  INDEX=0
  INPUT_FILE_PATH=
  if [ $SUPRESS_DIVIDED_ROADS == "no" ]
  then
    INPUT_FILE_PATH=$FILE_PATH
  else
    # Make sure we use the correct input file if we transferred attributes beforehand.
    INPUT_FILE_PATH=$ATTRIBUTE_TRANSFER_OUTPUT_PATH
  fi
  
  # Conflate the first pair of inputs.
  
  echo "-----------------------------------------------------------------------------------------------------------------------------------------"
  echo "Conflate ($(( $INDEX + 1 ))/$(( $LENGTH - 1 ))): ${FILE_ARRAY[$(( $START_POSITION - 1 ))]} with ${FILE_ARRAY[$START_POSITION]} to make conflation_$(( $START_POSITION )).osm"
  echo "-----------------------------------------------------------------------------------------------------------------------------------------"
  hoot conflate ${QUIET} ${HOOT_CONFLATE_OPTS} \
    ${INPUT_FILE_PATH}/${FILE_ARRAY[$(( $START_POSITION - 1 ))]} \
    ${INPUT_FILE_PATH}/${FILE_ARRAY[$START_POSITION]} \
    ${OUTPUT_PATH}/conflation_$(( $START_POSITION )).osm

  # Now cumulatively conflate each additional input as secondary against the previously conflated file as reference.

  for (( INDEX=$(( $START_POSITION + 1 )); INDEX<=$(( $LENGTH - 1 )); INDEX++ ))
  do
    echo "Conflate ($(( $INDEX ))/$(( $LENGTH - 1 ))): conflation_$(( $INDEX - 1 )).osm with ${FILE_ARRAY[$INDEX]} to make conflation_${INDEX}.osm"
    echo "-----------------------------------------------------------------------------------------------------------------------------------------"
    hoot conflate ${QUIET} ${HOOT_CONFLATE_OPTS} \
      ${OUTPUT_PATH}/conflation_$(( $INDEX - 1 )).osm \
      ${INPUT_FILE_PATH}/${FILE_ARRAY[$INDEX]} \
      ${OUTPUT_PATH}/conflation_${INDEX}.osm
  done
  # The final output is the last conflated file.
  CONVERT_SRC="${OUTPUT_PATH}/conflation_$(( $LENGTH - 1 )).osm"
else

  echo "Conflating $LENGTH maps in parallel..."

  CONFLATION=1
  
  # The ordering of conflate jobs here works with the divided road suppression when the start position is the first file 
  # (reverse the inputs if you want to start with the last), since that guarantees the first file and its subsequent outputs 
  # are always the reference input for each conflate job. This currently won't work with divided road suppression if any 
  # other position in the list is selected as the start position and additional work would need to be done if that ends up 
  # being required.

  RESULT_ARRAY=()
  for (( INDEX=$(( $START_POSITION - 1)); INDEX <= $(( $LENGTH - 1)); INDEX++ ))
  do
    INPUT_FILE_DIR=
    if [ $SUPRESS_DIVIDED_ROADS == "no" ]
    then
      INPUT_FILE_DIR=$FILE_PATH
    else
      # Make sure we use the correct input file if we transferred attributes beforehand.
      INPUT_FILE_DIR=$ATTRIBUTE_TRANSFER_OUTPUT_PATH
    fi
    RESULT_ARRAY+=("$INPUT_FILE_DIR/${FILE_ARRAY[$INDEX]}")
  done

  while [ ${#RESULT_ARRAY[@]} -gt 1 ]
  do
    FILE_ARRAY_1=()
    FILE_ARRAY_2=()
    SOURCE_ARRAY=("${RESULT_ARRAY[@]}")
    RESULT_ARRAY=()
    # Split the result array in half.
    for (( INDEX=0; INDEX<$(( ${#SOURCE_ARRAY[@]} - 1 )); INDEX+=2 ))
    do
      FILE_ARRAY_1+=(${SOURCE_ARRAY[$INDEX]})
      FILE_ARRAY_2+=(${SOURCE_ARRAY[$(( $INDEX + 1 ))]})
      RESULT_ARRAY+=(${OUTPUT_PATH}/conflation_${CONFLATION}.osm)
      CONFLATION=$(( $CONFLATION + 1))
    done

    # Run hoot conflate in parallel with older file as the reference dataset with the newer as the secondary.
    parallel $PARALLEL_DEBUG --xapply hoot conflate ${QUIET} ${HOOT_CONFLATE_OPTS} {1} {2} {3} ::: ${FILE_ARRAY_1[@]} ::: ${FILE_ARRAY_2[@]} ::: ${RESULT_ARRAY[@]}

    # After the parallel call is made, add in the "extra" file that wasn't conflated.
    if [ $(( ${#SOURCE_ARRAY[@]} % 2 ))  == 1 ]
    then
      RESULT_ARRAY+=(${SOURCE_ARRAY[$(( ${#SOURCE_ARRAY[@]} - 1 ))]})
    fi

  done

  CONVERT_SRC=${RESULT_ARRAY[0]}
fi

# DATA SUMMARIZATION AND CLEANUP

DATA_SUMMARY_OPS=
if [ $SUPRESS_DIVIDED_ROADS == "yes" ]
then
  # These opts add a tag filter that runs before data summarization and keeps only tags that were in the initial input files 
  # and any added metadata tags (for debugging purposes) so that we don't end up adding any new tags as a result of the
  # previously run attribute transfer. It also sets all 'highway' tags back to 'road', which is what we started with, since they
  # may have been changed by attribute transfer.
  DATA_SUMMARY_OPS=" -D convert.ops=hoot::KeepTagsVisitor;hoot::SetTagValueVisitor;hoot::DataSummaryTagVisitor -D tag.filter.keys=$INPUT_TAG_KEYS;hoot:* -D set.tag.value.visitor.keys=highway -D set.tag.value.visitor.values=road -D set.tag.value.visitor.element.criteria=hoot::HighwayCriterion -D data.summary.tag.sources=${TAG_SOURCES}"
  echo "Cleaning and summarizing data output..."
else
  # default data summary ops
  DATA_SUMMARY_OPS=" -D convert.ops=hoot::DataSummaryTagVisitor -D data.summary.tag.sources=${TAG_SOURCES}"
  echo "Summarizing data output..."
fi
hoot convert ${QUIET} ${DATA_SUMMARY_OPS} ${CONVERT_SRC} ${OUTPUT_PATH}/results.osm

