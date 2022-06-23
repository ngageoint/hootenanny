
var assert = require('assert');

var HOOT_HOME = process.env.HOOT_HOME;
var hoot = require(HOOT_HOME + '/lib/HootJs');
hoot.Log.setLogLevel('warn');

describe('SettingsJs', function() {
    beforeEach(function() {
      if (hoot.listContains('conflate.post.ops', 'RailwayOneToManySecondaryMatchElementRemover')) {
        hoot.removeFromList({'conflate.post.ops': 'RailwayOneToManySecondaryMatchElementRemover'});
      }
    });

    it('Should append a value to a configuration list option', function() {

      var defaultConflatePostOpsStr =  hoot.get('conflate.post.ops');
      var conflatePostOps = defaultConflatePostOpsStr.split(";");
      assert.equal(false, conflatePostOps.includes('RailwayOneToManySecondaryMatchElementRemover'));

      hoot.appendToList({'conflate.post.ops': 'RailwayOneToManySecondaryMatchElementRemover'}, false);
      conflatePostOps = hoot.get('conflate.post.ops').split(";");
      assert.equal('RailwayOneToManySecondaryMatchElementRemover', conflatePostOps[conflatePostOps.length - 1]);

      hoot.set({'conflate.post.ops': defaultConflatePostOpsStr});

    }).timeout(5000);

    it('Should prepend a value to a configuration list option', function() {

      var defaultConflatePostOpsStr =  hoot.get('conflate.post.ops');
      var conflatePostOps = defaultConflatePostOpsStr.split(";");
      assert.equal(false, conflatePostOps.includes('RailwayOneToManySecondaryMatchElementRemover'));

      hoot.prependToList({'conflate.post.ops': 'RailwayOneToManySecondaryMatchElementRemover'}, false);
      conflatePostOps = hoot.get('conflate.post.ops').split(";");
      assert.equal('RailwayOneToManySecondaryMatchElementRemover', conflatePostOps[0]);

      hoot.set({'conflate.post.ops': defaultConflatePostOpsStr});

    }).timeout(5000);

    it('Should remove a value from a configuration list option', function() {

      var defaultConflatePostOpsStr =  hoot.get('conflate.post.ops');
      var conflatePostOps = defaultConflatePostOpsStr.split(";");
      assert.equal(true, conflatePostOps.includes('DuplicateWayRemover'));

      hoot.removeFromList({'conflate.post.ops': 'DuplicateWayRemover'});
      conflatePostOps = hoot.get('conflate.post.ops').split(";");
      assert.equal(false, conflatePostOps.includes('DuplicateWayRemover'));

      hoot.set({'conflate.post.ops': defaultConflatePostOpsStr});

    }).timeout(5000);

    it('Should replace a value in a configuration list option', function() {

      var defaultConflatePostOpsStr =  hoot.get('conflate.post.ops');
      var conflatePostOps = defaultConflatePostOpsStr.split(";");
      assert.equal(true, conflatePostOps.includes('DuplicateWayRemover'));
      assert.equal(false, conflatePostOps.includes('RailwayOneToManySecondaryMatchElementRemover'));
      assert.equal(false, conflatePostOps.includes('RubberSheet'));

      hoot.replaceInList({'conflate.post.ops': 'DuplicateWayRemover->RailwayOneToManySecondaryMatchElementRemover;RemoveMissingElementsVisitor->RubberSheet'});
      conflatePostOps = hoot.get('conflate.post.ops').split(";");
      assert.equal(false, conflatePostOps.includes('DuplicateWayRemover'));
      assert.equal(true, conflatePostOps.includes('RailwayOneToManySecondaryMatchElementRemover'));
      assert.equal(true, conflatePostOps.includes('RubberSheet'));

      hoot.set({'conflate.post.ops': defaultConflatePostOpsStr});

    }).timeout(5000);

    it('Should test push, pop, and clear configuration', function() {

      var origVal = hoot.get('conflate.post.ops');
      var testVal = 'WayJoinerOp;RelationCircularRefRemover';

      // Save settings
      hoot.push();

      // Now change something and verify
      hoot.set({'conflate.post.ops': testVal});
      var checkVal = hoot.get('conflate.post.ops');
      assert.equal(testVal, checkVal);

      // Clear Settings & verify
      hoot.clear();
      checkVal = hoot.get('conflate.post.ops');
      assert.equal(undefined, checkVal);

      // Set back to how it was
      hoot.pop();

      // Verify
      checkVal = hoot.get('conflate.post.ops');
      assert.equal(origVal, checkVal);
    }).timeout(5000);

});
