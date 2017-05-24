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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import static hoot.services.HootProperties.*;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.commons.io.FileUtils;

import hoot.services.command.CommandResult;
import hoot.services.command.ExternalCommand;
import hoot.services.geo.BoundingBox;


class CalculateTilesCommand extends ExportCommand {

    CalculateTilesCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        CalculateTilesParams calcTilesParams = (CalculateTilesParams)params;
        
        List<String> options = new LinkedList<>();
        options.add("id.generator=hoot::PositiveIdGenerator");
        options.add("api.db.email=test@test.com");
        //bounding box is optional for this command; if not specified, the command will calculate
        //for the combined extent of all input datasets which, of course, can be very expensive for
        //large datasets
        if (params.getBounds() != null)
        {
          BoundingBox bounds = new BoundingBox(params.getBounds());
          options.add("convert.bounding.box=" + bounds.getMinLon() + "," + bounds.getMinLat() + "," + bounds.getMaxLon() + "," + bounds.getMaxLat());
        }
        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        //can specify multiple inputs with ';' delimiter
        //substitutionMap.put("INPUTS", super.getInput());
        //substitutionMap.put("OUTPUT", super.getOutputPath());
        substitutionMap.put("MAX_NODE_COUNT_PER_TILE", calcTilesParams.getMaxNodeCountPerTile());
        substitutionMap.put("PIXEL_SIZE", calcTilesParams.getPixelSize());

        String command = "hoot calculate-tiles --${DEBUG_LEVEL} ${HOOT_OPTIONS} ${INPUT} ${OUTPUT_PATH} ${MAX_NODE_COUNT_PER_TILE} ${PIXEL_SIZE}";

        super.configureCommand(command, substitutionMap, caller);
    }
}
