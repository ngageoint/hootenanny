This is a Differential Conflation test that does not treat reviews as matches. Therefore, you should see a large group of restaurant POIs
involved in reviews passed through to output (the reviews themselves aren't passed to the diff output). If 
differential.treat.reviews.as.matches was set to the default value of true, you would not see these POIs pass through to output, since the 
reviews they are in would be treated as matches and the features in those matches automatically dropped from the output.
