package hoot.services;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Profile;
import org.springframework.context.annotation.PropertySource;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.web.AuthenticationEntryPoint;
import org.springframework.security.web.RedirectStrategy;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.access.AccessDeniedHandler;


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
            .antMatchers("/**").authenticated();
        return http.build();
    }

    @Bean
    public AuthenticationEntryPoint authenticationEntryPoint(){
        AuthenticationEntryPoint entryPoint =
          new AuthenticationEntryPointImpl();
        return entryPoint;
    }

    @Bean
    public AccessDeniedHandler accessDeniedHandler(){
        AccessDeniedHandler handler =
          new HootAccessDeniedHandler();
        return handler;
    }

    @Bean
    public RedirectStrategy redirectStrategy(){
        RedirectStrategy strategy =
          new HootInvalidSessionStrategy();
        return strategy;
    }

//    @Bean
//    public HttpSessionBasedTokenServices tokenServices(){
//        HttpSessionBasedTokenServices tokenServices =
//          new HttpSessionBasedTokenServices();
//        return handler;
//    }
}
