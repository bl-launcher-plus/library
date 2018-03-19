#pragma once
#ifndef TYPES_H
#define TYPES_H

typedef unsigned int U32;
typedef signed int S32;
typedef float F32;
typedef double F64;
typedef U32 SimObjectId;

struct SimObject;
struct ConsoleObject;

//Typedefs for callbacks that Torque will call the function as.
typedef const char *(*StringCallback)(SimObject *obj, int argc, const char* argv[]);
typedef int(*IntCallback)   (SimObject *obj, int argc, const char* argv[]);
typedef float(*FloatCallback) (SimObject *obj, int argc, const char* argv[]);
typedef void(*VoidCallback)  (SimObject *obj, int argc, const char* argv[]);
typedef bool(*BoolCallback)  (SimObject *obj, int argc, const char* argv[]);


//Structs that we stole from Torque. Converted from classes to structs...
struct Namespace
{
	const char* mName;
	const char* mPackage;

	Namespace *mParent;
	Namespace *mNext;
	void *mClassRep;
	U32 mRefCountToParent;
	struct Entry
	{
		enum
		{
			GroupMarker = -3,
			OverloadMarker = -2,
			InvalidFunctionType = -1,
			ScriptFunctionType,
			StringCallbackType,
			IntCallbackType,
			FloatCallbackType,
			VoidCallbackType,
			BoolCallbackType
		};

		Namespace *mNamespace;
		//char _padding1[4];
		Entry *mNext;
		const char *mFunctionName;
		S32 mType;
		S32 mMinArgs;
		S32 mMaxArgs;
		const char *mUsage;
		const char *mPackage;
		void *mCode; // CodeBlock *mCode;
		U32 mFunctionOffset;
		union {
			StringCallback mStringCallbackFunc;
			IntCallback mIntCallbackFunc;
			VoidCallback mVoidCallbackFunc;
			FloatCallback mFloatCallbackFunc;
			BoolCallback mBoolCallbackFunc;
			const char* mGroupName;
		} cb;
	};
	Entry *mEntryList;
	Entry **mHashTable;
	U32 mHashSize;
	U32 mHashSequence;

	char * lastUsage;
};

enum ACRFieldTypes
{
	StartGroupFieldType = 0xFFFFFFFD,
	EndGroupFieldType = 0xFFFFFFFE,
	DepricatedFieldType = 0xFFFFFFFF
};

struct SimObject
{
	enum {
#define BIT(x) (1 << (x))
		Deleted = BIT(0),   ///< This object is marked for deletion.
		Removed = BIT(1),   ///< This object has been unregistered from the object system.
		Added = BIT(3),   ///< This object has been registered with the object system.
		Selected = BIT(4),   ///< This object has been marked as selected. (in editor)
		Expanded = BIT(5),   ///< This object has been marked as expanded. (in editor)
							 //ModStaticFields = BIT(6),    ///< The object allows you to read/modify static fields
							 //ModDynamicFields = BIT(7)     ///< The object allows you to read/modify dynamic fields
							 // why are these different?
							 ModStaticFields = BIT(7),    ///< The object allows you to read/modify static fields
							 ModDynamicFields = BIT(8)     ///< The object allows you to read/modify dynamic fields
	};

	char _padding1[4]; //This padding actually contains the AbstractClassRep of the object.
	const char *objectName;
	// char _padding2[24];
	// SimObjectId mId;
	SimObject *nextNameObject;
	SimObject *nextManagerNameObject;
	SimObject *nextIdObject;
	void *mGroup;
	unsigned int mFlags;
	void *mNotifyList;
	SimObjectId id;
	Namespace *mNameSpace;
	unsigned int mTypeMask;
	void *mFieldDictionary;
};

struct SimDatablock : public SimObject {
	typedef SimObject Parent;
	S32 modifiedKey;
	SimObjectId sNextObjectId;
	S32 sNextModifiedKey;
};

struct BitSet32 {
	U32 mbits;
};

struct EnumTable
{
	/// Number of enumerated items in the table.
	S32 size;

	/// This represents a specific item in the enumeration.
	struct Enums
	{
		S32 index;        ///< Index label maps to.
		const char *label;///< Label for this index.
	};

	Enums *table;

	/// Constructor.
	///
	/// This sets up the EnumTable with predefined data.
	///
	/// @param sSize  Size of the table.
	/// @param sTable Pointer to table of Enums.
	///
	/// @see gLiquidTypeTable
	/// @see gAlignTable
	EnumTable(S32 sSize, Enums *sTable)
	{
		size = sSize; table = sTable;
	}
};

struct Field {
	const char* pFieldname;    ///< Name of the field.
	const char* pGroupname;      ///< Optionally filled field containing the group name.
								 ///
								 ///  This is filled when type is StartField or EndField

	const char*    pFieldDocs;    ///< Documentation about this field; see consoleDoc.cc.
	bool           groupExpand;   ///< rip expanded/not state of this group in the editor.
	U32            type;          ///< A type ID. @see ACRFieldTypes
	U32            offset;        ///< Memory offset from beginning of class for this field.
	S32            elementCount;  ///< Number of elements, if this is an array.
	EnumTable*    table;         ///< If this is an enum, this points to the table defining it.
	BitSet32       flag;          ///< Stores various flags
	void *validator;     ///< Validator, if any.
};

struct SimEvent
{
	SimEvent *nextEvent;
	unsigned int startTime;
	unsigned int time;
	unsigned int sequenceCount;
	SimObject *destObject;
};

#endif // TYPES_H
