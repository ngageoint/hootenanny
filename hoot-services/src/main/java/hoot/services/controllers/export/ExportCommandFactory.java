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
 * @copyright Copyright (C) 2016, 2017 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.export;

import java.lang.reflect.Constructor;

import org.springframework.stereotype.Component;


@Component
class ExportCommandFactory {

    ExportCommand build(String jobId, ExportParams params, String debugLevel, Class<? extends ExportCommand> exportCommandClass, Class<?> caller) {
        try {
            Constructor<? extends ExportCommand> constuctor =
                    exportCommandClass.getDeclaredConstructor(String.class, ExportParams.class, String.class, Class.class);
            return constuctor.newInstance(jobId, params, debugLevel, caller);
        }
        catch (Exception e) {
            throw new RuntimeException("Failed to instantiate an instance of class: " + exportCommandClass.getName(), e);
        }
    }
}
