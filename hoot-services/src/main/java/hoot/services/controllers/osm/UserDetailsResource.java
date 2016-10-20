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

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.utils.DbUtils;


/**
 * Service endpoint for authenticated OSM user detail information
 */
@Controller
@Path("api/0.6/user/details")
@Transactional(readOnly = true)
public class UserDetailsResource {
    private static final Logger logger = LoggerFactory.getLogger(UserDetailsResource.class);

    public UserDetailsResource() {}

    /**
     * Service method endpoint for retrieving OSM user detail information for
     * the authenticated user associated with the request
     * 
     * The Hootenanny user services implements the methods of the OSM User
     * service v0.6 and OSM User Details service v0.6. Hootenanny has no
     * authentication mechanism implemented yet. This service exists for now
     * only to avoid modifications to the iD editor. In the future the
     * Hootenanny OSM services will likely support OAuth 2.0 and this service
     * will have purpose. The service methods first attempt to parse the request
     * user identification data as a numerical user ID, and then, if
     * unsuccessful, attempts to parse it as a user name string.
     * 
     * GET hoot-services/osm/user/test
     *
     * @return XML response with user detail information
     *
     * //TODO: update to get actual logged in user once security is implemented
     */
    @GET
    @Produces(MediaType.TEXT_XML)
    public Response getDetails() {
        // For now, we're just grabbing the first user in the db, since we don't
        // have any authentication in place to get the correct user. Worst case, for now, you see
        // incorrect user information from iD editor...not a big deal since authentication doesn't exist
        // anyway. When hoot gets user authentication, then this obviously has to be updated.
        long userId = DbUtils.getTestUserId();

        return (new UserResource()).get(String.valueOf(userId));
    }
}
