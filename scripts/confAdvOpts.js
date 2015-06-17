/**
 */
var HOOT_HOME = process.env.HOOT_HOME                                           
var hoot = require(HOOT_HOME + '/lib/HootJs');                                  

hoot.log(hoot.MatchFactory.getAllAvailableCreators());
hoot.log(hoot.MergerFactory.getAllAvailableCreators());

