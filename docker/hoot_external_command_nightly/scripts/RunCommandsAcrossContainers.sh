#!/bin/bash
# This file is part of Hootenanny
# This file is responsible for executing hoot commands across containers

# Commands can be added here
# TODO: create functions for different commands when they are added and add flag to specify which command to run
hoot conflate -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D conflate.match.only=true -D writer.include.conflate.score.tags=true -D match.creators="BuildingMatchCreator;HighwayMatchCreator" -D conflate.use.data.source.ids.1=false -D writer.include.debug.tags=true -D uuid.helper.repeatable=true -D conflate.post.ops-="WayJoinerOp" -D id.generator=PositiveIDGenerator -D building.match.threshold=$4 -D building.review.threshold=$5 $1 $2 $3
