package hoot.services;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import javax.ws.rs.container.ContainerRequestContext;
import javax.ws.rs.container.ContainerRequestFilter;
import javax.ws.rs.core.Context;

import org.springframework.beans.factory.annotation.Autowired;

import hoot.services.controllers.auth.UserManager;
import hoot.services.models.db.Users;

public class HootUserRequestFilter implements ContainerRequestFilter {
    public static final String HOOT_USER_ATTRIBUTE = "hoot.services#USER";

    @Context
    HttpServletRequest webRequest;

    @Autowired
    UserManager userManager;

    @Override
    public void filter(ContainerRequestContext requestContext) throws IOException {
        HttpSession session = webRequest.getSession();
        Users user = userManager.getUserBySession(session.getId());
        webRequest.setAttribute(HOOT_USER_ATTRIBUTE, user);
    }
}
