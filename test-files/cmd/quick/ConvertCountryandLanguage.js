#!/usr/bin/node
/*
 * Testing translation functions using Nodejs
 */


var HOOT_HOME = process.env.HOOT_HOME

// Get the hooks into the c++
hoot = require(HOOT_HOME + '/lib/HootJs');

require(HOOT_HOME + '/translations/translate.js')

// #####################################
console.log('c3 AFG to name');
console.log(translate.convertCountryCode('c3','n','AFG'));
console.log('#####');

console.log('c3 AUS to c2');
console.log(translate.convertCountryCode('c3','c2','AUS'));
console.log('#####');

console.log('c3 CAN to urn');
console.log(translate.convertCountryCode('c3','urn','CAN'));
console.log('#####');

console.log('urn ge:GENC:3:1-2:LAO to c3');
console.log(translate.convertCountryCode('urn','c3','ge:GENC:3:1-2:LAO'));
console.log('#####');

console.log('missing 01');
console.log(translate.convertCountryCode('c2','c3','smurf'));
console.log('#####');

console.log('missing 02');
console.log(translate.convertCountryCode('smurf','c3','ge:GENC:3:1-2:LAO'));
console.log('#####');

console.log('genc ge:GENC:3:1-2:LAO to local name');
console.log(translate.convertCountryCode('urn','lN','ge:GENC:3:1-2:LAO'));
console.log('#####');

console.log('name BOSNIA AND HERZEGOVINA to local name');
console.log(translate.convertCountryCode('n','lN','BOSNIA AND HERZEGOVINA'));
console.log('#####');

console.log('local name for BHUTAN');
console.log(translate.convertCountryCode('n','lN','BHUTAN'));
console.log('#####');

console.log('local name for ZAF');
console.log(translate.convertCountryCode('c3','lN','ZAF'));
console.log('#####');

console.log('country name for local name Kòrsou');
console.log(translate.convertLocalNameCountryCode('n','Kòrsou'));
console.log('#####');

console.log('c2 for local name Sewula Afrika');
console.log(translate.findCountryCode('Sewula Afrika'));
console.log('#####');

console.log('Languages for Kòrsou local country name');
cName = translate.convertLocalNameCountryCode('n','Kòrsou');
console.log('Country name: ' + cName);
localList = translate.convertCountryCode('n','lN',cName);
for (i in localList)
{
	lang2 = translate.findLanguage2Code(localList[i]);
	langName = translate.findLanguageName(localList[i]);
	console.log('Name: ' + i + '  Lang: ' + localList[i] + '  Lang2: ' + lang2 + '  LangName: ' + langName);
}
console.log('#####');

console.log('language 2code for Chuang');
console.log(translate.findLanguage2Code('Chuang'));
console.log('#####');

console.log('language 2code and name for fao');
console.log(translate.findLanguage2Code('fao'));
console.log(translate.findLanguageName('fao'));
console.log('#####');


