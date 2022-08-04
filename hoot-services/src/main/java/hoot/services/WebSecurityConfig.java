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
package hoot.services;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Profile;
import org.springframework.context.annotation.PropertySource;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.web.SecurityFilterChain;


@EnableWebSecurity
@ComponentScan(basePackages = {"hoot.services"})
@PropertySource("classpath:oauth2-client.properties")
@Profile("production")
public class WebSecurityConfig {

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            .csrf().disable()
            .authorizeRequests()
            .antMatchers("/auth/**").permitAll()
            .anyRequest().authenticated();

        return http.build();
    }

// Not sure we need any of this but it was reflected in security-applicationContext.xml
//    @Bean
//    public AuthenticationEntryPoint authenticationEntryPoint(){
//        AuthenticationEntryPoint entryPoint =
//          new AuthenticationEntryPointImpl();
//        return entryPoint;
//    }
//
//    @Bean
//    public AccessDeniedHandler accessDeniedHandler(){
//        AccessDeniedHandler handler =
//          new HootAccessDeniedHandler();
//        return handler;
//    }

//    @Bean
//    public RedirectStrategy redirectStrategy(){
//        RedirectStrategy strategy =
//          new HootInvalidSessionStrategy();
//        return strategy;
//    }
}
