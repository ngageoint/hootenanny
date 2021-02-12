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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.grail;

import java.lang.reflect.Constructor;

import org.springframework.stereotype.Component;


/**
 * Used for constructing an object from the specified 'grailCommandClass' passed in the constructor
 * The middle man for constructing the various grail commands
 */
@Component
class GrailCommandFactory {

    GrailCommand build(String jobId, GrailParams params, String debugLevel, Class<? extends GrailCommand> grailCommandClass, Class<?> caller) {
        try {
            Constructor<? extends GrailCommand> constuctor =
                    grailCommandClass.getDeclaredConstructor(String.class, GrailParams.class, String.class, Class.class);
            return constuctor.newInstance(jobId, params, debugLevel, caller);
        }
        catch (Exception e) {
            throw new RuntimeException("Failed to instantiate an instance of class: " + grailCommandClass.getName(), e);
        }
    }
}
