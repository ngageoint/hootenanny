var extension = ".dbf";

var openfile = sidebar.append('div');
openfile.append('a')
    .classed('space-bottom1 button dark animate strong block icon big plus pad2x pad1y js-toggle', true)
    .attr('href','#')
    .text('Open shapefile *.dbf')
    .on('click', function() {
        d3.select(this).select("input").node().click();
    })
    .append('input')
    .attr('type', 'file')
    .attr('multiple', 'false')
    .attr('accept', extension)
    .classed('hidden', true)
    .on('change', function () {

        var file = this.files[0];
        if (file.name.indexOf(extension) > -1) {
            if (file) {
                d3.selectAll('a, div').classed('wait', true);
                new DBF(file, function (data) {
                    initMapping(calcUniqueValues(data), file.name);
                    d3.selectAll('a, div').classed('wait', false);
                });
            }
        } else {
            openfile.append('div')
                .text("Please select a .dbf file.")
                .style("color", "red")
                .classed("fr", true)
                .transition()
                .duration(3500)
                .style("opacity", 0)
                .remove();
        }

        function calcUniqueValues(d) {
            //build a map of attribute field names to an array of their unique values
            var attrUniqueValues = d3.map();
            d.records.forEach( function(rec) {
                d3.entries(rec).forEach( function(attr) {
                    if (attrUniqueValues.has( attr.key )) {
                        //Limit the unique values to 30, more than this an we
                        //assume we won't be mapping them to tag values
                        if (attrUniqueValues.get( attr.key ).size() < 30) attrUniqueValues.get( attr.key ).add( attr.value );
                    } else {
                        //Suppress the _isDeleted field that js-shapefile-to-geojson includes
                        if (attr.key != '_isDeleted') attrUniqueValues.set( attr.key, d3.set([attr.value]) );
                    }
                });
            });
            //filter out attributes with only NULL values
            attrUniqueValues.forEach(function(k, v) {
                if (v.size() == 1 && v.values()[0] == '') this.remove(k);
            });
            return attrUniqueValues;
        }

    });



