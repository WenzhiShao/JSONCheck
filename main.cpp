#include<string>
#include<stack>
#include<iostream>
using namespace std;
enum status { unorder, order };
enum type { nv_empty, nv_name, nv_value, ordered_value };
void deleteAllMark(string& s, const string& mark)
{
	size_t nSize = mark.size();
	while (1)
	{
		size_t pos = s.find(mark);    
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
	type test;
	if (jstr[0] == '{')
	{
		s1.push(jstr[0]);
		s2.push(unorder);
		test = nv_empty;
	}
	else if (jstr[0] == '[')
	{
		s1.push(jstr[0]);
		s2.push(order);
		test = ordered_value;
	}
	else
		return false;

	for (size_t i = 1;i < jstr.size();i++)
	{
		if (test == nv_empty)
		{
			if (jstr[i] == '"' && s2.top() == unorder)
				test = nv_name;
			else if (jstr[i] == '}')
			{
				if (s1.top() == '{')
				{
					s1.pop();
					s2.pop();
					if (!s2.empty())
					{
						switch (s2.top())
						{
						case unorder:
							test = nv_name;
							break;
						case order:
							test = ordered_value;
							break;
						}
					}
					else
						break;
				}
				else
					return false;
			}
			else
				return false;
		}
		if (test == nv_name)
		{
			if (jstr[i] == '"')
			{
				if (s1.top() == '"')
				{
					s1.pop();
					if (i + 1 < jstr.size() && jstr[i + 1] == ':')
					{
						test = nv_value;
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
			else if (s1.top() != '"')
				return false;
			//if (jstr[i] == '}')
			//{
			//	if (s1.top() == '{')
			//	{
			//		s1.pop();
			//		s2.pop();
			//		if (!s2.empty())
			//		{
			//			switch (s2.top())
			//			{
			//			case unorder:
			//				test = nv_name;
			//				break;
			//			case order:
			//				test = ordered_value;
			//				break;
			//			}
			//		}
			//		else
			//			break;
			//	}
			//	else
			//		return false;
			//}
		}
		else if (test == nv_value)
		{
			if (jstr[i] == ',')
			{
				if (jstr[i - 1] == ':')
					return false;
				else
					test = nv_name;
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
					if (s2.top() == unorder)
					{
						s1.pop();
						s2.pop();
						if (!s2.empty())
						{
							switch (s2.top())
							{
							case unorder:
								test = nv_name;
								break;
							case order:
								test = ordered_value;
								break;
							}
						}
						else
							break;
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
				s2.push(unorder);
				test = nv_name;
			}

			else if (jstr[i] == '[')
			{
				s1.push(jstr[i]);
				s2.push(order);
				test = ordered_value;
			}

			else if (jstr[i] == ']')
			{
				if (s1.top() == '[')
				{
					if (s2.top() == order)
					{
						s1.pop();
						s2.pop();
						if (!s2.empty())
						{
							switch (s2.top())
							{
							case unorder:
								test = nv_name;
								break;
							case order:
								test = ordered_value;
								break;
							}
						}
						else
							break;
					}
					else
						return false;
				}
				else
					return false;
			}



		}
		else if (test == ordered_value)
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
				s2.push(unorder);
				test = nv_empty;
			}
			if (jstr[i] == '[')
			{
				s1.push(jstr[i]);
				s2.push(order);
				test = ordered_value;
			}

			if (jstr[i] == ']')
			{
				if (s1.top() == '[')
				{
					s1.pop();
					s2.pop();
					if (!s2.empty())
					{
						switch (s2.top())
						{
						case unorder:
							test = nv_name;
							break;
						case order:
							test = ordered_value;
							break;
						}
					}
					else
						break;
				}
				else
					return false;
			}
			if (jstr[i] == ',')
			{
				if (i + 1 < jstr.size() && jstr[i + 1] != ']')
					continue;
				else
					return false;
			}
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