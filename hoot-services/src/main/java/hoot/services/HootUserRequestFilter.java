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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */
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
