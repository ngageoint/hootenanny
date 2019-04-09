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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm.user;

import static hoot.services.models.db.QUsers.users;
import static hoot.services.utils.DbUtils.createQuery;

import java.sql.Timestamp;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.dom.DOMSource;

import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.controllers.osm.OsmResponseHeaderGenerator;
import hoot.services.models.db.QUsers;
import hoot.services.models.db.Users;
import hoot.services.models.osm.User;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Service endpoint for OSM user information
 */
@Controller
@Path("/user")
@Transactional
public class UserResource {
    public UserResource() {
    }

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response get(@Context HttpServletRequest request) {
         Users user = Users.fromRequest(request);
         return Response.ok().entity(user).build();
    }

    /**
     * Service method endpoint for retrieving OSM user information
     * <p>
     * This is currently implemented as a dummy method to appease iD. It always
     * retrieves information for the first user record in the services database.
     * It cannot properly be implemented until user authentication is first
     * implemented.
     * <p>
     * GET hoot-services/osm/api/0.6/user/details
     *
     * @param userId ID of the user to retrieve information for
     * @return Response with the requested user's information
     */
    @GET
    @Path("/{userId}")
    public Response get(@Context HttpServletRequest request, @PathParam("userId") Long userId) throws ParserConfigurationException {
        Users user = createQuery()
                .select(users)
                .from(users)
                .where(users.id.eq(userId))
                .fetchOne();

        if (user == null) {
            return Response.status(Status.NOT_FOUND).build();
        }
        String contentType = null;
        if(request != null) { contentType = request.getHeader("Content-Type"); }
        if(contentType == null || contentType.trim().equalsIgnoreCase("application/xml")) {
            Document responseDoc = writeResponse(new User(user));
            return Response.ok().entity(new DOMSource(responseDoc)).type(MediaType.APPLICATION_XML).build();
        } else {
            return Response.ok().entity(user).type(MediaType.APPLICATION_JSON).build();
        }
    }

    @GET
    @Path("/name/{displayName}")
    public Response getByDisplayName(@Context HttpServletRequest request, @PathParam("displayName") String displayName) throws ParserConfigurationException {
        Users user = createQuery()
                .select(users)
                .from(users)
                .where(users.displayName.equalsIgnoreCase(displayName))
                .fetchOne();

        if (user == null) {
            return Response.status(Status.NOT_FOUND).build();
        }
        String contentType = null;
        if(request != null) { contentType = request.getHeader("Content-Type"); }
        if(contentType == null || contentType.trim().equalsIgnoreCase("application/xml")) {
            Document responseDoc = writeResponse(new User(user));
            return Response.ok().entity(new DOMSource(responseDoc)).type(MediaType.APPLICATION_XML).build();
        } else {
            return Response.ok().entity(user).type(MediaType.APPLICATION_JSON).build();
        }
    }

    /**
     * Service method endpoint for retrieving OSM user information. This rest
     * end point retrieves user based on user email. If it does not exist then it creates first.
     *
     * @deprecated
     * we no longer want to create users by email -or- on-demand
     *
     * @param userEmail User email to save/get
     * @return Response with the requested user's information
     */
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    @Deprecated
    public Response getSaveUser(@Context HttpServletRequest request, @QueryParam("userEmail") String userEmail) {
        Users user;
        try {
            user = getOrSaveByEmail(userEmail);
            return Response.ok().entity(user).build();
        }
        catch (Exception e) {
            String msg = "Error saving user: " + " (" + userEmail + ")";
            return Response.status(Status.INTERNAL_SERVER_ERROR).type(MediaType.TEXT_PLAIN).entity(msg).build();
        }
    }

    /**
     * This rest end point retrieves all users based on user email.
     * <p>
     * GET hoot-services/osm/user/1/all
     *
     * @return JSONArray Object containing users detail
     */
    @GET
    @Path("/all")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAllUsers() {
        List<Users> users;
        try {
            users = retrieveAllUsers();
            return Response.ok().entity(users).build();
        }
        catch (Exception e) {
            return Response.status(Status.INTERNAL_SERVER_ERROR)
                    .type(MediaType.TEXT_PLAIN)
                    .entity("failed to list users")
                    .build();
        }


    }

    private static Document writeResponse(User user) throws ParserConfigurationException {
        Document responseDoc = XmlDocumentBuilder.create();
        Element osmElement = OsmResponseHeaderGenerator.getOsmHeader(responseDoc);
        Element userElement = user.toXml(osmElement, /* user.numChangesetsModified() */ -1);
        osmElement.appendChild(userElement);
        responseDoc.appendChild(osmElement);

        return responseDoc;
    }

    private static List<Users> retrieveAllUsers() {
        return createQuery().select(QUsers.users).from(QUsers.users).orderBy(QUsers.users.displayName.asc()).fetch();
    }

    private static Users getOrSaveByEmail(String userEmail) {
        Users users = createQuery()
                .select(QUsers.users)
                .from(QUsers.users)
                .where(QUsers.users.email.equalsIgnoreCase(userEmail))
                .fetchOne();

        // none then create
        if (users == null) {
            long rowCount = createQuery().insert(QUsers.users)
                    .columns(QUsers.users.email, QUsers.users.displayName)
                    .values(userEmail, userEmail)
                    .execute();

            if (rowCount > 0) {
                users = createQuery()
                        .select(QUsers.users)
                        .from(QUsers.users)
                        .where(QUsers.users.email.equalsIgnoreCase(userEmail))
                        .fetchOne();
            }
        }

        return users;
    }
}
