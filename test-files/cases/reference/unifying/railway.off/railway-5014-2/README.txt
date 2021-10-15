This tests the one to many rail match capability and transfers all tags from matching secondary 
features to reference features. The single rail from the second input should have all of its tags 
transferred to all three rails in the first input, it should not have its geometry merged with any 
input 1 rail, and it should be deleted. Note that the railway.one.to.many.transfer.keys config 
option value in the configuration file is ignored but left in the file for testing purposes.
