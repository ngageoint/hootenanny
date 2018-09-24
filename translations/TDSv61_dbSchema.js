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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// Convert TDSv61 to/from OSM+
//

// hoot.require('config');
hoot.require('translate');

// Create the output Schema
// function getDbSchema()
function buildTds61Schema()
{

    print('Start: buildTds61Schema');

    if (typeof config == 'undefined')
    {
        print('## buildSchema Loading config');
        hoot.require('config');
        print('## buildSchema Finished Loading config');
    }

    // var configOut = {};
    // configOut.OgrEsriFcsubtype = config.getOgrEsriFcsubtype();
    // configOut.OgrEsriFdname = config.getOgrEsriFdname();
    // configOut.OgrNoteExtra = config.getOgrNoteExtra();
    // configOut.OgrThematicStructure = config.getOgrThematicStructure();

    print('## buildSchema Check Thematic');

    if (config.getOgrThematicStructure() == 'false')
    {
        print('## buildSchema Not Thematic');

        hoot.require('tds61_schema');
        var rawSchema = tds61schema.getDbSchema();

        // Add the Very ESRI specific FCSubtype attribute
        if (config.getOgrEsriFcsubtype() == 'true') rawSchema = translate.addFCSubtype(rawSchema);

        // Add empty "extra" feature layers if needed
        if (config.getOgrNoteExtra() == 'file') rawSchema = translate.addExtraFeature(rawSchema);        

        // Now add an o2s[A,L,P] feature to the tds61.rawSchema
        // We can drop features but this is a nice way to see what we would drop
        rawSchema = translate.addEmptyFeature(rawSchema);

        // Add the empty Review layers
        rawSchema = translate.addReviewFeature(rawSchema);

        // Debugging:
        // translate.dumpSchema(rawSchema);

        return rawSchema;
    }

    print('## buildSchema Yes Thematic');

    // Load the Thematic version of the schema
    hoot.require('tds61_thematic_schema');
    var rawSchema = tds61schema.getDbSchema();

    // Add the Very ESRI specific FCSubtype attribute
    if (config.getOgrEsriFcsubtype() == 'true') rawSchema = translate.addFCSubtype(rawSchema);

    // Add empty "extra" feature layers if needed
    if (config.getOgrNoteExtra() == 'file') rawSchema = translate.addExtraFeature(rawSchema);        

    // Add the ESRI Feature Dataset name to the schema
    //  newSchema = translate.addFdName(newSchema,'TDS');
    if (config.getOgrEsriFdname() !== "") rawSchema = translate.addFdName(rawSchema,config.getOgrEsriFdname());

    // Now add the o2s feature to the tds61.rawSchema
    // We can drop features but this is a nice way to see what we would drop
    // NOTE: We add these feature AFTER adding the ESRI Feature Dataset so that they
    // DON'T get put under the Feature Dataset in the output.
    rawSchema = translate.addEmptyFeature(rawSchema);

    // Add the empty Review layers
    rawSchema = translate.addReviewFeature(rawSchema);

    // Debugging:
    // translate.dumpSchema(rawSchema);

    return rawSchema;






    // // NOTE: We use this in other functions
    // hoot.require('tds61_schema');
    // tds61.rawSchema = tds61.schema.getDbSchema();
    // print('Back from getDbSchema');

    // if (tds61.configOut.OgrEsriFcsubtype == 'true') tds61.rawSchema = translate.addFCSubtype(tds61.rawSchema);



    // // Debug:
    // // print("tds61.AttrLookup");
    // // translate.dumpLookup(tds61.AttrLookup);

    // // Decide if we are going to use TDS structure or 1 FCODE / File
    // // if we DON't want the new structure, just return the tds61.rawSchema
    // if (tds61.configOut.OgrThematicStructure == 'false')
    // {
    //     // Debugging:
    //     // translate.dumpSchema(tds61.rawSchema);
    //     return tds61.rawSchema;
    // }

    // // OK, now we build a new schema
    // var newSchema = [];
    // hoot.require('tds61_thematic_schema');
    // newSchema = tds61schema.getDbSchema();
    // print('Back from thematic getDbSchema');

    // // Debug:
    // print("tdsAttrLookup");
    // translate.dumpLookup(tdsAttrLookup);

    // // Add the ESRI Feature Dataset name to the schema
    // //  newSchema = translate.addFdName(newSchema,'TDS');
    // if (tds61.configOut.OgrEsriFdname !== "") newSchema = translate.addFdName(newSchema,tds61.configOut.OgrEsriFdname;

    // // Now add the o2s feature to the tds61.rawSchema
    // // We can drop features but this is a nice way to see what we would drop
    // // NOTE: We add these feature AFTER adding the ESRI Feature Dataset so that they
    // // DON'T get put under the Feature Dataset in the output.
    // newSchema = translate.addEmptyFeature(newSchema);

    // // Add the empty Review layers
    // newSchema = translate.addReviewFeature(newSchema);

    // // Debug:
    // // translate.dumpSchema(newSchema);
    //     print('## buildSchema Finished all');

    // return newSchema;

} // End getDbSchema
