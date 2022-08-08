This parser was started in order to allow parsing .qsf files so we could get pins and the signals they are linked to.

The Quartus® Prime Settings File (.qsf) contains all of the project-wide and entity-level assignments and settings for the current revision of the project. A separate Quartus® Prime Settings File exists for each individual revision. The Quartus® Prime Settings File syntax is based on Tcl script syntax. 


Since the project will allow generic boards with custom pins, it makes more sense to create custom config files with a new parser instead of parsing .qsf ones.


So this parser is no longer used and here for future reference only (**and maybe in the future, this could be used to allow transpiling .qsf to the new custom format**)