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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */
package hoot.services;

import javax.ws.rs.Produces;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.ext.ExceptionMapper;
import javax.ws.rs.ext.Provider;


@Provider
public class HootExceptionHandler implements ExceptionMapper<Exception> {
	@Override
	@Produces(MediaType.TEXT_PLAIN)
	public Response toResponse(Exception err) {
        if (err instanceof WebApplicationException) {
            WebApplicationException e = (WebApplicationException) err;
            Response r = e.getResponse();

            // Rebuild the response if needed,
            // but never send back the generic WebApplicationException messages:
            if (r.getLength() < 1 && !e.getMessage().startsWith("HTTP")) {
                return Response.status(r.getStatus()).entity(e.getMessage()).type(MediaType.TEXT_PLAIN).build();
            }
            else {
                return Response.status(r.getStatus()).entity("").type(MediaType.TEXT_PLAIN).build();
            }
        }
        return Response.status(Status.INTERNAL_SERVER_ERROR).entity("").build();
	}

}
