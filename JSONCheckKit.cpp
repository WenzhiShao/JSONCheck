#include<cctype>
#include"JSONCheckKit.h"
bool JSONCheckKit::JSONCheck() {
	while (index < jstr.size()) {
		switch (test) {
		case INITIAL: {
			if (INITIAL_check())
				break;
			else
				return false;
		}
		case NV_EMPTY: {
			if (NV_EMPTY_check())
				break;
			else
				return false;
		}
		case NV_NAME: {
			if (NV_NAME_check())
				break;
			else
				return false;
		}
		case NV_VALUE: {
			if (NV_VALUE_check())
				break;
			else
				return false;

		}
		case ARRAY_EMPTY: {
			if (ARRAY_EMPTY_check())
				break;
			else
				return false;

		}
		case ARRAY_VALUE: {
			if (ARRAY_VALUE_check())
				break;
			else
				return false;

		}
		case NEXT_ELE: {
			if (NEXT_ELE_check())
				break;
			else
				return false;
		}
		case STRING: {
			if (STRING_check())
				break;
			else
				return false;
		}
		case NUMBER: {
			if (NUMBER_check())
				break;
			else
				return false;
		}
		case NULLSTR: {
			if (NULLSTR_check())
				break;
			else
				return false;
		}
		case BOOLSTR: {
			if (BOOLSTR_check())
				break;
			else
				return false;
		}
		case FINISH: {
			if (FINISH_check())
				break;
			return false;
		}
		}
	}

	if (sc.empty() && ss.empty())
		return true;

	return false;
}

bool JSONCheckKit::INITIAL_check()
{
	while (index < jstr.size() && isspace(jstr[index])) {
		index++;
	}

	if (index >= jstr.size()) {
		return false;
	}

	if (jstr[index] == '{') {
		sc.push('{');
		ss.push(OBJECT);
		test = NV_EMPTY;
	} else if (jstr[index] == '[') {
		sc.push('[');
		ss.push(ARRAY);
		test = ARRAY_EMPTY;
	} else {
		return false;
	}

	index++;
	return true;
}

bool JSONCheckKit::NV_EMPTY_check()
{
	while (index < jstr.size() && isspace(jstr[index])) {
		index++;
	}

	if (index >= jstr.size()) {
		return false;
	}

	if (jstr[index] == '"') {
		test = NV_NAME;
		sc.push('"');
		index++;
		return true;
	} else if (jstr[index] == '}') {
		if (sc.top() == '{' && ss.top() == OBJECT) {
			sc.pop();
			ss.pop();
			index++;
			if (ss.empty() && sc.empty()) {
				test = FINISH;
			}
			else {
				test = NEXT_ELE;
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool JSONCheckKit::NV_NAME_check()
{

	while (index < jstr.size() && jstr[index] != '"') {
		if (jstr[index] == '\\') {
			if (!escape_character_test()) {
				return false;
			}
		}
		index++;
	}

	if (index >= jstr.size()) {
		return false;
	}

	if (jstr[index] == '"') {
		if (sc.top() == '"') {
			sc.pop();
			if (index < jstr.size()) {
				index++;
			} else {
				return false;
			}

			while (index <jstr.size() && jstr[index] != ':') {
				if (isspace(jstr[index])) {
					index++;
				}
				else {
					return false;
				}
			}

			if (index < jstr.size()) {
				index++;
			} else {
				return false;
			}

			while (isspace(jstr[index])) {
				index++;
			}

			if (index < jstr.size()) {
				test = NV_VALUE;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	} else {
		return false;
	}

	return true;
}

bool JSONCheckKit::NV_VALUE_check()
{

	if (jstr[index] == '"') {
		test = STRING;
	} else if ((jstr[index] >= '0' && jstr[index] <= '9') || jstr[index] == '-') {
		test = NUMBER;
	} else if (jstr[index] == 'n') {
		test = NULLSTR;
	} else if (jstr[index] == 't' || jstr[index] == 'f') {
		test = BOOLSTR;
	} else if (jstr[index] == '{') {
		test = NV_EMPTY;
		sc.push('{');
		ss.push(OBJECT);
		index++;
	} else if (jstr[index] == '[') {
		test = ARRAY_EMPTY;
		sc.push('[');
		ss.push(ARRAY);
		index++;
	} else {
		return false;
	}

	return true;
}

bool JSONCheckKit::ARRAY_EMPTY_check() {
	while (index < jstr.size() && isspace(jstr[index])) {
		index++;
	}

	if (index >= jstr.size()) {
		return false;
	}

	if (jstr[index] == ']') {
		if (sc.top() == '[' && ss.top() == ARRAY) {
			sc.pop();
			ss.pop();
			index++;
			if (ss.empty() && sc.empty()) {
				test = FINISH;
			} else {
				test = NEXT_ELE;
			}
		} else {
			return false;
		}
	} else {
		test = ARRAY_VALUE;
	}

	return true;
}

bool JSONCheckKit::ARRAY_VALUE_check()
{
	if (jstr[index] == '"') {
		test = STRING;
	} else if (jstr[index] >= '0' && jstr[index] <= '9' || jstr[index] == '-') {
		test = NUMBER;
	} else if (jstr[index] == 'n') {
		test = NULLSTR;
	} else if (jstr[index] == 't' || jstr[index] == 'f') {
		test = BOOLSTR;
	} else if (jstr[index] == '{') {
		test = NV_EMPTY;
		sc.push('{');
		ss.push(OBJECT);
		index++;
	} else if (jstr[index] == '[') {
		test = ARRAY_EMPTY;
		sc.push('[');
		ss.push(ARRAY);
		index++;
	} else {
		return false;
	}

	return true;
}

bool JSONCheckKit::NEXT_ELE_check()
{
	while (index < jstr.size() && isspace(jstr[index]))
		index++;

	if (index >= jstr.size()) {
		return false;
	}

	if (jstr[index] == ',') {
		if (ss.empty()) {
			return false;
		} else {
			switch (ss.top()) {
			case ARRAY:
				test = ARRAY_VALUE;
				break;
			case OBJECT:
				test = NV_VALUE;
				break;
			}
		}
	} else if (jstr[index] == ']') {
		if (sc.top() == '[' && ss.top() == ARRAY) {
			sc.pop();
			ss.pop();

			if (sc.empty() && ss.empty()) {
				test = FINISH;
			}
		} else {
			return false;
		}
	} else if (jstr[index] == '}') {
		if (sc.top() == '{' && ss.top() == OBJECT) {
			sc.pop();
			ss.pop();
			if (sc.empty() && ss.empty())
				test = FINISH;
		} else {
			return false;
		}
	} else {
		return false;
	}

	index++;
	return true;
}

bool JSONCheckKit::is_special_char(const char c)
{
	switch (c) {
	case ',':
	case '}':
	case ']':
		return true;
	default:
		return false;
	}
}

bool JSONCheckKit::STRING_check() {
	sc.push('"');
	if (++index == jstr.size())
		return false;

	while (index < jstr.size() && jstr[index] != '"'){
		if (jstr[index] == '\\'){
			if (!escape_character_test()) {
				return false;
			}
		}
		index++;
	}

	if (index >= jstr.size()) {
		return false;
	}
	
	sc.pop();
	index++;
	test = NEXT_ELE;
	return true;
}

bool JSONCheckKit::NUMBER_check()
{
	if (jstr[index] == '-') {

		if (++index >= jstr.size()) {
			return false;
		}
		if (!(jstr[index] >= '0' && jstr[index] <= '9')) {
			return false;
		}
	}
	if (jstr[index] == '0') {
		if (jstr[index + 1] != '.')
			return false;
	}

	while (index < jstr.size()) {
		if (jstr[index] == ' ' || is_special_char(jstr[index])) {
			test = NEXT_ELE;
			return true;
		}
		else if ((jstr[index] >= '0' && jstr[index] <= '9')) {
			index++;
		}
		else if (jstr[index] == '.') {
			index++;
			if (jstr[index] == '\0' || !(jstr[index] >= '0' && jstr[index] <= '9')) {
				return false;
			}
		}
		else if (jstr[index] == 'e' || jstr[index] == 'E') {
			index++;
			if (jstr[index] == '\0') {
				return false;
			} else if (jstr[index] == '-' || jstr[index] == '+') {
				index++;
			} else if ((jstr[index] >= '0' && jstr[index] <= '9')) {
			} else {
				return false;
			}

			while (index < jstr.size()) {
				if (is_special_char(jstr[index])) {
					test = NEXT_ELE;
					return true;
				}
				else if (jstr[index] >= '0' && jstr[index] <= '9') {
					index++;
				}
				else {
					return false;
				}
			}
		}
		else {
			return false;
		}
	}

	test = NEXT_ELE;
	return true;
}

bool JSONCheckKit::BOOLSTR_check()
{
	std::string tmp_str;
	if (jstr[index] == 't') {
		for (size_t i = 0;i < 4;i++) {
			if (index < jstr.size()) {
				tmp_str += jstr[index], index++;
			} else {
				return false;
			}
		}

		if (tmp_str == "true"){
			test = NEXT_ELE;
			return true;
		} else {
			return false;
		}
	} else {
		for (size_t i = 0;i < 5; i++) {
			if (index < jstr.size())
				tmp_str += jstr[index], index++;
			else
				return false;
		}

		if (tmp_str == "false") {
			test = NEXT_ELE;
			return true;
		} else {
			return false;
		}
	}
}

bool JSONCheckKit::NULLSTR_check()
{
	std::string tmp_str;
	for (size_t i = 0;i < 4;i++) {
		if ( index < jstr.size() ) {
			tmp_str += jstr[index];
			index++;
		} else {
			return false;
		}
	}

	if (tmp_str == "null") {
		test = NEXT_ELE;
		return true;
	} else {
		return false;
	}
}

bool JSONCheckKit::escape_character_test()
{
	if (index + 1 == jstr.size()) {
		return false;
	}
	switch (jstr[index++]) {
		case '\\':
		case '/':
		case 'b':
		case 'f':
		case 't':
		case 'n':
		case 'r':
		case '"': {
			if (index + 1 == jstr.size())
				return false;
			return true;
		}
		case 'u': {
			for (size_t i = 0; i < 4; i++) {
				index++;
				if (index == jstr.size())
					return false;
			}
			return true;
		}
		default:
			return false;
	}
}

bool JSONCheckKit::FINISH_check()
{
	while (index < jstr.size()) {
		if (isspace(jstr[index])) {
			index++;
		} else {
			return false;
		}
	}

	return true;
}