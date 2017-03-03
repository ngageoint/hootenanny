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
package hoot.services.controllers.hgis;

import static hoot.services.utils.DbUtils.mapExists;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Base code for HGIS specific Rest endpoint.
 */
abstract class HGISResource {
    private static final Logger logger = LoggerFactory.getLogger(HGISResource.class);

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
}
