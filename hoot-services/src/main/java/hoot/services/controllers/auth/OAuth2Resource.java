package hoot.services.controllers.auth;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.URI;
import java.net.URL;
import java.net.URLConnection;
import java.security.Principal;
import java.util.Collections;
import java.util.concurrent.ConcurrentHashMap;

import javax.annotation.PostConstruct;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;

import org.apache.commons.lang3.RandomStringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.http.RequestEntity;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AnonymousAuthenticationToken;
import org.springframework.security.config.web.server.ServerHttpSecurity;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.AuthorityUtils;
import org.springframework.security.core.context.SecurityContext;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.oauth2.client.endpoint.DefaultAuthorizationCodeTokenResponseClient;
import org.springframework.security.oauth2.client.registration.ClientRegistration;
import org.springframework.security.oauth2.client.registration.ClientRegistrationRepository;
import org.springframework.security.oauth2.client.registration.InMemoryClientRegistrationRepository;
import org.springframework.security.oauth2.client.AuthorizationCodeOAuth2AuthorizedClientProvider;
import org.springframework.security.oauth2.client.OAuth2AuthorizeRequest;
import org.springframework.security.oauth2.client.authentication.OAuth2AuthenticationToken;
import org.springframework.security.oauth2.core.AuthorizationGrantType;
import org.springframework.security.oauth2.core.endpoint.DefaultOAuth2AccessTokenResponseMapConverter;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.util.UriComponentsBuilder;
import org.xml.sax.SAXException;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import hoot.services.ApplicationContextUtils;
import hoot.services.HootProperties;
import hoot.services.WebSecurityConfig;
import hoot.services.command.ExternalCommandManagerImpl;
import hoot.services.models.db.Users;

import javax.ws.rs.core.Response;
import javax.xml.parsers.ParserConfigurationException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.NewCookie;
import javax.ws.rs.core.Cookie;
import javax.ws.rs.core.Request;


 @Controller
 @ComponentScan(basePackages = {"hoot.services"})
 @Path("")
 @Transactional
 public class OAuth2Resource {

	    @Value("${spring.security.oauth2.client.registration.osm.client-name}")
	    private String clientName;

	    @Value("${spring.security.oauth2.client.registration.osm.client-id}")
	    private String clientId;

	    @Value("${spring.security.oauth2.client.registration.osm.client-secret}")
	    private String clientSecret;

	    @Value("${spring.security.oauth2.client.registration.osm.scope}")
	    private String scope;

	    @Value("${spring.security.oauth2.client.registration.osm.authorization-grant-type}")
	    private String authorizationGrantType;

	    @Value("${spring.security.oauth2.client.registration.osm.redirect-uri}")
	    private String redirectUri;

	    @Value("${spring.security.oauth2.client.provider.osm.user-info-uri}")
	    private String userInfoUri;

	    @Value("${spring.security.oauth2.client.provider.osm.authorization-uri}")
	    private String authorizationUri;

	    @Value("${spring.security.oauth2.client.provider.osm.token-uri}")
	    private String tokenUri;

	    // TODO: get this from config somewhere
//	    private String cookiePath = "";


	    @Autowired
	    private UserManager userManager;

//	    @Autowired
//	    private RestTemplate restTemplate;



	    private InMemoryClientRegistrationRepository clientRegistry;

	    @PostConstruct
	    public void init() {
	    	clientRegistry = new InMemoryClientRegistrationRepository(
		        ClientRegistration.withRegistrationId(clientName)
		        	.tokenUri(tokenUri)
		        	.clientId(clientId)
		        	.clientSecret(clientSecret)
		        	.clientName(clientName)
		        	.scope(scope)
		        	.redirectUri(redirectUri)
		        	.authorizationUri(authorizationUri)
		        	.authorizationGrantType(new AuthorizationGrantType(authorizationGrantType))
		        	.userInfoUri(userInfoUri)
		        	.build()
		    );
	    }

	    private String buildAuthorizationRequest() {
	    	ClientRegistration osmRegistration = clientRegistry.findByRegistrationId("osm");
	    	return UriComponentsBuilder
	    		.fromHttpUrl(osmRegistration.getProviderDetails().getAuthorizationUri())
	    		.queryParam("scope", String.join(" ", osmRegistration.getScopes()))
	    		.queryParam("client_id", osmRegistration.getClientId())
	    		.queryParam("response_type", "code")
	    		.queryParam("state", generateNewStateParam())
	    		.queryParam("redirect_uri", osmRegistration.getRedirectUri())
	    		.build().encode().toString();
	    }

	    private ResponseEntity<String> doTokenRequest(String code) {
	    	ClientRegistration osmRegistration = clientRegistry.findByRegistrationId("osm");
    		RestTemplate restTemplate = new RestTemplate();

    		HttpHeaders headers = new HttpHeaders();
    		headers.add("content-type", "application/x-www-form-urlencoded");

    		HttpEntity<String> request = new HttpEntity<String>(headers);

    		String tokenRequest = UriComponentsBuilder
	    		.fromHttpUrl(osmRegistration.getProviderDetails().getTokenUri())
	    		.queryParam("client_id", osmRegistration.getClientId())
	    		.queryParam("client_secret", osmRegistration.getClientSecret())
	    		.queryParam("grant_type", "authorization_code")
	    		.queryParam("redirect_uri", osmRegistration.getRedirectUri())
	    		.queryParam("code", code)
	    		.build().encode().toUri().toString();

    		return restTemplate.postForEntity(tokenRequest, request, String.class);
		}

	    private ResponseEntity<String> doUserDetailsRequest(String tokenType, String accessToken) {
	    	ClientRegistration osmRegistration = clientRegistry.findByRegistrationId("osm");
	    	RestTemplate restTemplate = new RestTemplate();

	    	HttpHeaders headers = new HttpHeaders();
	    	headers.add("Authorization", tokenType + " " + accessToken);
    		HttpEntity<String> request = new HttpEntity<String>(headers);

	    	String detailsRequest = UriComponentsBuilder
		    		.fromHttpUrl(osmRegistration
		    			.getProviderDetails()
		    			.getUserInfoEndpoint()
		    			.getUri()
		    		)
		    		.build().encode().toUri().toString();

    		return restTemplate.exchange(detailsRequest, HttpMethod.GET, request, String.class);
	    }


	    private ConcurrentHashMap<String, Boolean> states = new ConcurrentHashMap<>();
	    private String generateNewStateParam() {
	    	String stateParam = RandomStringUtils.randomAlphanumeric(32);
	    	states.put(stateParam, true);
	    	return stateParam;
	    }

	    private static final Logger logger = LoggerFactory.getLogger(OAuth2Resource.class);

	    @GET
	    @Path("/oauth2/authorize")
	    @Produces(MediaType.TEXT_PLAIN)
	    public Response request() {
	    	return Response.ok()
	    		.entity(buildAuthorizationRequest())
	    		.type(MediaType.TEXT_PLAIN).build();
	    }


	    @GET
	    @Path("/oauth2/callback")
	    @Produces(MediaType.APPLICATION_JSON)
	    public Response callback(@QueryParam("state") String state, @QueryParam("code") String code) {

	    	if (code == null) {
	    		return Response.status(401).build();
	    	}

	    	if (!states.containsKey(state)) {
	    		return Response.status(401).build();
	    	} else {
	    		states.remove(state);
	    	};

	    	HttpSession sess = getSession();
	        Users user;
	    	try {
	    		ResponseEntity<String> tokenResponse = doTokenRequest(code);

	    		if (!tokenResponse.getStatusCode().equals(HttpStatus.OK)) {
	    			return Response.status(401).build();
	    		}

		    	JsonNode tokenResponseJson = new ObjectMapper()
		    		.readTree(tokenResponse.getBody());

		    	String accessToken = tokenResponseJson.get("access_token").asText();
		    	String tokenType = tokenResponseJson.get("token_type").asText();

		    	ResponseEntity<String> userDetailsRequest = doUserDetailsRequest(tokenType, accessToken);

		    	if (!userDetailsRequest.getStatusCode().equals(HttpStatus.OK)) {
		    		return Response.status(401).build();
		    	}

		    	JsonNode userDetailsJson = new ObjectMapper().readTree(userDetailsRequest.getBody());

		        try {
		            user = userManager.upsert(userDetailsJson, accessToken, sess.getId());
		        } catch (InvalidUserProfileException | SAXException | IOException | ParserConfigurationException e) {
		            logger.error("Failed to read user profile from oauth provider", e);
		            return Response.status(502).build();
		        } catch (Exception e) {
		            logger.error("Failed to store user during oauth verification", e);
		            return Response.status(500).build();
		        }


	    	} catch (Exception e) {
	    		return Response.status(500).build();
	    	}

	    	URI redirectURI = UriComponentsBuilder
    			.fromHttpUrl(clientRegistry.findByRegistrationId("osm").getRedirectUri())
    			.build().toUri();

	        return Response.status(200)
//	        		.cookie(new NewCookie(new Cookie(
//        	    		"SESSION", sess.getId(),
//        	    		"/",
//        	    		"localhost"//redirectURI.getScheme() + "://" + redirectURI.getHost() + ":" + redirectURI.getPort()
//	        	    )))
	        		.entity(user)
	        		.type(MediaType.APPLICATION_JSON).build();
	    };


		protected HttpSession getSession() {
	        //OAuthSecurityContext context = OAuthSecurityContextHolder.getContext();
	        SecurityContext context = SecurityContextHolder.getContext();

	        if (context == null) {
	            throw new IllegalStateException("A security context must be established.");
	        }

	        HttpServletRequest request;
	        try {
	            request = (HttpServletRequest) context.getAuthentication();
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