#pragma once
#include <typeinfo>
#include <vector>
#include <string>

#include <functional>

#include <Wiwa/Core.h>
#include <Wiwa/utilities/containers/Array.h>

#define memberoffset(type, member) (int)&((type*)0)->member

struct Type {
	const char* name;
	size_t size;
	size_t hash;
	bool is_class;
	bool is_enum;
	bool is_array;

	bool operator==(const Type* other) { return Equals(other); }

	bool Equals(const Type* other) { return hash == other->hash; }
};

struct Field {
	const Type* type;
	const char* name;
	size_t offset;
};

struct Function {
	Field returnValue;
	Field parameters[3];
	const char* name;
};

struct Class : public Type {
	std::vector<Field> fields;
	size_t field_count;
};

struct EnumMember {
	const char* name;
	int value;
};

struct Enum : public Type {
	std::vector<EnumMember> members;
	size_t member_count;
};

// FUNCTIONS
template<class T> inline const Type* GetType_impl() {
	static Type type;

	type.name = typeid(T).name();
	type.size = sizeof(T);
	type.hash = std::hash<std::string>{}(type.name);

	type.is_class = false;
	type.is_enum = false;

	return &type;
}

template<class T> inline const Type* GetType() {
	static const Type* type = GetType_impl<T>();

	return type;
}

template<class T> inline const Class* GetClass() {
	return (const Class*)GetType<T>();
}

template<class T> inline const Enum* GetEnum() {
	return (const Enum*)GetType<T>();
}

// COMPILE-TIME FUNCTIONS
template<size_t c>
const Type* GetCompileType() {
	return NULL;
}

#define REGISTER_TYPE(type) template<> inline const Type* GetCompileType<__COUNTER__>(){ \
	return GetType<type>(); \
}

// REFLECT STRUCT / CLASS
#define REFLECTION_BEGIN(rtype) REGISTER_TYPE(rtype); \
template<> inline const Type* GetType_impl<rtype>(){ \
	static Class type; \
	type.name = typeid(rtype).name(); \
	type.size = sizeof(rtype); \
	type.hash = std::hash<std::string>{}(type.name); \
	type.is_class = true; \
	type.is_enum = false; \
	type.is_array = false; \
	type.field_count = 0; \
	rtype temp;

#define REFLECT_MEMBER(member) type.fields.emplace_back(); \
	type.fields[type.field_count].name = #member; \
	type.fields[type.field_count].type = GetType<decltype(((decltype(temp)*)0)->member)>(); \
	type.fields[type.field_count].offset = memberoffset(decltype(temp), member); \
	type.field_count++;

#define REFLECTION_END return &type; }

// REFLECT ENUMERATOR
#define ENUM_REFLECTION_BEGIN(rtype) \
template<> inline const Type* GetType_impl<rtype>(){ \
	static Enum type; \
	type.name = typeid(rtype).name(); \
	type.size = sizeof(rtype); \
	type.hash = std::hash<std::string>{}(type.name); \
	type.is_class = false; \
	type.is_enum = true; \
	type.is_array = false; \
	type.member_count = 0; \
	rtype temp;

#define ENUM_REFLECT_MEMBER(member) type.members.emplace_back(); \
	type.members[type.member_count].name = #member; \
	type.members[type.member_count].value = (int)decltype(temp)::member; \
	type.member_count++;

#define ENUM_REFLECTION_END REFLECTION_END

//======== Compile-time types ================================================================================================================

extern const size_t TYPE_COUNT;

// No return foreach
template <size_t N, size_t I = 0>
void GetTypes(Wiwa::Array<const Type*, N>& arr) {
	// Take type
	arr[I] = GetCompileType<I>();

	// Recurse from I to N
	if constexpr (I + 1 < N) GetTypes<N, I + 1>(arr);
};

template <size_t count>
const Wiwa::Array<const Type*, count>& GetTypes() {
	static Wiwa::Array<const Type*, count> types{};

	GetTypes<count, 0>(types);

	return types;
};

#define GET_TYPES() GetTypes<TYPE_COUNT>()