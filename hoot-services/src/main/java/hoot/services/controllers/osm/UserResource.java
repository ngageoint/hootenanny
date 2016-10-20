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
package hoot.services.controllers.osm;

import static hoot.services.models.db.QUsers.users;
import static hoot.services.utils.DbUtils.createQuery;

import java.util.List;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.dom.DOMSource;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import hoot.services.models.db.QUsers;
import hoot.services.models.db.Users;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.models.osm.User;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Service endpoint for OSM user information
 */
@Controller
@Path("/user/{userId}")
@Transactional
public class UserResource {
    private static final Logger logger = LoggerFactory.getLogger(UserResource.class);

    public UserResource() {}

    /**
     * Service method endpoint for retrieving OSM user information
     * 
     * This is currently implemented as a dummy method to appease iD. It always
     * retrieves information for the first user record in the services database.
     * It cannot properly be implemented until user authentication is first
     * implemented.
     * 
     * GET hoot-services/osm/api/0.6/user/details
     *
     * @param userId
     *            ID of the user to retrieve information for
     * @return Response with the requested user's information
     */
    @GET
    @Produces(MediaType.TEXT_XML)
    public Response get(@PathParam("userId") String userId) {
        Document responseDoc;

        try {
            long userIdNum;
            try {
                // input mapId may be a map ID or a map name
                userIdNum = ModelDaoUtils.getRecordIdForInputString(userId, users, users.id, users.displayName);
            }
            catch (Exception e) {
                if (e.getMessage().startsWith("Multiple records exist") ||
                        e.getMessage().startsWith("No record exists")) {
                    String message = e.getMessage().replaceAll("records", "users").replaceAll("record", "user");
                    throw new WebApplicationException(e, Response.status(Status.NOT_FOUND).entity(message).build());
                }
                else {
                    String message = "Error requesting user with ID: " + userId + " (" + e.getMessage() + ")";
                    throw new WebApplicationException(e, Response.status(Status.BAD_REQUEST).entity(message).build());
                }
            }

            // there is only ever one test user
            Users user = createQuery().select(users).from(users).where(users.id.eq(userIdNum)).fetchOne();

            if (user == null) {
                String message = "No user exists with ID: " + userId + ".  Please request a valid user.";
                throw new WebApplicationException(Response.status(Status.NOT_FOUND).entity(message).build());
            }

            responseDoc = writeResponse(new User(user));
        }
        catch (WebApplicationException wae) {
            throw wae;
        }
        catch (Exception e) {
            String message = "Error fetching OSM user data!";
            throw new WebApplicationException(e, Response.serverError().entity(message).build());
        }

        return Response.ok(new DOMSource(responseDoc)).build();
    }

    /**
     * Service method endpoint for retrieving OSM user information. This rest
     * end point retrieves user based on user email. If it does not exist then it creates first.
     * 
     * @param userEmail
     *            User email to save/get
     * @return Response with the requested user's information
     */
    @POST
    @Consumes(MediaType.APPLICATION_JSON)
    @Produces(MediaType.APPLICATION_JSON)
    public UserSaveResponse getSaveUser(@QueryParam("userEmail") String userEmail) {
        Users user;
        try {
            user = getOrSaveByEmail(userEmail);
        }
        catch (Exception e) {
            String msg = "Error saving user: " + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return new UserSaveResponse(user);
    }

    /**
     * This rest end point retrieves all users based on user email.
     * 
     * GET hoot-services/osm/user/1/all
     *
     * @return JSONArray Object containing users detail
     */
    @GET
    @Path("/all")
    @Produces(MediaType.APPLICATION_JSON)
    public UsersGetResponse getAllUsers() {
        List<Users> users;
        try {
            users = retrieveAllUsers();
        }
        catch (Exception e) {
            String msg = "Error getting all users: " + " (" + e.getMessage() + ")";
            throw new WebApplicationException(e, Response.serverError().entity(msg).build());
        }

        return new UsersGetResponse(users);
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
