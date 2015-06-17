var mapping = sidebar
    .append('form')
    .classed('round space-bottom1 attribute-mapping', true);

function initMapping(attributeValues, fileName) {


    //var geometry = attributeValues.GEOMETRY[0];

    var currentIndex = 0;

    var currentAttribute;

    var jsonMapping = {};

    //Remove existing controls
    mapping.selectAll("div, a").remove();

    function checkNext() {
        //deactive Next if no lookup node present
        var enable = Boolean(d3.select(".lookup").node());
        d3.select(".next-attribute").classed({'loud': enable, 'disabled': !enable});
    }

    function getStatus(attr) {
        var status;
        if (!jsonMapping[attr]) {
            status = '&#9744;';
        } else if (jsonMapping[attr] == 'IGNORED') {
            status = '&#9746;';
        } else {
            status = '&#9745;';
        }
        return status + ' ' + attr;
    }

    //Set up the attribute display
    function updateAttributes() {
        attributes.select("div.attributes-count")
            .html(function(d) { return (currentIndex+1) + " of " + d.keys().length + " Attributes"; });
        attributes.select("div.attributes-sample")
            .html(function(d) {
                currentAttribute = d.entries()[currentIndex];
                //If we want just one sample value
                //return d3.values(d)[currentIndex][0];
                //If we want three followed by ellipsis
                return currentAttribute.value.values().reduce(function (prev, curr, idx) {
                    if (idx == 3) {
                        return prev + "...";
                    }
                    if (idx > 3) {
                        return prev;
                    }
                    return prev + ", " + curr;
                });
            });
        var attmapping = attributes.select("div.attributes-name")
            .selectAll('div')
            .data([currentAttribute], function(d) { return d.key; });
        attmapping.enter().append('div')
            .classed('attributes-option', true);
        attmapping.classed('attributes-hover', false)
            .html(function(d) {
                return getStatus(d.key);
            })
            .on('click', function(d) {
                var attmappings = attributes.select("div.attributes-name").selectAll('div')
                    .data(attributeValues.entries(), function(d) { return d.key; });
                attmappings.enter().append('div')
                    .classed('attributes-option attributes-hover', true);
                attmappings.html(function(d, i) {
                    return getStatus(d.key);
                })
                .on('click', function(d, i) {
                    //console.log(d3.select(this).node().parentNode);
                    //d3.select(d3.select(this).node().parentNode).selectAll('div').remove();
                    currentIndex = i;
                    updateAttributes();
                });
                attmappings.exit().remove();
            });
        attmapping.exit().remove();

        d3.selectAll(".lookup").remove();
        checkNext();

        //Check jsonMapping if lookups exist, is not ignored, and should be populated
        if (jsonMapping[currentAttribute.key] && jsonMapping[currentAttribute.key] != 'IGNORED') {
            var mapping = d3.map(jsonMapping[currentAttribute.key])
            mapping.entries().forEach( function(d) {
                  var lookup = mappingSection.insert("div", ".add-mapping")
                  .classed('round space-bottom1 fill-white keyline-all lookup', true);
                  var value = d.key;
                  var schemaOption = d3.select('input[type=radio].schema-option:checked').attr('value');

                  var values = tags[schemaOption].filter(function(val) {
                      return val.key && val.key == value.toLowerCase();
                  });
                  selectTag(lookup, (values.length > 0) ? values[0] : {key: d.key, value: []});
            });
        }

        //console.log(JSON.stringify(jsonMapping));

        //If all attributes are in the mapping display the translate button
        if (d3.keys(jsonMapping).length == attributeValues.size()) {
            enableTranslate();
        }

    }

    var attributes = mapping.selectAll("div.attributes")
        .data([attributeValues]);
    attributes.enter()
        .append('div')
        .classed('attributes', true);
    var attributesCount = attributes.append("div")
        .classed('dark fill-dark center strong pad0y', true);
    attributesCount.append('div').classed('arrowicon backarrow', true).on('click', back);
    attributesCount.append('div').classed('attributes-count pad1x', true);
    attributesCount.append('div').classed('arrowicon forwardarrow', true).on('click', forward);
    var currAttribute = attributes.append("div")
            .classed('pad2y', true);
    currAttribute.append("div")
        .classed('attributes-name bigger center strong results-list auto-overflow', true);
    currAttribute.append("div")
        .classed('attributes-sample italic center quiet', true);
    updateAttributes();
    attributes.exit().remove();

    d3.select(document).on('keydown.translation-assistant', function() {
        switch (d3.event.keyCode) {
            // right arrow
            case 39:
                forward();
                break;
            // left arrow
            case 37:
                back();
                break;
        }
    })

    function forward() {
        //Advance to the next attribute
        if (currentIndex < attributeValues.size()-1) {
            currentIndex++;
        } else {
            currentIndex = 0;
        }
        updateAttributes();
    }

    function back() {
        //Return to the previous attribute
        if (currentIndex == 0) {
            currentIndex = attributeValues.size()-1;
        } else {
            currentIndex--;
        }
        updateAttributes();
    }

    function selectTag(el, d) {
        //enable Next button
        d3.select(".next-attribute").classed({'loud': true, 'disabled': false});

        //update div to attribute mapping control
        el.text(null);
        el.append("div")
            .classed('pad1 inline thumbnail fr big icon trash maptag translate-icon keyline-left', true)
            .on("click", function() {
                el.select(".results-list")
                    .classed("hidden", !el.select(".results-list").classed("hidden"));
            });
        el.append("div")
            .classed('pad1 inline thumbnail fr big icon trash removetag translate-icon', true)
            .on("click", function() {
                el.remove();
                checkNext();
            });
        el.append("label")
            .classed("pad1 space-bottom0 center bigger", true)
            .text( d.key );

        attributeMapping = el.append("div")
            .classed("auto-overflow results-list hidden", true);
        var values = d.value;

        var attributeRows = attributeMapping.append("div")
            .classed("preset-input-wrap", true)
            .append("ul")
            .selectAll("li")
            .data(currentAttribute.value.values())
            .enter().append("li")
            ;

        attributeRows.append("div")
            .classed("col6 label keyline-top", true)
            .append("span")
            .text(function(d) {
                return d;
            });

        var selectPreset = attributeRows.append("div")
            .classed("col6 keyline-top preset-input-access-wrap", true);
        var presetInput = selectPreset.append("input")
            .classed("preset-input-access", true)
            .style("margin", "0")
            .style("width", "100%")
            .attr('type', 'text')
            .attr('id', function(d) { return 'preset-input-' + hashCode(d); } )
            .each(function(d) {
                d3.select(this)
                    .call(d3.combobox()
                        .data(values.map(function(obj) {
                            return {title: obj.replace('_', ' '), value: obj};
                        }))
                    )
             });

        //Restore preset dropdown if mapping exists
        if (jsonMapping[currentAttribute.key]) {
            var mapping = d3.map(jsonMapping[currentAttribute.key])
            mapping.entries().filter( function(entry) {
                return (d.key == entry.key) && (entry.value != currentAttribute.key);
            }).forEach( function(entry) {
                attributeMapping.classed("hidden", false);
                d3.map(entry.value).entries().forEach( function(e) {
                    d3.select('input#preset-input-' + hashCode(e.key)).property("value", e.value);
                });
            })
        }

    }



    //Set up the add attribute mapping section

    //The add tag button
    var mappingSection = mapping.append("div")
        .classed('fill-darken0 pad2', true);
    var addMapping = mappingSection
        .append('div')
        .classed('space-bottom1 add-mapping', true);
    addMapping.append('a')
        .classed('button fill-transp keyline-all block icon big plus pad2 quiet', true)
        .attr('href', '#')
        .on('click', function() {

            function keydown() {
                switch (d3.event.keyCode) {
                    // backspace, delete
                    case 8:
                    case 46:
                        searchTag.on('input.searchtag', function() {
                            var start = searchTag.property('selectionStart');
                            searchTag.node().setSelectionRange(start, start);
                            searchTag.on('input.searchtag', change);
                        });
                        break;
                    // tab
                    case 9:
                        accept();
                        break;
                    // return
                    case 13:
                        d3.event.preventDefault();
                        break;
                    // up arrow
                    case 38:
                        //nav(-1);
                        d3.event.preventDefault();
                        break;
                    // down arrow
                    case 40:
                        //nav(+1);
                        d3.event.preventDefault();
                        break;
                }
                d3.event.stopPropagation();
            }

            function keyup() {
                switch (d3.event.keyCode) {
                    // escape
                    case 27:
                        remove();
                        break;
                    // return
                    case 13:
                        accept();
                        break;
                }
            }

            function blur() {
                var value = searchTag.property('value');
                var d = {key: value, value: []};
                var lookup = d3.select(searchTag.node().parentNode);
                selectTag(lookup, d);
            }

            function change() {
                //console.log(d3.event);

                var value = searchTag.property('value');
                //console.log(value);
                var results;
                if (value.length) {
                    //var schemaOption = d3.select('input[type=radio].schema-option:checked').attr('value');
                    results = tags[schemaOption]
                    .concat([{key: 'extra_attributes', value: []}]) //add the many-to-one extra_attributes key
                    .filter( function(val) {
                        return val.key && (val.key.toLowerCase().indexOf(value.toLowerCase()) == 0);
                    }).sort(function(a, b) { return b.rank - a.rank }); //sort by count_nodes (popularity) for osmTagInfo

                    //provide highlighted remaining text of suggestion
                    if (results.length > 0) {
                        var suggestion = results[0].key;
                        searchTag.property('value', suggestion);
                        searchTag.node().setSelectionRange(value.length, suggestion.length);
                        searchTag.node().focus();
                    }
                } else {
                    results = [];
                }

                updateResults(results);
            }

            function updateResults(results) {
                //The search tag results
                var searchtags = resultsList.selectAll(".search-result")
                    .data(results);

                searchtags.enter().append("div")
                    .classed("search-result pad1x pad1y keyline-left keyline-top", true);

                searchtags
                    .text( function(d) { return !d || d.key; })
                    .on("click", function(d) {
                        var lookup = d3.select(searchTag.node().parentNode);
                        selectTag(lookup, d);
                    })
                    ;

                searchtags.exit().remove();

            }

//            function nav(dir) {
//
//            }

            function accept() {
                var value = searchTag.property('value');
                if (value.length) {
                    var el = resultsList.select(".search-result:first-child");
                    //If selection is empty, use the user specified value as the tag key
                    var d = (!el.empty() && el.text() == value) ? el.datum() : {key: value, value: []};
                    var lookup = d3.select(searchTag.node().parentNode);
                    selectTag(lookup, d);
                }
            }

            function remove() {
                var lookup = d3.select(searchTag.node().parentNode);
                lookup.remove();
                checkNext();
            }

            //The tag lookup input
            var lookup = mappingSection.insert("div", ".add-mapping")
                .classed('round space-bottom1 fill-white keyline-all lookup', true);
            lookup.append("div")
                .classed('pad1 inline thumbnail big icon trash searchtag translate-icon keyline-right', true);

            var schemaOption = d3.select('input[type=radio].schema-option:checked').attr('value');

            var searchTag = lookup.append("input")
                .attr("type", "text")
                .attr("placeholder", "Search tag")
                .classed('strong bigger pad1x pad2y reset', true)
                .style("width", "auto")
                //.on('focus.searchtag', focus)
                //.on('blur.searchtag', blur)
                .on('keydown.searchtag', keydown)
                .on('keyup.searchtag', keyup)
                .on('input.searchtag', change)
                ;

            searchTag.node().focus();

            var resultsList = lookup.append("div")
                .classed("auto-overflow results-list", true);
        });






    //Set up the navigation buttons
    var navigation = mapping.selectAll("div.navigation")
        .data([attributeValues]);
    navigation.enter()
        .append('div')
        .classed('navigation pad2y auto-overflow', true)
        ;
    navigation
        .append('a')
        .classed('button strong bigger pad2 wd49 mr1', true)
        .attr('href', '#')
        .text("Ignore")
        .on('click', function(d) {
            updateAttributeMapping(d, 'IGNORED');
        });
    navigation
        .append('a')
        .classed('button strong bigger pad2 wd49 ml1 next-attribute disabled', true)
        .attr('href', '#')
        .text("Next")
        .on('click', function(d) {
            if (!d3.select(this).classed("disabled")) {
                var key = d.keys()[currentIndex];
                var value = buildAttributeMappingJson(key);
                updateAttributeMapping(d, value);
            }
        });
    navigation.exit().remove();

    //Sample mapping
    /*
        {
          "FCC": {
            "A20": {
              "highway": "primary",
              "motor_vehicles": "yes"
            }
          }
        }
     */

    function buildAttributeMappingJson(parentKey) {
        var json = {};

        d3.selectAll(".lookup").each( function() {
            if (d3.select(this).select(".searchtag").empty()) {
                var key = d3.select(this).select("label").text();
                var value = {};
                d3.select(this).selectAll("li").each( function() {
                    var k = d3.select(this).select("span").text();
                    var v = d3.select(this).select("input").property("value");
                    value[k] = v;
                });
                var attr = d3.map(value);
                if (attr.values().some( function(obj) { return obj.length > 0; })) {
                    json[key] = value;
                } else {
                    json[key] = parentKey;
                }
            }
        });

        return json;
    }

    function updateAttributeMapping(d, mapping) {
        var key = d.keys()[currentIndex];
        jsonMapping[key] = mapping;

        forward();
        updateAttributes();
    }

    function enableTranslate() {
        if (d3.selectAll(".conflate").empty()) {
            mapping
            .append('a')
            .classed('button dark animate strong block icon big conflate pad2x pad1y js-toggle', true)
            .attr('href', '#')
            .text('Translate')
            .on('click', function() {

                //Pretty print the attribute-to-tag mapping
                var json = JSON.stringify(jsonMapping, null, 4);
/*
                require('translation_assistant')

                var attributeMapping;
                var fcode;

                //translateToOsm - takes 'attrs' and returns OSM 'tags'
                var translateToOsm = function(attrs, layerName) {
                    return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode);
                }
*/
                var output = "hoot.require('translation_assistant')\n"
                    + "\n"
                    + "var attributeMapping = " + json + ";\n"
                    + "var fcode;\n"
                    + "var schema;\n"
                    + "\n"
                    + "//translateToOsm - takes 'attrs' and returns OSM 'tags'\n"
                    + "var translateToOsm = function(attrs, layerName) {\n"
                    + "    return translation_assistant.translateAttributes(attrs, layerName, attributeMapping, fcode, schema);\n"
                    + "};\n"
                    ;

                var schema = d3.select('input[type=radio].schema-option:checked').attr('value');
                if (schema === 'TDSv40' || schema === 'TDSv61') {
                    var featureType = d3.select('#preset-input-fcode').property('value');
                    var fCode = eTDSfcodes[schema][featureType] + ": " + featureType;
                    if (featureType.length == 0 || !eTDSfcodes[schema][featureType]) {
                        schema.append('div')
                            .text("Please select a valid " + schema + " feature type.")
                            .style("color", "red")
                            .classed("fr", true)
                            .transition()
                            .duration(3500)
                            .style("opacity", 0)
                            .remove();
                        return;
                    }
                    output = output.replace("var fcode;", "var fcode = '" + fCode + "';")
                                .replace("var schema;", "var schema = '" + schema + "';");
                }
                //console.log(output);

                var blob = new Blob([output], {type: "text/plain;charset=utf-8"});
                saveAs(blob, fileName.split(".")[0] + "-translation.js");

            });
        }
    }

    //http://stackoverflow.com/questions/7616461/generate-a-hash-from-string-in-javascript-jquery
    function hashCode(input) {
        var hash = 0, i, chr, len;
        if (input.length == 0) return hash;
        for (i = 0, len = input.length; i < len; i++) {
            chr   = input.charCodeAt(i);
            hash  = ((hash << 5) - hash) + chr;
            hash |= 0; // Convert to 32bit integer
        }
        return hash;
    }

}