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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
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

import java.util.Arrays;

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
        logger.error("oauth_token: " + oauth_token);
        logger.error("oauth_verifier: " + oauth_verifier);
        OAuthConsumerSupport s = oauthRestTemplate.getSupport();
        ProtectedResourceDetails r = oauthRestTemplate.getResource();
        logger.error("ProtectedResourceDetails:");
        logger.error("id: " + r.getId());
        logger.error("access token url: " + r.getAccessTokenURL());
        logger.error("additional params: " + r.getAdditionalParameters());
        logger.error("additional request headers: " + r.getAdditionalRequestHeaders());
        logger.error("auth header realm: " + r.getAuthorizationHeaderRealm());
        logger.error("consumer key: " + r.getConsumerKey());
        logger.error("request token url: " + r.getRequestTokenURL());
        logger.error("user auth url: " + r.getUserAuthorizationURL());
        HttpSession sess = getSession();
        if (sess == null) {
            return Response.status(401).entity("No session specified.").build();
        }
        sess.setMaxInactiveInterval(31536000);
        //logger.error("Session: " + Arrays.toString(sess.getValueNames()));
        for (String valName : sess.getValueNames())
        {
          logger.error("key: " + valName + ", val: " + sess.getValue(valName));
        }
        OAuthConsumerToken requestToken = tokenServices.getToken(r.getId());
        if (requestToken == null) {
            logger.error("requestToken null");
            return Response.status(401).entity("Unable to retrieve request token.").build();
        }
        OAuthConsumerToken accessToken = null;
        try {
            accessToken = s.getAccessToken(r /* <== Specifying this was important [!] */, requestToken, oauth_verifier);
        } catch (Exception e) {
            // Pass.
            logger.error(e.getMessage());
        }
        if (accessToken == null) {
            logger.error("accessToken null");
            return Response.status(401).entity("Unable to retrieve access token.").build();
        }
        logger.error("accessToken:");
        logger.error("resource id: " + accessToken.getResourceId());
        logger.error("secret: " + accessToken.getSecret());
        logger.error("value: " + accessToken.getValue());
        logger.error("isAccessToken: " + accessToken.isAccessToken());
        logger.error("additional params: " + accessToken.getAdditionalParameters());
        // Save this access token to our HTTP Session based token services:
        tokenServices.storeToken(r.getId(), accessToken);
        // Save this access token to our security context, used by
        // OAuthClientHttpRequestFactory to authorize
        // outbound requests.
        OAuthSecurityContext context = OAuthSecurityContextHolder.getContext();
        context.getAccessTokens().put(r.getId(), accessToken);
        logger.error("security context:");
        logger.error("access tokens: " + context.getAccessTokens());
        logger.error("details: " + context.getDetails());

        String response = oauthRestTemplate
                .getForObject(HootProperties.OAUTH_PROVIDERURL + HootProperties.OAUTH_PATHS_USER, String.class);
        logger.error("response: " + response);
        Users user;
        try {
            user = userManager.upsert(response, accessToken, sess.getId());
        } catch (InvalidUserProfileException | SAXException | IOException | ParserConfigurationException e) {
            logger.error("Failed to read user profile from oauth provider", e);
            return Response.status(502).entity("Failed to read user profile from oauth provider.").build();
        } catch (Exception e) {
            logger.error("Failed to store user during oauth verification", e);
            return Response.status(500).entity("Failed to store user during oauth verification.").build();
        }
        logger.error("user:");
        logger.error("email: " + user.getEmail());
        logger.error("display name: " + user.getDisplayName());
        logger.error("id: " + user.getId());
        logger.error("provider access key: " + user.getProviderAccessKey());
        logger.error("provider access token: " + user.getProviderAccessToken());

        return Response.status(200).entity(user).type(MediaType.APPLICATION_JSON).build();
    }

    @GET
    @Path("/oauth1/request")
    @Produces(MediaType.TEXT_PLAIN)
    public Response request() {
        OAuthConsumerSupport s = oauthRestTemplate.getSupport();
        ProtectedResourceDetails r = oauthRestTemplate.getResource();
        logger.error("ProtectedResourceDetails:");
        logger.error("id: " + r.getId());
        logger.error("access token url: " + r.getAccessTokenURL());
        logger.error("additional params: " + r.getAdditionalParameters());
        logger.error("additional request headers: " + r.getAdditionalRequestHeaders());
        logger.error("auth header realm: " + r.getAuthorizationHeaderRealm());
        logger.error("consumer key: " + r.getConsumerKey());
        logger.error("request token url: " + r.getRequestTokenURL());
        logger.error("user auth url: " + r.getUserAuthorizationURL());
        OAuthConsumerToken requestToken = null;
        try {
            requestToken = s.getUnauthorizedRequestToken(r, HootProperties.OAUTH_REDIRECTURL);
        } catch (OAuthRequestFailedException e) {
            logger.error("Failed to obtain request token", e);
            return Response.status(502).entity("Failed to obtain request token.").build();
        } catch (Exception e) {
            logger.error("Failed to execute request for token.", e);
            return Response.status(500).entity("Failed to execute request for token.").build();
        }
        logger.error("requestToken:");
        logger.error("resource id: " + requestToken.getResourceId());
        logger.error("secret: " + requestToken.getSecret());
        logger.error("value: " + requestToken.getValue());
        logger.error("isAccessToken: " + requestToken.isAccessToken());
        logger.error("additional params: " + requestToken.getAdditionalParameters());

        tokenServices.storeToken(r.getId(), requestToken);
        logger.error("Response: " + r.getUserAuthorizationURL() + "?oauth_token=" + requestToken.getValue());
        return Response.ok().entity(r.getUserAuthorizationURL() + "?oauth_token=" + requestToken.getValue()).type(MediaType.TEXT_PLAIN).build();
    }
    @GET
    @Path("/oauth1/logout")
    @Produces(MediaType.TEXT_PLAIN)
    public Response logout() {

        logger.error("logout");
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
