package hoot.services;

import javax.ws.rs.Produces;
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
		return Response.status(Status.INTERNAL_SERVER_ERROR).entity("").build();
	}

}
