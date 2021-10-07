#!/bin/bash
# This file is part of Hootenanny
# This file is responsible for executing Sherlock hoot commands across containers

export LD_LIBRARY_PATH=/opt/python39/lib:/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.302.b08-0.el7_9.x86_64/jre/lib/amd64/server/
hoot conflate -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D conflate.match.only=true -D writer.include.conflate.score.tags=true -D match.creators="BuildingMatchCreator" -D conflate.use.data.source.ids.1=false -D writer.include.debug.tags=true -D uuid.helper.repeatable=true -D conflate.post.ops-="WayJoinerOp" -D id.generator=PositiveIdGenerator $1 $2 $3