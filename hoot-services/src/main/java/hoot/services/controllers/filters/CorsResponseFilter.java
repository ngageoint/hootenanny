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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.filters;

import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import com.sun.jersey.spi.container.ContainerRequest;
import com.sun.jersey.spi.container.ContainerResponse;
import com.sun.jersey.spi.container.ContainerResponseFilter;


/**
 * Jersey filter for supporting CORS:
 * http://en.wikipedia.org/wiki/Cross-origin_resource_sharing
 */
public class CorsResponseFilter implements ContainerResponseFilter {
    /**
     * Returns a CORS aware filter
     */
    @Override
    public ContainerResponse filter(ContainerRequest request, ContainerResponse response) {
        ResponseBuilder resp = Response.fromResponse(response.getResponse());
        resp.header("Access-Control-Allow-Origin", "*")
            .header("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS");

        String reqHead = request.getHeaderValue("Access-Control-Request-Headers");
        if ((reqHead != null) && !reqHead.isEmpty()) {
            resp.header("Access-Control-Allow-Headers", reqHead);
        }

        response.setResponse(resp.build());
        return response;
    }
}
