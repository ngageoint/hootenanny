package hoot.services.controllers.auth;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.oauth.consumer.OAuthConsumerSupport;
import org.springframework.security.oauth.consumer.OAuthConsumerToken;
import org.springframework.security.oauth.consumer.OAuthSecurityContext;
import org.springframework.security.oauth.consumer.OAuthSecurityContextHolder;
import org.springframework.security.oauth.consumer.ProtectedResourceDetails;
import org.springframework.security.oauth.consumer.client.OAuthRestTemplate;
import org.springframework.security.oauth.consumer.token.OAuthConsumerTokenServices;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;

@Controller
@Path("")
@Transactional
public class OAuth1Resource {
    private static final Logger logger = LoggerFactory.getLogger(OAuth1Resource.class);
    @Autowired
    private OAuthRestTemplate oauthRestTemplate;
    @Autowired
    private OAuthConsumerTokenServices tokenServices;

    @GET
    @Path("/oauth1/verify")
    @Produces(MediaType.APPLICATION_JSON)
    public Response verify(@QueryParam("oauth_token") String oauth_token, @QueryParam("oauth_verifier") String oauth_verifier) {
        OAuthConsumerSupport s = oauthRestTemplate.getSupport();
        ProtectedResourceDetails r = oauthRestTemplate.getResource();

        OAuthConsumerToken requestToken = tokenServices.getToken(r.getId());
        OAuthConsumerToken accessToken = s.getAccessToken(r /* <== Specifying this was important [!] */, requestToken, oauth_verifier);
        // Save this access token to our HTTP Session based token services:
        tokenServices.storeToken(r.getId(), accessToken);
        // Save this access token to our security context, used by
        // OAuthClientHttpRequestFactory to authorize
        // outbound requests.
        OAuthSecurityContext context = OAuthSecurityContextHolder.getContext();
        context.getAccessTokens().put(r.getId(), accessToken);

        String profile = oauthRestTemplate.getForObject("https://api.openstreetmap.org/api/0.6/user/details", String.class);
        logger.info(profile);

        return Response.status(200).entity("{}").type(MediaType.APPLICATION_JSON).build();
    }

    @GET
    @Path("/oauth1/request")
    @Produces(MediaType.TEXT_PLAIN)
    public String request() {
        OAuthConsumerSupport s = oauthRestTemplate.getSupport();
        ProtectedResourceDetails r = oauthRestTemplate.getResource();

        OAuthConsumerToken requestToken = s.getUnauthorizedRequestToken(r, "http://host.local/hoot-services/auth/oauth1");
        tokenServices.storeToken(r.getId(), requestToken);
        return r.getUserAuthorizationURL() + "?oauth_token=" + requestToken.getValue();
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
