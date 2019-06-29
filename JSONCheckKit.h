#pragma once
#include<string>
#include<stack>
class JSONCheckKit {
private:
	enum DataUnitTypeEnum { OBJECT, ARRAY };
	enum StatusEnum {
		INITIAL, NV_EMPTY, NV_NAME, NV_VALUE,\
		ARRAY_EMPTY, ARRAY_VALUE, NEXT_ELE, STRING,\
		NUMBER, BOOLSTR, NULLSTR, FINISH
	};
private:
	const std::string jstr;
	std::stack<char> sc;
	std::stack<DataUnitTypeEnum> ss;
	StatusEnum test;
	size_t index;
public:
	JSONCheckKit(const std::string& str = "")
		:jstr(str)
		,test(INITIAL)
		,index(0)
	{}

	bool JSONCheck();
private:
	bool INITIAL_check();
	bool NV_EMPTY_check();
	bool NV_NAME_check();
	bool NV_VALUE_check();
	bool ARRAY_EMPTY_check();
	bool ARRAY_VALUE_check();
	bool NEXT_ELE_check();
	bool STRING_check();
	bool NUMBER_check();
	bool BOOLSTR_check();
	bool NULLSTR_check();
	bool FINISH_check();
	bool is_special_char(const char c);
	bool escape_character_test();
};


