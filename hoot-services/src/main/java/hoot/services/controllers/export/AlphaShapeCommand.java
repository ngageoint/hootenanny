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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.export;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import hoot.services.geo.BoundingBox;
import hoot.services.models.db.Users;


class AlphaShapeCommand extends ExportCommand {

    AlphaShapeCommand(String jobId, ExportParams params, String debugLevel, Class<?> caller, Users user) {
        super(jobId, params);
        if(user != null) {
            params.setUserEmail(user.getEmail());
        }

        List<String> options = new LinkedList<>();
        if(user == null) {
            options.add("api.db.email=test@test.com");
        } else {
            options.add("api.db.email=" + user.getEmail());
        }

        options.add("writer.precision=7");

        // bounding box is optional for this command; if not specified, the
        // command will calculate for the combined extent of all input datasets which, of course, can
        // be very expensive for large datasets
        if (params.getBounds() != null) {
            BoundingBox bounds = new BoundingBox(params.getBounds());
            options.add("convert.bounding.box=" + bounds.getMinLon() + "," + bounds.getMinLat() + ","
                    + bounds.getMaxLon() + "," + bounds.getMaxLat());
        }
        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("DEBUG_LEVEL", debugLevel);
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        // can specify multiple inputs with ';' delimiter
        substitutionMap.put("INPUTS", super.getInput());
        substitutionMap.put("OUTPUT", super.getOutputPath());

        substitutionMap.put("ALPHA", String.valueOf(params.getAlpha()));
        substitutionMap.put("BUFFER", String.valueOf(params.getBuffer()));

        String command = "hoot.bin generate-alpha-shape --${DEBUG_LEVEL} ${HOOT_OPTIONS} --alpha ${ALPHA} --buffer ${BUFFER} ${INPUTS} ${OUTPUT}";

        super.configureCommand(command, substitutionMap, caller, new File("/tmp"));
    }
}
