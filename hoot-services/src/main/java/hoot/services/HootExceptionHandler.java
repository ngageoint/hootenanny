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

            // Rebuild the response if needed:
            if (r.getLength() < 1) {
                return Response.status(r.getStatus()).entity(e.getMessage()).type(MediaType.TEXT_PLAIN).build();
            }
            else {
                return r;
            }
        }
        return Response.status(Status.INTERNAL_SERVER_ERROR).entity("").build();
	}

}
