The output for this test is very bad, and that is why its turned off. - see #3135

Attribute Conflation test with the Network Roads Algorithm for bridge merging into a reference dataset without bridges identified.  The 
reference geometry will potentially be changed where there are bridges (see option attribute.conflation.allow.ref.geometry.changes.for.bridges).

The divided bridges named Puente: Llaguno should overwrite the divided highway named Av. Urdaneta.

Only one of the divided bridges named Viaducto: Nueva República currently replaces the divided highway named Av. Sucre.  The other one generates
a review, but unfortunately reviews are currently thrown out with Attribute Conflation.  So, this test needs fixed if possible to where both 
bridges get merged.

