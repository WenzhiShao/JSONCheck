#include<string>
#include<stack>
#include<iostream>
using namespace std;
enum status { namev_alue, value };
enum type { is_name, is_value, is_free };
void deleteAllMark(string& s, const string& mark)
{
	size_t nSize = mark.size();
	while (1)
	{
		size_t pos = s.find(mark);    //  尤其是这里
		if (pos == string::npos)
		{
			return;
		}

		s.erase(pos, nSize);
	}
}
bool JSONCheck(string& jstr)
{
	deleteAllMark(jstr, " ");
	if (jstr.empty())
		return false;
	stack<char> s1;
	stack<status> s2;
	type test = is_free;
	if (jstr[0] == '{')
	{
		s1.push(jstr[0]);
		s2.push(namev_alue);
		test = is_name;
	}
	else if (jstr[0] == '[')
	{
		s1.push(jstr[0]);
		s2.push(value);
		is_free;
	}
	else
		return false;

	for (int i = 1;i < jstr.size();i++)
	{
		if (test == is_name)
		{
			if (jstr[i] == '"')
			{
				if (s1.top() == '"')
				{
					s1.pop();
					if (i + 1 < jstr.size() && jstr[i + 1] == ':')
					{
						test = is_value;
						i++;
					}
					else
						return false;
				}
				else if (s1.top() != '"')
				{
					s1.push(jstr[i]);
				}
			}
		}
		else if (test == is_value)
		{
			if (jstr[i] == ',')
			{
				if (jstr[i - 1] == ':')
					return false;
				else
					test = is_name;
			}
				
			else if (jstr[i] == '"')
			{
				if (s1.top() == '"')
				{
					s1.pop();
				}
				else s1.push(jstr[i]);
			}

			else if (jstr[i] == '}')
			{
				if (s1.top() == '{')
				{
					if (s2.top() == namev_alue)
					{
						s1.pop();
						s2.pop();
						switch (s2.top())
						{
						case namev_alue:
							test = is_name;
							break;
						case value:
							test = is_free;
							break;
						}
					}
					else
						return false;
				}
				else
					return false;
			}
			else if (jstr[i] == '{')
			{
				s1.push(jstr[i]);
				s2.push(namev_alue);
				test = is_name;
			}

			else if (jstr[i] == '[')
			{
				s1.push(jstr[i]);
				s2.push(value);
				test = is_free;
			}

			else if (jstr[i] == ']')
			{
				if (s1.top() == '[')
				{
					if (s2.top() == value)
					{
						s1.pop();
						s2.pop();
						switch (s2.top())
						{
						case namev_alue:
							test = is_name;
							break;
						case value:
							test = is_free;
							break;
						}
					}
					else
						return false;
				}
				else
					return false;
			}



		}
		else if (test == is_free)
		{
			if (jstr[i] == '"')
			{
				if (s1.top() == '"')
					s1.pop();
				else
					s1.push(jstr[i]);
			}

			if (jstr[i] == '{')
			{
				s1.push(jstr[i]);
				s2.push(namev_alue);
				test = is_name;
			}
			if (jstr[i] == '[')
			{
				s1.push(jstr[i]);
				s2.push(value);
				test = is_free;
			}
			if (jstr[i] == ',');
		}

	}

	if (s1.empty() && s2.empty())
		return true;
	else
		return false;
}

int main(void)
{
	string json;
	getline(cin, json);
	/*cout << json;*/
	JSONCheck(json);
	return 0;
}