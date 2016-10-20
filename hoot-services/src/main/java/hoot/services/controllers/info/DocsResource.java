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
package hoot.services.controllers.info;

import static hoot.services.HootProperties.DOC_NAME;
import static hoot.services.HootProperties.HOME_FOLDER;

import java.io.File;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.ResponseBuilder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;


@Controller
@Path("/document")
public class DocsResource {
    private static final Logger logger = LoggerFactory.getLogger(DocsResource.class);

    public DocsResource() {}

    /**
     * REST end point for user document
     * 
     * @return Octet stream of retrieved file
     */
    @GET
    @Path("/export")
    @Produces(MediaType.APPLICATION_OCTET_STREAM)
    public Response exportDoc() {
        String documentPath = HOME_FOLDER + "/" + "docs" + "/" + DOC_NAME;
        File out = new File(documentPath);

        ResponseBuilder responseBuilder = Response.ok(out, "application/pdf");
        responseBuilder.header("Content-Disposition", "attachment; filename=" + DOC_NAME);

        return responseBuilder.build();
    }
}
