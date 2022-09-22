#include "Empathy/headers/utilities/String.h"

namespace EM {
	String::String(size_t len)
	{
		m_Text = new char[len + 1];
		m_Length = len;

		m_Text[len] = '\0';
	}

	String::String(const char* text)
	{
		set(text);
	}

	String::String(const String& str)
	{
		set(str);
	}

	String::~String()
	{
		delete[] m_Text;
	}

	char& String::operator[](int index)
	{
		return m_Text[index];
	}

	char& String::operator[](int index) const
	{
		return m_Text[index];
	}

	void String::setLength(size_t len)
	{
		if (len == m_Length) return;

		char* newText = new char[len + 1];

		if (len < m_Length) {
			memcpy(newText, m_Text, len);
		}
		else if (len > m_Length) {
			memcpy(newText, m_Text, m_Length);
		}

		newText[len] = '\0';

		delete[] m_Text;
		m_Text = newText;
	}

	void String::append(const char* text)
	{
		size_t lastLen = m_Length;
		size_t appLen = strlen(text);

		size_t newLen = lastLen + appLen + 1;

		// Copy and append
		char* newText = new char[newLen];
		memcpy(newText, m_Text, lastLen);
		memcpy(newText + lastLen, text, appLen);
		newText[newLen - 1] = '\0';

		// Swap buffers
		delete[] m_Text;
		m_Text = newText;

		m_Length = m_Length + appLen;
	}

	void String::append(const String& str)
	{
		size_t lastLen = m_Length;
		size_t appLen = str.length();

		size_t newLen = lastLen + appLen + 1;

		// Copy and append
		char* newText = new char[newLen];
		memcpy(newText, m_Text, lastLen);
		memcpy(newText + lastLen, str.data(), appLen);
		newText[newLen - 1] = '\0';

		// Swap buffers
		delete[] m_Text;
		m_Text = newText;

		m_Length = m_Length + appLen;
	}

	void String::append(char c)
	{
		size_t lastLen = m_Length;
		size_t appLen = 1;

		size_t newLen = lastLen + appLen + 1;

		// Copy and append
		char* newText = new char[newLen];
		memcpy(newText, m_Text, lastLen);
		newText[lastLen + 1] = c;
		newText[newLen - 1] = '\0';

		// Swap buffers
		delete[] m_Text;
		m_Text = newText;

		m_Length = m_Length + appLen;
	}

	void String::set(const char* text)
	{
		m_Length = strlen(text);

		m_Text = new char[m_Length + 1];
		memcpy(m_Text, text, m_Length);

		m_Text[m_Length] = '\0';
	}

	void String::set(const String& str)
	{
		m_Length = str.length();

		m_Text = new char[m_Length + 1];
		memcpy(m_Text, str.data(), m_Length);

		m_Text[m_Length] = '\0';
	}

	size_t String::find(char c, size_t startIndex) const
	{
		for (size_t i = startIndex; i < m_Length; i++) {
			if (m_Text[i] == c) return i;
		}

		return end();
	}

	size_t String::find(const char* text, size_t startIndex) const
	{
		size_t tLen = strlen(text);
		size_t j = 0;

		for (size_t i = startIndex; i < m_Length; i++) {
			if (m_Text[i] == text[j]) j++;
			else j = 0;

			if (j == tLen) return i - j + 1;
		}

		return end();
	}

	size_t String::find(const String& str, size_t startIndex) const
	{
		size_t tLen = str.length();
		size_t j = 0;

		for (size_t i = startIndex; i < m_Length; i++) {
			if (m_Text[i] == str[j]) j++;
			else j = 0;

			if (j == tLen) return i - j + 1;
		}

		return end();
	}

	size_t String::findb(char c, size_t startIndex) const
	{
		for (size_t i = startIndex; i < m_Length; i++) {
			if (m_Text[m_Length - i - 1] == c) return m_Length - i - 1;
		}

		return end();
	}

	size_t String::findb(const char* text, size_t startIndex) const
	{
		size_t tLen = strlen(text);
		size_t j = 0;

		for (size_t i = startIndex; i < m_Length; i++) {
			if (m_Text[m_Length - i - 1] == text[tLen - j - 1]) j++;
			else j = 0;

			if (j == tLen) return m_Length - i - 1;
		}

		return end();
	}

	size_t String::findb(const String& str, size_t startIndex) const
	{
		size_t tLen = str.length();
		size_t j = 0;

		for (size_t i = startIndex; i < m_Length; i++) {
			if (m_Text[m_Length - i - 1] == str[tLen - j - 1]) j++;
			else j = 0;

			if (j == tLen) return m_Length - i - 1;
		}

		return end();
	}

	String String::substr(size_t index, size_t count) const
	{
		String str(count);

		memcpy(str.data(), m_Text + index, count);

		return str;
	}

	String String::substr(size_t index) const
	{
		return substr(index, m_Length - index);
	}

	size_t String::replace(const char* text1, const char* text2, size_t startIndex)
	{
		size_t index = find(text1, startIndex);

		if (index != end()) {
			size_t len1 = strlen(text1);
			size_t len2 = strlen(text2);

			if (len1 == len2) {
				memcpy(m_Text + index, text2, len2);
			}
			else {
				size_t newLen = m_Length + (len2 - len1);

				char* newText = new char[newLen + 1];
				memcpy(newText, m_Text, index);
				memcpy(newText + index, text2, len2);
				memcpy(newText + index + len2, m_Text + index + len1, m_Length - index - len1);

				newText[newLen] = '\0';

				delete[] m_Text;

				m_Text = newText;
				m_Length = newLen;
			}
		}

		return index;
	}

	void String::replaceAll(const char* text1, const char* text2)
	{
		size_t index = 0;

		do {
			index = replace(text1, text2, index);
		} while (index != end());
	}

	void String::toLower()
	{
		for (size_t i = 0; i < m_Length; i++) {
			if (m_Text[i] >= 'A' && m_Text[i] <= 'Z') {
				m_Text[i] += 32;
			}
		}
	}

	void String::toUpper()
	{
		for (size_t i = 0; i < m_Length; i++) {
			if (m_Text[i] >= 'a' && m_Text[i] <= 'z') {
				m_Text[i] -= 32;
			}
		}
	}

	void String::operator=(const char* text)
	{
		set(text);
	}

	void String::operator=(const String& str)
	{
		set(str);
	}

	void String::operator+=(const char* text)
	{
		append(text);
	}

	void String::operator+=(const String& str)
	{
		append(str);
	}

	void String::operator+=(char c)
	{
		append(c);
	}

	String operator+(const String& str1, const String& str2)
	{
		String str = "";

		str.append(str1);
		str.append(str2);

		return str;
	}

	bool operator==(const String& str1, const String& str2)
	{
		if (str1.length() != str2.length()) return false;

		bool equals = true;

		for (size_t i = 0; i < str1.length(); i++) {
			equals = equals && str1[i] == str2[i];

			if (!equals) break;
		}

		return equals;
	}

	bool operator==(const String& str, const char* text)
	{
		if (str.length() != strlen(text)) return false;

		bool equals = true;

		for (size_t i = 0; i < str.length(); i++) {
			equals = equals && str[i] == text[i];

			if (!equals) break;
		}

		return equals;
	}

	std::ostream& operator<<(std::ostream& out, const String& str)
	{
		out << str.data();

		return out;
	}
}