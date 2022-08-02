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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.ingest;


import java.io.File;
import java.util.List;

import org.springframework.stereotype.Component;

import hoot.services.models.db.Users;


@Component
class ImportCommandFactory {

    ImportCommand build(String jobId, File workDir, List<File> filesToImport,
                         List<File> zipsToImport, String translation, String advUploadOpts, String etlName, Boolean isNoneTranslation,
                         String debugLevel, UploadClassification uploadClassification, Class<?> caller, Users user) {
        return new ImportCommand(jobId, workDir, filesToImport, zipsToImport, translation, advUploadOpts, etlName, isNoneTranslation,
                                 debugLevel, uploadClassification, caller, user);
    }

    ImportCommand build(String jobId, String url, String translation, String advUploadOpts, String etlName, Boolean isNoneTranslation,
            String debugLevel, UploadClassification uploadClassification, Class<?> caller, Users user) {
return new ImportCommand(jobId, url, translation, advUploadOpts, etlName, isNoneTranslation,
                    debugLevel, uploadClassification, caller, user);
}

}
