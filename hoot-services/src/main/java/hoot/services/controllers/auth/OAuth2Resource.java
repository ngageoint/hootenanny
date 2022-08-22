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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.auth;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import javax.annotation.PostConstruct;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

import net.jodah.expiringmap.ExpiringMap;

import org.apache.commons.lang3.RandomStringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContext;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.oauth2.client.authentication.OAuth2AuthenticationToken;
import org.springframework.security.oauth2.client.registration.ClientRegistration;
import org.springframework.security.oauth2.client.registration.InMemoryClientRegistrationRepository;
import org.springframework.security.oauth2.core.AuthorizationGrantType;
import org.springframework.security.oauth2.core.user.DefaultOAuth2User;
import org.springframework.security.oauth2.core.user.OAuth2User;
import org.springframework.security.web.context.HttpSessionSecurityContextRepository;
import org.springframework.stereotype.Controller;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.client.RestClientException;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.util.UriComponentsBuilder;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import hoot.services.models.db.Users;


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

        @Autowired
        private UserManager userManager;

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

        private ResponseEntity<String> doTokenRevoke() throws RestClientException {
            ClientRegistration osmRegistration = clientRegistry.findByRegistrationId("osm");
            RestTemplate restTemplate = new RestTemplate();

            SecurityContext sc = SecurityContextHolder.getContext();
            OAuth2AuthenticationToken oToken = (OAuth2AuthenticationToken)sc.getAuthentication();
            String accessToken = oToken.getPrincipal().getAttribute("Bearer");

            HttpHeaders headers = new HttpHeaders();
            headers.add("content-type", "application/x-www-form-urlencoded");

            HttpEntity<String> request = new HttpEntity<String>(headers);

            String revokeUrl = osmRegistration.getProviderDetails().getTokenUri().replaceAll("token$", "revoke");
            String revokeRequest = UriComponentsBuilder
                .fromHttpUrl(revokeUrl)
                .queryParam("client_id", osmRegistration.getClientId())
                .queryParam("client_secret", osmRegistration.getClientSecret())
                .queryParam("token", accessToken)
                .build().encode().toUri().toString();

            return restTemplate.postForEntity(revokeRequest, request, String.class);
        }

        private ResponseEntity<String> doTokenRequest(String code) throws RestClientException {
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

        private ResponseEntity<String> doUserDetailsRequest(String tokenType, String accessToken) throws RestClientException {
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


        private Map<String, Boolean> states = ExpiringMap.builder()
            .maxSize(200)
            .expiration(2, TimeUnit.MINUTES).build();

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
        public Response callback(@Context HttpServletRequest request, @QueryParam("state") String state, @QueryParam("code") String code) {

            if (code == null) {
                return Response.status(Response.Status.BAD_REQUEST).entity("Code parameter is missing in OAuth callback URL").build();
            }

            if (!states.containsKey(state)) {
                return Response.status(Response.Status.BAD_REQUEST).entity("State parameter is missing or unknown to server in OAuth callback URL").build();
            } else {
                states.remove(state);
            };

            Users user;
            ResponseEntity<String> tokenResponse;
            try {
                tokenResponse = doTokenRequest(code);
            } catch (RestClientException e) {
                String msg = "Failed to retrieve access token from OAuth provider";
                logger.error(msg);
                return Response.status(Response.Status.BAD_GATEWAY).entity(msg).build();
            }

            JsonNode tokenResponseJson;
            try {
                tokenResponseJson = new ObjectMapper()
                    .readTree(tokenResponse.getBody());
            } catch (JsonProcessingException e) {
                String msg = "Received invalid token response from OAuth provider";
                logger.error(msg);
                return Response.status(Response.Status.BAD_GATEWAY).entity(msg).build();
            }

            String accessToken = tokenResponseJson.get("access_token").asText();
            String tokenType = tokenResponseJson.get("token_type").asText();
            ResponseEntity<String> userDetailsRequest;
            try {
                userDetailsRequest = doUserDetailsRequest(tokenType, accessToken);
            } catch (RestClientException e) {
                String msg = "Failed to retrieve user from OAuth provider";
                logger.error(msg);
                return Response.status(Response.Status.BAD_GATEWAY).entity(msg).build();
            }

            JsonNode userDetailsJson;
            try {
                userDetailsJson = new ObjectMapper().readTree(userDetailsRequest.getBody());
            } catch (JsonProcessingException e) {
                String msg = "Received invalid user response from oauth provider";
                logger.error(msg);
                return Response.status(Response.Status.BAD_GATEWAY).entity(msg).build();
            }

            user = userManager.upsert(userDetailsJson, tokenType, accessToken, request.getSession().getId());

            Map<String,Object> attributes = new HashMap<>();
            attributes.put("name", user.getDisplayName());
            attributes.put("Bearer", accessToken);
            SecurityContext sc = SecurityContextHolder.getContext();
            OAuth2User oUser = new DefaultOAuth2User(null, attributes, tokenType);
            Authentication auth = new OAuth2AuthenticationToken(oUser, null, clientId);
            sc.setAuthentication(auth);
            HttpSession session = request.getSession(false);
            session.setAttribute(HttpSessionSecurityContextRepository.SPRING_SECURITY_CONTEXT_KEY, sc);

            return Response.status(Response.Status.OK)
                    .entity(user)
                    .type(MediaType.APPLICATION_JSON).build();
        };

        @GET
        @Path("/oauth2/logout")
        @Produces(MediaType.TEXT_PLAIN)
        public Response logout(@Context HttpServletRequest request) {
            // Revoke the osm access token
            try {
                ResponseEntity<String> revokeResponse = doTokenRevoke();
            } catch (RestClientException ex) {
                String errMessage = "Failed to revoke OAuth access token";
                logger.error(errMessage);
                return Response.status(Response.Status.BAD_GATEWAY).entity(errMessage).build();
            } finally {
                // Invalidate HTTP Session
                HttpSession sess = request.getSession();
                sess.invalidate();

                // Clear the security context
                SecurityContext sc = SecurityContextHolder.getContext();
                sc.setAuthentication(null);
                SecurityContextHolder.clearContext();
            }

            return Response.ok().build();
        }

 }