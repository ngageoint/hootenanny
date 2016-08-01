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
package hoot.services.nativeinterfaces;

/**
 * @author Jong Choi
 *
 *         This class provides the custom exception where it associates Http
 *         error code with exception. It defaults to Http 500 error. Since we
 *         doe not need to track all http status code, add as needed.
 *
 */
public class NativeInterfaceException extends Exception {
    private static final long serialVersionUID = 2099436655304473111L;

    // add more as needed
    public enum HttpCode {
        BAD_RQUEST(400), USER_CANCEL(1001), SERVER_ERROR(500);

        private final int value;

        HttpCode(int value) {
            this.value = value;
        }

        public int toInt() {
            return value;
        }
    }

    private NativeInterfaceException.HttpCode exceptionCode = NativeInterfaceException.HttpCode.SERVER_ERROR;

    public NativeInterfaceException.HttpCode getExceptionCode() {
        return exceptionCode;
    }

    public void setExceptionCode(NativeInterfaceException.HttpCode exceptionCode) {
        this.exceptionCode = exceptionCode;
    }

    public NativeInterfaceException(String message) {
        super(message);
    }

    public NativeInterfaceException(String message, Throwable cause) {
        super(message, cause);
    }

    public NativeInterfaceException(String message, NativeInterfaceException.HttpCode exceptCode) {
        super(message);
        exceptionCode = exceptCode;
    }

    public NativeInterfaceException(String message, NativeInterfaceException.HttpCode exceptCode, Throwable cause) {
        super(message, cause);
        exceptionCode = exceptCode;
    }
}
