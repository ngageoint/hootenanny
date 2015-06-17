var attributeValues;

d3.json("data/mock-attribute-values.json", function(error, json) {
    if (error) return console.warn(error);
    attributeValues = json;
});
