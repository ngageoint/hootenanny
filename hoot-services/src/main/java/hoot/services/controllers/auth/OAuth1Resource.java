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
 * @copyright Copyright (C) 2018, 2020, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.auth;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.parsers.ParserConfigurationException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.security.oauth.consumer.OAuthConsumerSupport;
import org.springframework.security.oauth.consumer.OAuthConsumerToken;
import org.springframework.security.oauth.consumer.OAuthRequestFailedException;
import org.springframework.security.oauth.consumer.OAuthSecurityContext;
import org.springframework.security.oauth.consumer.OAuthSecurityContextHolder;
import org.springframework.security.oauth.consumer.ProtectedResourceDetails;
import org.springframework.security.oauth.consumer.client.OAuthRestTemplate;
import org.springframework.security.oauth.consumer.token.OAuthConsumerTokenServices;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.xml.sax.SAXException;

import hoot.services.HootProperties;
import hoot.services.models.db.Users;

@Controller
@Path("")
@Transactional
public class OAuth1Resource {
    private static final Logger logger = LoggerFactory.getLogger(OAuth1Resource.class);
    @Autowired
    private OAuthRestTemplate oauthRestTemplate;
    @Autowired
    private OAuthConsumerTokenServices tokenServices;
    @Autowired
    private UserManager userManager;


    @GET
    @Path("/oauth1/verify")
    @Produces(MediaType.APPLICATION_JSON)
    public Response verify(@QueryParam("oauth_token") String oauth_token, @QueryParam("oauth_verifier") String oauth_verifier) {
        OAuthConsumerSupport s = oauthRestTemplate.getSupport();
        ProtectedResourceDetails r = oauthRestTemplate.getResource();
        HttpSession sess = getSession();
        if (sess == null) {
            return Response.status(401).build();
        }
        sess.setMaxInactiveInterval(31536000);
        OAuthConsumerToken requestToken = tokenServices.getToken(r.getId());
        if (requestToken == null) {
            return Response.status(401).build();
        }
        OAuthConsumerToken accessToken = null;
        try {
            accessToken = s.getAccessToken(r /* <== Specifying this was important [!] */, requestToken, oauth_verifier);
        } catch (Exception e) {
            logger.warn("Failed to get access token for user w/ exception:", e);
        }
        if (accessToken == null) {
            logger.warn("Failed to get access token for user; no details");
            return Response.status(401).build();
        }
        // Save this access token to our HTTP Session based token services:
        tokenServices.storeToken(r.getId(), accessToken);
        // Save this access token to our security context, used by
        // OAuthClientHttpRequestFactory to authorize
        // outbound requests.
        OAuthSecurityContext context = OAuthSecurityContextHolder.getContext();
        context.getAccessTokens().put(r.getId(), accessToken);

        HttpHeaders headers = new HttpHeaders();
        headers.set("Accept", "application/xml");
        HttpEntity<String> entity = new HttpEntity<>(headers);

        ResponseEntity<String> responseEntity = oauthRestTemplate.exchange(
                HootProperties.OAUTH_PROVIDERURL + HootProperties.OAUTH_PATHS_USER, HttpMethod.GET, entity, String.class);
        String response = responseEntity.getBody();
        Users user;
        try {
            user = userManager.upsert(response, accessToken, sess.getId());
        } catch (InvalidUserProfileException | SAXException | IOException | ParserConfigurationException e) {
            logger.error("Failed to read user profile from oauth provider", e);
            return Response.status(502).build();
        } catch (Exception e) {
            logger.error("Failed to store user during oauth verification", e);
            return Response.status(500).build();
        }

        return Response.status(200).entity(user).type(MediaType.APPLICATION_JSON).build();
    }

    @GET
    @Path("/oauth1/request")
    @Produces(MediaType.TEXT_PLAIN)
    public Response request() {
        OAuthConsumerSupport s = oauthRestTemplate.getSupport();
        ProtectedResourceDetails r = oauthRestTemplate.getResource();
        OAuthConsumerToken requestToken = null;
        try {
            requestToken = s.getUnauthorizedRequestToken(r, HootProperties.OAUTH_REDIRECTURL);
        } catch (OAuthRequestFailedException e) {
            logger.error("Failed to obtain request token", e);
            return Response.status(502).build();
        } catch (Exception e) {
            logger.error("Failed to execute request for token.", e);
            return Response.status(500).build();
        }

        tokenServices.storeToken(r.getId(), requestToken);
        return Response.ok().entity(r.getUserAuthorizationURL() + "?oauth_token=" + requestToken.getValue()).type(MediaType.TEXT_PLAIN).build();
    }
    @GET
    @Path("/oauth1/logout")
    @Produces(MediaType.TEXT_PLAIN)
    public Response logout() {

        ProtectedResourceDetails r = oauthRestTemplate.getResource();
        tokenServices.removeToken(r.getId());

        OAuthSecurityContext context = OAuthSecurityContextHolder.getContext();
        context.getAccessTokens().remove(r.getId());

        // Invalidate HTTP Session
        HttpSession sess = getSession();
        sess.invalidate();

        return Response.ok().build();
    }

    protected HttpSession getSession() {
        OAuthSecurityContext context = OAuthSecurityContextHolder.getContext();
        if (context == null) {
            throw new IllegalStateException("A security context must be established.");
        }

        HttpServletRequest request;
        try {
            request = (HttpServletRequest) context.getDetails();
        } catch (ClassCastException e) {
            throw new IllegalStateException("The security context must have the HTTP servlet request as its details.");
        }

        if (request == null) {
            throw new IllegalStateException("The security context must have the HTTP servlet request as its details.");
        }

        HttpSession session = request.getSession(true);
        if (session == null) {
            throw new IllegalStateException("Unable to create a session in which to store the tokens.");
        }

        return session;
    }

}
