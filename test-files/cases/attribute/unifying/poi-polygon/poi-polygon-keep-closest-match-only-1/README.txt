This is the Attribute Conflation version of unifying/poi-polygon/poi-polygon-keep-closest-match-only-13.

One of the ref buildings has a dummy tag "conflation_should_not=delete_me". This tag should not be removed during merging using the default AC
configuration. Alternatively, to get rid of it, the default tag merger can be changed to ReplaceTag1Merger.

That ref building also has a "source:imagery:datetime" tag. That tag should be preserved due to that tag key entry in the default value for the
tag.merger.overwrite.exclude config option.


