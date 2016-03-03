module.exports = QuietReporter;

function QuietReporter(runner) {
  var passes = 0;
  var failures = 0;

  runner.on('pass', function(test){
    passes++;
  });

  runner.on('fail', function(test, err){
    failures++;
    console.log('fail: %s -- error: %s', test.fullTitle(), err.message);
  });

  runner.on('end', function(){
    if (failures > 0)
    {
        console.log('end: %d/%d', passes, passes + failures);
    }
    process.exit(failures);
  });
}
