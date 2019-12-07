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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm.user;

import static hoot.services.models.db.QUsers.users;
import static hoot.services.utils.DbUtils.createQuery;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import javax.servlet.http.HttpServletRequest;
import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
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

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import com.querydsl.core.Tuple;
import com.querydsl.core.types.OrderSpecifier;

import hoot.services.controllers.auth.UserManager;
import hoot.services.controllers.osm.OsmResponseHeaderGenerator;
import hoot.services.models.db.QUsers;
import hoot.services.models.db.Users;
import hoot.services.models.osm.User;
import hoot.services.utils.PostgresUtils;
import hoot.services.utils.XmlDocumentBuilder;


/**
 * Service endpoint for OSM user information
 */
@Controller
@Path("/api/0.6/user")
@Transactional
public class UserResource {
    private static final Logger logger = LoggerFactory.getLogger(UserResource.class);

    @Autowired
    UserManager userManager;

    public UserResource() {
    }

    @GET
    @Produces(MediaType.APPLICATION_JSON)
    public Response get(@Context HttpServletRequest request) {
         Users user = Users.fromRequest(request);
         return Response.ok().entity(user).build();
    }

    private Response userResponse(HttpServletRequest request, Users user) throws ParserConfigurationException {

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
     * Service method endpoint for retrieving Hoot user information by id
     *
     * GET hoot-services/osm/api/0.6/user/{userId}
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

        return userResponse(request, user);
    }

    /**
     * Service method endpoint for retrieving Hoot user
     *
     * GET hoot-services/osm/api/0.6/user/details
     *
     * @return Response with the user's information
     */
    @GET
    @Path("/details")
    public Response getDetails(@Context HttpServletRequest request) throws ParserConfigurationException {
        Users user = Users.fromRequest(request);

        return userResponse(request, user);
    }

    @GET
    @Path("/name/{displayName}")
    public Response getByDisplayName(@Context HttpServletRequest request, @PathParam("displayName") String displayName) throws ParserConfigurationException {
        Users user = createQuery()
                .select(users)
                .from(users)
                .where(users.displayName.equalsIgnoreCase(displayName))
                .fetchOne();

        return userResponse(request, user);
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
    public Response getSaveUser(@QueryParam("userEmail") String userEmail) {
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
     * GET hoot-services/osm/user/all
     *
     * @return JSONArray Object containing users detail
     */
    @GET
    @Path("/all")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAllUsers(@Context HttpServletRequest request,
            @QueryParam("sort") @DefaultValue("") String sort,
            @QueryParam("privileges") @DefaultValue("") String privileges) {
        Users currentUser = Users.fromRequest(request);

        try {
            List<Tuple> userInfo;
            OrderSpecifier<?> sorter;
            Collection<String> activePrivileges = new ArrayList<>();

            switch (sort) {
                case "-auth":
                    sorter = users.hootservices_last_authorize.desc();
                    break;
                case "+auth":
                    sorter = users.hootservices_last_authorize.asc();
                    break;
                case "-name":
                    sorter = users.displayName.desc();
                    break;
                case "+name":
                default:
                    sorter = users.displayName.asc();
                    break;
            }

            // Run the proper query to retrieve user data based on the request users privileges
            // Admin user gets extra info on other users
            if (adminUserCheck(currentUser)) {
                if (!privileges.isEmpty()) {
                    activePrivileges = Arrays.stream(privileges.split(","))
                            .collect(Collectors.toList());
                }

                userInfo = createQuery()
                        .select(users.id, users.displayName, users.hootservices_last_authorize, users.privileges)
                        .from(users)
                        .orderBy(sorter)
                        .fetch();
            } else {
                userInfo = createQuery()
                        .select(users.id, users.displayName)
                        .from(users)
                        .orderBy(sorter)
                        .fetch();
            }

            List<Users> userList = new LinkedList<>();

            for (Tuple tuple : userInfo) {
                Users user = new Users();

                if (adminUserCheck(currentUser)) {
                    Map<String, String> substitutionMap = (Map<String, String>) tuple.get(users.privileges);
                    Collection<String> filterPrivileges = substitutionMap.keySet()
                            .stream().filter(map -> substitutionMap.get(map).equals("true"))
                            .collect(Collectors.toSet());

                    if (activePrivileges.size() == 0 || filterPrivileges.containsAll(activePrivileges)) {
                        user.setId(tuple.get(users.id));
                        user.setDisplayName(tuple.get(users.displayName));
                        user.setHootservicesLastAuthorize(tuple.get(users.hootservices_last_authorize));
                        user.setPrivileges(tuple.get(users.privileges));
                        userList.add(user);
                    }

                } else {
                    user.setId(tuple.get(users.id));
                    user.setDisplayName(tuple.get(users.displayName));
                    userList.add(user);
                }
            }

            return Response.ok().entity(userList).build();
        }
        catch (Exception e) {
            logger.error("Failed to get all users", e);

            return Response.status(Status.INTERNAL_SERVER_ERROR)
                    .type(MediaType.TEXT_PLAIN)
                    .entity("failed to list users")
                    .build();
        }
    }

    /**
     *
     * Saves the privileges for the specified users list
     *
     * POST hoot-services/osm/api/0.6/user/savePrivileges
     *
     * @param request
     * @param userList list of objects containing the users id and privileges
     *  looks like:
     *      [
     *        { id:1, privileges: { admin: false, advanced: true } },
     *        { id:2, privileges: { admin: true, advanced: true } }
     *      ]
     * @return success status if everything is updated.
     *      forbidden status if the user trying to save the privileges isn't an admin
     */
    @POST
    @Path("/savePrivileges")
    @Consumes(MediaType.APPLICATION_JSON)
    public Response savePrivileges(@Context HttpServletRequest request,
            List<LinkedHashMap> userList) {
        Users currentUser = Users.fromRequest(request);

        if (!adminUserCheck(currentUser)) {
            return Response.status(Status.FORBIDDEN).type(MediaType.TEXT_PLAIN).entity("You do not have access to save privileges").build();
        }

        for (LinkedHashMap user : userList) {
            Long userId = Long.valueOf(user.get("id").toString());

            createQuery().update(users)
                .where(users.id.eq(userId))
                .set(users.privileges, user.get("privileges"))
                .execute();

            userManager.clearCachedUser(userId);

        }

        return Response.ok().build();
    }

    /**
     * Gets the current users privileges
     *
     * GET hoot-services/osm/api/0.6/user/getPrivileges
     *
     * @param request
     * @return the current users privileges
     */
    @GET
    @Path("/getPrivileges")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getPrivileges(@Context HttpServletRequest request) {
        Users user = Users.fromRequest(request);
        Map<String, String> json = PostgresUtils.postgresObjToHStore(user.getPrivileges());

        return Response.ok(json).build();
    }

    /**
     * Checks if the specified user is an admin user
     *
     * @param user
     * @return true if user has admin privileges, else false
     */
    private static boolean adminUserCheck(Users user) {
        Map<String, String> privileges = PostgresUtils.postgresObjToHStore(user.getPrivileges());
        return privileges != null && ("true").equals(privileges.get("admin"));
    }

    private static Document writeResponse(User user) throws ParserConfigurationException {
        Document responseDoc = XmlDocumentBuilder.create();
        Element osmElement = OsmResponseHeaderGenerator.getOsmHeader(responseDoc);
        Element userElement = user.toXml(osmElement, /* user.numChangesetsModified() */ -1);
        osmElement.appendChild(userElement);
        responseDoc.appendChild(osmElement);

        return responseDoc;
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
