#pragma once

constexpr size_t FNV_OFF_BASIS{ 0xcbf29ce484222325 };
constexpr size_t FNV_PRIME{ 0x100000001b3 };

constexpr size_t FNV1_Hash(const char* const str, const size_t value) noexcept
{
	return (str[0] == '\0') ? value : FNV1_Hash(&str[1], (value * FNV_PRIME) ^ size_t(str[0]));
}

constexpr size_t FNV1a_Hash(const char* const str, const size_t value) noexcept
{
	return (str[0] == '\0') ? value : FNV1a_Hash(&str[1], (value ^ size_t(str[0])) * FNV_PRIME);
}

#define FNV1_HASH(str) FNV1_Hash(str, FNV_OFF_BASIS)
#define FNV1A_HASH(str) FNV1a_Hash(str, FNV_OFF_BASIS)