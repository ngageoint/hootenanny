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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

//
// GeoNames Conversion
//

hoot.require('config')

mil2525c = {
	// Rule format
	// key:{value:{hierarchy,code_scheme,standard_identity,category,status,function_id,size_mobility,country_code,order_of_battle},
	//		value:{...},
	//		}
	// NOTE: All of the objects get joined to make the sidc_code attribute

	one2one : {
    'aeroway':{'aerodrome':['EMS.INFSTR.TSP.AIRPT','S','N','G','P','IBA---','H*','**','C'] },
    'amenity':{'atm':['EMS.INFSTR.BFI.ATM','E','N','F','P','BA----','H*','**','C'],
		       'bank':['EMS.INFSTR.BFI.BANK','E','N','F','P','BB----','H*','**','C'],
		       'college':['EMS.INFSTR.EDFAC.COLUNI','E','N','F','P','DA----','H*','**','C'],
    		   'clinic':['EMS.INFSTR.EMMED.MFOP','E','N','O','P','AJ----','H*','**','C'],
			   'ferry_terminal':['EMS.INFSTR.TSP.FRYTRM','E','N','F','P','LE----','H*','**','C'],
			   'fire_hydrant':['EMS.OPN.FIRFT.FIRHYD','E','N','O','P','CC----','**','**','C'],
			   'fire_station':['EMS.OPN.FIRFT.FIRSTN','E','N','O','P','CE----','H*','**','C'],
			   'hospital':['EMS.OPN.EMMED.HSP','S','N','G','P','IXH---','H*','**','C'],
			   'mortuary':['EMS.INFSTR.EMMED.MRG','E','N','O','P','AK----','H*','**','C'],
			   'pharmacy':['EMS.INFSTR.EMMED.RX','E','N','O','P','AL----','H*','**','C'],
			   'police':['EMS.OPN.LAWENF.POL.POLINS','E','N','O','P','DJC---','H*','**','C'],
			   'post_office':['EMS.OPN.PSTSRV.PO','E','N','F','P','HB----','H*','**','C'],
			   'prison':['EMS.OPN.LAWENF.PRSN','E','N','O','P','DK----','**','**','C'],
    		   'place_of_worship':['EMS.INFSTR.PUBVEN.RELIG','E','N','F','P','ID----','H*','**','C'],
		       'school':['EMS.INFSTR.EDFAC.SCHOOL','E','N','F','P','DB----','H*','**','C'],
		       'university':['EMS.INFSTR.EDFAC.COLUNI','E','N','F','P','DA----','H*','**','C'],
    		 },
    'barrier':{'toll_booth':['EMS.INFSTR.TSP.TOLLF','E','N','F','P','LM----','H*','**','C'] }, 
    'bridge':{'yes':['MS.INFSTR.TSP.BRG','G','N','M','P','BCB---','H*','**','X'] }, 
    'emergency':{'ambulance_station':['MS.INFSTR.EMMED.AMBLNC','E','N','O','P','AE----','H*','**','C'] }, 
    'highway':{'bus_station':['MS.INFSTR.TSP.BSTN','E','N','F','P','LD----','H*','**','X'],
    		   'rest_area':['MS.INFSTR.TSP.BSTN','E','N','F','P','LK----','H*','**','C']
    		}, 
    'lock':{'yes':['EMS.INFSTR.TSP.LCK','W','O','S','-','ML----','P-','**','C'] }, 
    'man_made':{'water_well':['EMS.INFSTR.WS.GWWELL','E','N','F','P','MD----','H*','**','C'],
    			'pumping_station':['EMS.INFSTR.WS.PMPSTN','E','O','F','P','ME----','H*','**','C'],
    			'wastewater_plant':['EMS.INFSTR.WS.WH20TF','E','N','F','P','MI----','H*','**','C']
    		}, 
    'place':{'farm':['EMS.INFSTR.AGFD.FRMRNC','E','V','F','-','AD----','H*','**','C'] }, 
    'power':{'station':['EMS.INFSTR.ENGFAC.GENSTA','E','N','F','-','EA----','H*','**','C'],
		    'plant':['EMS.INFSTR.ENGFAC.GENSTA','E','N','F','-','EA----','H*','**','C'] 
			}, 
    'seamark:type':{'anchorage':['EMS.INFSTR.TSP.ANCRG','W','O','S','-','HPBA--','P-','--','-'],
    			'coastguard_station':['EMS.INFSTR.LAWENF,CSTGD.CSTGDI','E','N','O','*','DNC---','H*','**','C']
     			}, 
    'tunnel':{'yes':['EMS.INFSTR.TSP.TNL','E','N','F','P','LP----','H*','**','C'] }, 
    'water':{'reservoir':['EMS.INFSTR.WS.RSVR','E','N','F','P','MF----','H*','**','C'] }, 
    'waterway':{'dam':['EMS.INFSTR.WS.DAM','E','N','F','P','MB----','H*','**','C'],
			},
	} // End one2one

}


// IMPORT
// translateAttributes - takes 'attrs' and returns OSM 'tags'
function translateAttributes(attrs, layerName, geometryType)
{

	// We are only processing Point features
	if (geometryType !== 'Point') return attrs;


	if (mil2525c.configIn == undefined)
	{
		mil2525c.configIn = {};
        mil2525c.configIn.OgrDebugDumptags = config.getOgrDebugDumptags();
	}

	for (var col in attrs)
	{
		var value = attrs[col];
		
		if (col in mil2525c.one2one)
		{
			if (value in mil2525c.one2one[col])
			{
				var row =mil2525c.one2one[col][value];
				attrs['mil2525c:hieraracy'] = row[0];
				attrs['mil2525c:code_scheme'] = row[1];
				attrs['mil2525c:standard_identity'] = row[2];
				attrs['mil2525c:category'] = row[3];
				attrs['mil2525c:status'] = row[4];
				attrs['mil2525c:function_id'] = row[5];
				attrs['mil2525c:size_mobility'] = row[6];
				attrs['mil2525c:country_code'] = row[7];
				attrs['mil2525c:order_of_battle'] = row[8];

				// Just grab the first one
				break;
			}
		}// End if col
	} // End for attrs

	if (attrs['mil2525c:hieraracy'])
	{
		attrs['mil2525c:sidc_code'] = attrs['mil2525c:code_scheme'] + attrs['mil2525c:standard_identity'] + 
									attrs['mil2525c:category'] + attrs['mil2525c:status'] + 
									attrs['mil2525c:function_id'] + attrs['mil2525c:size_mobility'] + 
									attrs['mil2525c:country_code'] + attrs['mil2525c:order_of_battle'];
	}

    // Debug:
    if (mil2525c.configIn.OgrDebugDumptags == 'true')
    {
        print('In Geometry: ' + geometryType + '  In layerName: ' + layerName);
        var kList = Object.keys(attrs).sort()
        for (var i = 0, fLen = kList.length; i < fLen; i++) print('Out attrs: ' + kList[i] + ': :' + attrs[kList[i]] + ':');
    }

    return attrs;

} // End of Translate Attributes


