#include "functions.h"

//Con::printf
PrintfFn Printf;

//Con::lookupNamespace
LookupNamespaceFn LookupNamespace;

//StringTable::insert
initGameFn initGame;
StringTableInsertFn StringTableInsert;
Namespace__lookupFn Namespace__lookup;
CodeBlock__execFn CodeBlock__exec;
Sim__findObject_nameFn Sim__findObject_name;
Sim__findObject_idFn Sim__findObject_id;
Sim__postEventFn Sim__postEvent;
Sim__cancelEventFn Sim__cancelEvent;
SimObject__setDataFieldFn SimObject__setDataField;
SimObject__getDataFieldFn SimObject__getDataField;
SimObject__registerObjectFn SimObject__registerObject;
SimObject__registerReferenceFn SimObject__registerReference;
SimObject__unregisterReferenceFn SimObject__unregisterReference;
AbstractClassRep_create_classNameFn AbstractClassRep_create_className;
SetGlobalVariableFn SetGlobalVariable;
GetGlobalVariableFn GetGlobalVariable;
Sim__initFn Sim__init;

ShapeNameHudOnRenderFn ShapeNameHudOnRender;
