
cp = require('child_process');
im = require('imagemagick');

// First, checks if it isn't implemented yet.
if (!String.prototype.format) {
  String.prototype.format = function() {
    var args = arguments;
    return this.replace(/{(\d+)}/g, function(match, number) { 
      return typeof args[number] != 'undefined'
        ? args[number]
        : match
      ;
    });
  };
}

var headerHeight = 28;
var rowHeight = 20;
var rowsPerPage = 70;

function breakImage(image) {

    var width, height, rowCount;

    function breakPage(image, page) {
        var rowStart = page * rowsPerPage;
        var rowEnd = Math.min(rowCount, rowStart + rowsPerPage);
        console.log([rowStart, rowEnd, rowCount]);
        // convert tmp/hootenany-gantt.png[1253x28+0+0] tmp/hootenany-gantt.png[1253x100+0+228] -append tmp/dum.png
        var args = ["tmp/hootenany-gantt.png[{0}x28+0+0]".format(width),
                    "tmp/hootenany-gantt.png[{0}x{1}+0+{2}]".format(width, (rowEnd - rowStart) * rowHeight, rowStart * rowHeight + headerHeight),
                    "-append", 
                    "-stroke", "black", 
                    "-draw", "line 0,{0} {1},{0}".format(headerHeight + (rowEnd - rowStart) * rowHeight - 1, width),
                    "tmp/page-{0}.png".format(page)];
                
        console.log(args);

        var convert = cp.spawn('convert', args);
        convert.on('exit', function(code) {
            if (code != 0) throw new Error("Error creating appended image.");
            console.log(code);
        });
        convert.stdin.end();
    }

    im.identify(image, function(err, features) {
        if (err) throw err;
        width = features.width;
        height = features.height;
        rowCount = (height - headerHeight) / rowHeight;
        console.log(rowCount);
        pageCount = Math.ceil(rowCount / rowsPerPage);

        for (var page = 0; page < pageCount; page++) {
            breakPage(image, page);
        }
    });
}


breakImage('tmp/hootenany-gantt.png');
