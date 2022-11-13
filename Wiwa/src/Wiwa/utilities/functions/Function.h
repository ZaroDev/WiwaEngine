#pragma once
namespace Fn {
	class CallableF {};

	template<class _ret, class... _params>
	class Function {
	private:
		_ret(CallableF::* func)(_params...);
		CallableF* obj;
	public:
		template<class T>
		Function(_ret(T::* _func)(_params...), void* _obj) {
			func = (_ret(CallableF::*)(_params...))_func;
			obj = (CallableF*)_obj;
		}

		_ret execute(_params... params) {
			return (obj->*func)(params...);
		}

		_ret operator()(_params... params) {
			return execute(params...);
		}

		void* getObj() { return obj; }
		void* getFuncPtr() { return (void*&)func; }
	};
}