/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.grail;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;


class RunDiffCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(RunDiffCommand.class);

    RunDiffCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId,params);

        logger.info("Params: " + params);

        // These are all of the non-default options from the UI
        List<String> options = new LinkedList<>();
        options.add("convert.ops=hoot::DecomposeBuildingRelationsVisitor");
        options.add("element.cache.size.node=10000000");
        options.add("element.cache.size.relation=2000000");
        options.add("element.cache.size.way=2000000");
        options.add("map.cleaner.transforms=hoot::ReprojectToPlanarOp;hoot::DuplicateWayRemover;hoot::SuperfluousWayRemover;hoot::IntersectionSplitter;hoot::UnlikelyIntersectionRemover;hoot::DualWaySplitter;hoot::ImpliedDividedMarker;hoot::DuplicateNameRemover;hoot::SmallWayMerger;hoot::RemoveEmptyAreasVisitor;hoot::RemoveDuplicateAreaVisitor;hoot::NoInformationElementRemover;hoot::CornerSplitter;hoot::RubberSheet");
        // options.add("match.creators=hoot::NetworkMatchCreator;hoot::ScriptMatchCreator,Area.js;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::PoiPolygonMatchCreator;hoot::ScriptMatchCreator,PowerLine.js;hoot::ScriptMatchCreator,Railway.js;hoot::ScriptMatchCreator,LinearWaterway.js");
        // options.add("merger.creators=hoot::NetworkMergerCreator;hoot::ScriptMergerCreator;hoot::BuildingMergerCreator;hoot::PoiPolygonMergerCreator");
        options.add("match.creators=hoot::HighwayMatchCreator;hoot::ScriptMatchCreator,Area.js;hoot::BuildingMatchCreator;hoot::ScriptMatchCreator,PoiGeneric.js;hoot::PoiPolygonMatchCreator;hoot::ScriptMatchCreator,PowerLine.js;hoot::ScriptMatchCreator,Railway.js;hoot::ScriptMatchCreator,LinearWaterway.js");
        options.add("merger.creators=hoot::HighwaySnapMergerCreator;hoot::ScriptMergerCreator;hoot::BuildingMergerCreator;hoot::PoiPolygonMergerCreator");
        options.add("reader.conflate.use.data.source.ids.1=true");
        options.add("reader.conflate.use.data.source.ids.2=false");
        options.add("way.subline.matcher=hoot::MaximalSublineMatcher");
        options.add("conflate.pre.ops=hoot::CornerSplitter;");

        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("INPUT1", params.getInput1());
        substitutionMap.put("INPUT2", params.getInput2());
        substitutionMap.put("OUTPUT", params.getOutput());
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);

        String command = "hoot conflate --${DEBUG_LEVEL} -C RemoveReview2Pre.conf ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${OUTPUT} --differential --include-tags --separate-output";

        super.configureCommand(command, substitutionMap, caller);
    }
}
