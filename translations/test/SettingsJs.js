
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('SettingsJs', function() {

   it('Should append a value to a configuration list option', function() {
    
      hoot.appendToList({'conflate.post.ops': 'RailwayOneToManySecondaryMatchElementRemover'}, false);
      var conflatePostOps = hoot.get('conflate.post.ops');
      assert.equal('RailwayOneToManySecondaryMatchElementRemover', conflatePostOps[conflatePostOps.length - 1]);

    }).timeout(5000);
    
    it('Should prepend a value to a configuration list option', function() {
    
      hoot.prependToList({'conflate.post.ops': 'RailwayOneToManySecondaryMatchElementRemover'}, false);
      var conflatePostOps = hoot.get('conflate.post.ops');
      assert.equal('RailwayOneToManySecondaryMatchElementRemover', conflatePostOps[0]);

    }).timeout(5000);
    
    it('Should remove a value from a configuration list option', function() {
    
      hoot.removeFromList({'conflate.post.ops': 'DuplicateWayRemover'});
      var conflatePostOps = hoot.get('conflate.post.ops');
      assert.equal(false, conflatePostOps.includes('DuplicateWayRemover');

    }).timeout(5000);
    
    it('Should replace a value in a configuration list option', function() {
    
      hoot.replaceInList({'conflate.post.ops': 'DuplicateWayRemover'}, 'RailwayOneToManySecondaryMatchElementRemover');
      var conflatePostOps = hoot.get('conflate.post.ops');
      assert.equal(false, conflatePostOps.includes('DuplicateWayRemover');
      assert.equal(true, conflatePostOps.includes('RailwayOneToManySecondaryMatchElementRemover');

    }).timeout(5000);

});
