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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.log4j2;

import org.apache.commons.lang3.exception.ExceptionUtils;
import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.Marker;
import org.apache.logging.log4j.core.LogEvent;
import org.apache.logging.log4j.core.Logger;
import org.apache.logging.log4j.core.config.plugins.Plugin;
import org.apache.logging.log4j.core.config.plugins.PluginAttribute;
import org.apache.logging.log4j.core.config.plugins.PluginFactory;
import org.apache.logging.log4j.core.filter.AbstractFilter;
import org.apache.logging.log4j.message.Message;


@Plugin(name = "BrokenPipeFilter", category = "Core", elementType = "filter", printObject = true)
public final class BrokenPipeFilter extends AbstractFilter {

    private final Level level;

    private BrokenPipeFilter(Level level, Result onMatch, Result onMismatch) {
        super(onMatch, onMismatch);
        this.level = level;
    }

    @Override
    public Result filter(Logger logger, Level level, Marker marker, String msg, Object[] params) {
        return filter(level);
    }

    @Override
    public Result filter(Logger logger, Level level, Marker marker, Object msg, Throwable t) {
        return filter(t);
    }

    @Override
    public Result filter(Logger logger, Level level, Marker marker, Message msg, Throwable t) {
        return filter(t);
    }

    @Override
    public Result filter(LogEvent event) {
        // If there is a throwable provided, inspect it.
        Throwable t = event.getThrown();
        if (t != null) {
            return this.filter(t);
        }

        return filter(event.getLevel());
    }

    private Result filter(Level level) {
        return Result.NEUTRAL;
    }

    private Result filter(Throwable t) {
        if (ExceptionUtils.getRootCauseMessage(t).contains("SocketException: Broken pipe")) {
            return Result.DENY;
        }
        else {
            return Result.NEUTRAL;
        }
    }

    @Override
    public String toString() {
        return level.toString();
    }

    @PluginFactory
    public static BrokenPipeFilter createFilter(@PluginAttribute(value = "level", defaultString = "ERROR") Level level,
            @PluginAttribute(value = "onMatch", defaultString = "NEUTRAL") Result onMatch,
            @PluginAttribute(value = "onMismatch", defaultString = "DENY") Result onMismatch) {
        return new BrokenPipeFilter(level, onMatch, onMismatch);
    }
}
