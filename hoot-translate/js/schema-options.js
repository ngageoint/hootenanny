var options = [
               {name:'OSM', enabled:true, checked:true},
               {name:'OSM+', enabled:true},
               {name:'TDSv40', enabled:true},
               {name:'TDSv61', enabled:true}
               //{name:'NAS', enabled:false}
               ];

var schema = sidebar.append('div')
    .attr('class', 'fill-darken0 pad1 space-bottom1');

schema.append('label')
    .attr('class', 'inline')
    .html('Tag Schema');

var schema_options = schema
    .selectAll('input label')
    .data(options);

var opts = schema_options.enter().append('span')
    .attr('class', 'inline pad0');
opts.append('input')
    .attr('class', 'inline schema-option')
    .attr('type', 'radio')
    .attr('name', 'schema')
    .attr('id', function(d) { return d.name; })
    .attr('value', function(d) { return d.name; })
    .property('disabled', function(d) { return !d.enabled; })
    .property('checked', function(d) { return d.checked; })
    .on('change', function(d) {
        var schema = d.name;
        if (schema == 'TDSv40' || schema == 'TDSv61') {
//            var fcodeKeys = d3.keys(eTDSfcodes['TDSv61']);
//            console.log(eTDSschemas[schema].filter(function(d) {
//                return fcodeKeys.indexOf(d.key) == -1;
//            }).map(function(d) { return d.key; }));
//
//            console.log(fcodeKeys.filter(function(d) {
//                return eTDSschemas[schema].map(function(s) {
//                    return s.key;
//                }).indexOf(d) == -1;;
//            }).sort());

            //Show the combobox
            fcodeInput.classed('hidden', false);

            //Add a combobox with all the fcode feature types
            var featureTypes = fcodeInput.selectAll('input')
                .data([eTDSschemas[schema].map(function(d) { return d.key; })]);
            featureTypes.enter().append('input')
            .classed("preset-input-access", true)
            .style("margin", "0")
            .style("width", "100%")
            .attr('type', 'text')
            .attr('id', 'preset-input-fcode')
            .on('change', function(d) {
                //update the tags map
                var schema = d3.select('input[type=radio].schema-option:checked').attr('value');
                var fcode = d3.select(this).property('value');
                var fcodetags = eTDSschemas[schema].filter(function(d) {
                    return d.key == fcode;
                });
                tags[schema] = (fcodetags.length == 1) ? fcodetags[0].values : [];
            });

            featureTypes.property('value', '').each(function(d) {
                d3.select(this)
                    .call(d3.combobox()
                        .data(d.map(function(obj) {
                            return {title: obj, value: obj};
                        }))
                    );
             });

            featureTypes.exit().remove();
        } else {
            //Hide the combobox
            fcodeInput.classed('hidden', true);
        }
    });
opts.append('label')
    .attr('class', 'inline')
    .attr('for', function(d) { return d.name; })
    .html(function(d) { return d.name; });

var fcodeInput = schema.append('div')
    .classed('preset-input-access hidden', true);