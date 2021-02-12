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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Used for pushing OSM data to the database
 */
class InvertCropCommand extends GrailCommand {
    private static final Logger logger = LoggerFactory.getLogger(InvertCropCommand.class);

    InvertCropCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        logger.info("Params: " + params);

        List<String> options = new LinkedList<>();
        options.add("convert.ops=hoot::MapCropper;hoot::RemoveElementsVisitor");
        options.add("remove.elements.visitor.element.criteria=hoot::WayCriterion;hoot::RelationCriterion");
        options.add("remove.elements.visitor.recursive=false");
        options.add("crop.bounds=" + params.getBounds());
        options.add("crop.invert=true");

        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("INPUT", params.getInput1());
        substitutionMap.put("OUTPUT", params.getOutput());

        String command = "hoot.bin convert --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT}";

        super.configureCommand(command, substitutionMap, caller);
    }

}
