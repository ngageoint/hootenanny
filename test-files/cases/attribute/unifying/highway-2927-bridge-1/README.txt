Attribute Conflation test with the Unifying Roads Algorithm for bridge merging into a reference dataset without bridges identified.  The 
reference geometry will potentially be changed where there are bridges (see option attribute.conflation.allow.ref.geometry.changes.for.bridges).

The divided bridges named Puente: Llaguno should overwrite the divided highway named Av. Urdaneta.

Only one of the divided bridges named Viaducto: Nueva República currently replaces the divided highway named Av. Sucre.  The other one generates
a review, but unfortunately reviews are currently thrown out with Attribute Conflation.  So, this test needs fixed if possible to where both 
bridges get merged.

Note: Look at the Northwestern corner of Av. Sucre. There is a two node crossing street present in the first input, not part of Av. Sucre, and not present in the second input. This road gets joined to Av. Sucre during way joining by WayJoinerAdvanced::_joinAtNode. It should not be merged with Av. Sucre.

