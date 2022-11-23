$title Compares two gdx files

$SETGLOBAL workingfolder C:\Models\SATIMGE\
$SETGLOBAL GDXoutfolder %workingfolder%GDXout\

Execute_unload 'SDG_noInv_Base_ClosuresSI6GOV11.gdx' ;

Execute_unload 'SDGbaseTra_UrbAS_BAU_NewClosNOYK3.gdx' ;

Execute 'gdxdiff SDG_noInv_Base_ClosuresSI6GOV11 SDGbaseTra_UrbAS_BAU_NewClosNOYK3 DiffNoInvBase_UrbBAU' ;

