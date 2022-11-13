#pragma once
extern "C" {
	typedef struct _MonoType MonoType;
}

struct Type;

namespace Wiwa {

	Type* ConvertType(MonoType* monotype);
	
	class ScriptGlue 
	{
	public:
		static void RegisterFunctions();

	};
}