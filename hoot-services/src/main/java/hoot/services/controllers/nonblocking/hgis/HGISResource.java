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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.nonblocking.hgis;

import static hoot.services.models.db.QMaps.maps;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import hoot.services.HootProperties;
import hoot.services.command.ExternalCommand;
import hoot.services.controllers.nonblocking.AsynchronousJobResource;
import hoot.services.models.osm.ModelDaoUtils;


/**
 * Base code for HGIS specific Rest endpoint.
 */
abstract class HGISResource extends AsynchronousJobResource {
    private static final Logger logger = LoggerFactory.getLogger(HGISResource.class);


    public HGISResource(String processName) {
        super(processName);
    }

    ExternalCommand createHGISCommand(String sourceMap, String outputMap) {
        JSONArray commandArgs = new JSONArray();

        JSONObject arg = new JSONObject();
        arg.put("SOURCE", generateDbMapParam(sourceMap));
        commandArgs.add(arg);

        arg = new JSONObject();
        arg.put("OUTPUT", generateDbMapParam(outputMap));
        commandArgs.add(arg);

        return super.createBashScriptJobReq(commandArgs);
    }

    static void checkHGISCommandParams(String sourceMap, String outputMap) {
        if (StringUtils.isBlank(sourceMap)) {
            String msg = "Empty sourceMap!";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        if (StringUtils.isBlank(outputMap)) {
            String msg = "Empty outputMap!";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }

        if (!mapExists(sourceMap)) {
            String msg = "sourceMap [" + sourceMap + "] does not exist.";
            throw new WebApplicationException(Response.status(Response.Status.BAD_REQUEST).entity(msg).build());
        }
    }

    /**
     * Checks for the existence of map
     *
     * @param mapName
     * @return returns true when exists else false
     */
    private static boolean mapExists(String mapName) {
        return (verifyMapExists(mapName) > -1);
    }

    /**
     * Determines whether a maps data has been prepared for review; more or less
     * a wrapper with a more identifiable name around ModelDaoUtils map functionality
     *
     * @param mapIdStr
     *            map ID; may be a map ID or unique map name
     * @return the map's numeric ID
     */
    private static long verifyMapExists(String mapIdStr) {
        // this will throw if it doesn't find the map
        return ModelDaoUtils.getRecordIdForInputString(mapIdStr, maps, maps.id, maps.displayName);
    }

    /**
     * Creates db conection string based on config settings in
     * hoot-services.conf
     *
     * @param mapName
     * @return output looks like: postgresql://hoot:hoottest@localhost:5432/hoot1/BrazilOsmPois
     */
    private static String generateDbMapParam(String mapName) {
        return HootProperties.HOOT_APIDB_URL + "/" + mapName;
    }
}
