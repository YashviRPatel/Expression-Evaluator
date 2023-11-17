#include<iostream>
#include<string>
#include <vector>
#include <sstream>
#include<algorithm>
#include<map>
#include<regex>
#include<stack>
using namespace std;


bool checkSpace(char c)
{
	return c == ' ';
}
bool checkogAssignment(string ogString, string tempAssignment)
{


	string ogAssignment;
	auto ogIndex = ogString.find(';');
	if (ogIndex != string::npos)
	{
		ogAssignment = ogString.substr(ogIndex + 1);
	}
	// regex onlySpaces("([\\s])*");
	// if (regex_match(ogAssignment, onlySpaces))
	// 	return true;

	stringstream ss(ogAssignment);
	vector<string> assign;
	string tempAssign;

	// if ( ogAssignment.find(',') == string::npos)
	// 	return true;

	if (tempAssignment == "")
		return true;

	while (getline(ss, tempAssign, ',') )
	{
		assign.push_back(tempAssign);
	}

	for (string eachAssign : assign)
	{
		auto varKey = eachAssign.find(':');
		if (varKey != string::npos)
		{
			string varNames = eachAssign.substr(0, varKey);
			// regex varCheck("([a-zA-Z])+([a-zA-Z0-9])*[\\s]+([a-zA-Z])+([a-zA-Z0-9])*");
			// if (regex_match(varNames, varCheck))
			// {
			// 	return false;
			// }


			//removing leading spaces
			varNames = regex_replace(varNames, regex("^ +"), "");

			//removing trailing spaces
			varNames = regex_replace(varNames, regex(" +$"), "");

			for (int i = 0; i < varNames.length(); i++)
			{
				// cout << varNames[i] << endl;
				if (varNames[i] == ' ')
				{
					return false;
				}
			}


		}
		else
		{
			return false;
		}
	}
	return true;
}

bool checkAssignment(vector<string> tempSub, string tempAssignment, string ogString)
{
	//cout << tempSub.size() << endl;
	for (string eachTempSub : tempSub)
	{
		auto index = eachTempSub.find(':');
		if (index != string :: npos)
		{
			string left = eachTempSub.substr(0 , index);

			if (left == "" || index + 1 >= eachTempSub.length()) return false;

			string right = eachTempSub.substr(index + 1);
			if (left.length() > 10 || right.length() != 1)
			{
				return false;
			}
			else if (!((left[0] >= 65 && left[0] <= 90 ) || (left[0] >= 97 && left[0] <= 122)))
			{
				return false;
			}

			else if (!regex_match(left, regex("^[a-zA-Z_$][a-zA-Z_$0-9]*$")))
			{
				return false;
			}
			else if (right[0] != '0' && right[0] != '1')
			{
				return false;
			}
		}
		// If colon not found
		else
		{
			return false;
		}
	}
	return true && checkogAssignment(ogString, tempAssignment);
	//&&(tempAssignment.back() != ',')
}

//CHECK THE LENGTH OF FORMULA STRING
bool checkFormulaVarSize(string tFormula)
{
	int varSize = 0;
	for (int i = 0; i < tFormula.length(); i++)
	{

		if (tFormula[i] != '(' && tFormula[i] != ')' &&
		        tFormula[i] != '-' && tFormula[i] != '*' && tFormula[i] != '+')
		{
			varSize++;
			if (varSize > 10)
			{
				//cout << "Error: invalid varSize > 10" << endl;
				return false;
				// break;
			}
		}

		else
		{
			varSize = 0;
		}


	}
	return true;
}


// FOR HANDLING TEST CASES SUCH AS A B; AB:1
bool checkSpaces(string ogString)
{

	int hasWord = 0;
	int countOperand = 0;
	int countOperator = 0;

	for (int i = 0; i < ogString.length(); i++)
	{
		if (ogString[i] == '*' || ogString[i] == '+')
		{
			countOperator++;
		}
		if (isalnum(ogString[i]))
		{
			hasWord = 1 ;
		}
		else
		{
			if (hasWord)
			{
				countOperand++;
				hasWord = 0;
			}
		}
	}

	if (hasWord)
	{
		countOperand++;
	}
	return countOperator == countOperand - 1;
}

bool checkFormula(string tempFormula, string ogString)
{
	//cout << tempFormula << endl;
	stack<char> matchParam;
	int n = tempFormula.length();
	int isAlphabetPresent = 0;
	int operandCount = 0;
	for (int i = 0; i < n; i++)
	{
		if (isalpha(tempFormula[i])) isAlphabetPresent = 1;

		if (tempFormula[i] == '(')
		{
			matchParam.push('(');
			if (i + 1 < n && tempFormula[i + 1] == ')')
			{
				// cout << "a" << endl;
				return false;
			}
			if (i - 1 >= 0 && (tempFormula[i - 1] != '*' && tempFormula[i - 1] != '+' && tempFormula[i - 1] != '-' && tempFormula[i - 1] != '('))
			{
				return false;
			}
		}

		else if (tempFormula[i] == '0' || tempFormula[i] == '1')
		{
			operandCount++;
			if (i + 1 < n && (tempFormula[i + 1] != ')' && tempFormula[i + 1] != '*' && tempFormula[i + 1] != '+'))
			{
				// cout << "b" << endl;
				return false;
			}
		}
		else if (tempFormula[i] == '-')
		{
			if (i + 1 < n && (tempFormula[i + 1] != '0' && tempFormula[i + 1] != '1' && tempFormula[i + 1] != '(') && tempFormula[i] != '-' && !isalnum(tempFormula[i + 1]))
			{
				//cout << tempFormula[i] << " " << tempFormula[i + 1] << endl;
				// cout << "c" << endl;
				return false;
			}
			if (i + 1 < n && (tempFormula[i + 1] == '+' || tempFormula[i + 1] == '*' || tempFormula[i + 1] == ')' ))
			{
				return false;
			}
		}
		else if (tempFormula[i] == ')')
		{
			// cout << "d" << endl;
			if (matchParam.empty()) return false;
			else matchParam.pop();
		}
		else if (tempFormula[i] == '*' || tempFormula[i] == '+')
		{
			if (i - 1 >= 0 && tempFormula[i - 1] != '0' && tempFormula[i - 1] != '1' && tempFormula[i - 1] != ')' && !isalnum(tempFormula[i + 1]))
			{
				return false;
			}
			if (
			    (i + 1 < n) && (tempFormula[i + 1] != '-' && tempFormula[i + 1] != '0' && tempFormula[i + 1] != '1' && tempFormula[i + 1] != '(' && !isalnum(tempFormula[i + 1])))
			{
				// cout << "e" << endl;
				//cout << tempFormula[i] << " " << tempFormula[i + 1] << endl;
				return false;

			}
		}
		else if (isdigit(tempFormula[i]))
		{
			return false;
		}
		else {
			if (!isalnum(tempFormula[i]))
			{
				return false;
			}
		}
	}
	if (!matchParam.empty()) return false;

	if (tempFormula[n - 1] == '-' || tempFormula[n - 1] == '*' || tempFormula[n - 1] == '+' || tempFormula[0] == '+' || tempFormula[0] == '*')
	{
		return false;
	}

	return true && checkFormulaVarSize(tempFormula) && (operandCount || isAlphabetPresent) && checkSpaces(ogString);
}


//INFIX TO POSTFIX FORMULA CONVERSION

int priority(char c)
{
	if (c == '-')
		return 3;
	else if (c == '*')
		return 2;
	else if (c == '+')
		return 1;
	else
		return -1;
}


string postfixFormula (string infixFormula)
{
	stack<char> st; // For stack operations, we are using
	// C++ built in stack
	string result;

	for (int i = 0; i < infixFormula.length(); i++) {
		char c = infixFormula[i];

		// If the scanned character is
		// an operand, add it to output string.
		if (c == '0' || c == '1')
			result += c;

		// If the scanned character is an
		// �(�, push it to the stack.
		else if (c == '(')
			st.push('(');

		// If the scanned character is an �)�,
		// pop and to output string from the stack
		// until an �(� is encountered.
		else if (c == ')') {
			while (st.top() != '(') {
				result += st.top();
				st.pop();
			}
			st.pop();
		}

		// If an operator is scanned
		else {
			while (!st.empty()
			        && priority(infixFormula[i]) <= priority(st.top())) {
				result += st.top();
				st.pop();
			}
			st.push(c);
		}
	}

	// Pop all the remaining elements from the stack
	while (!st.empty()) {
		result += st.top();
		st.pop();
	}

	// cout << result << endl;
	return result;
}

//EVALUATION OF POSTFIX EXPRESSION

int postfixEvaluator(string postfixExp)
{
	stack<int> st;
	//cout << postfixExp << endl;
	for (int i = 0 ; i < postfixExp.length(); i++)
	{

		if (postfixExp[i] == '0' || postfixExp[i] == '1')
		{
			st.push(postfixExp[i] - '0');
		}

		else if (postfixExp[i] == '-')
		{
			int topValue = st.top();
			topValue = !topValue;
			st.pop();
			st.push(topValue);
		}

		else if (postfixExp[i] == '*')
		{
			int op1 = st.top();
			st.pop();
			int op2 = st.top();
			st.pop();
			int opAnswer = op1 && op2;
			st.push(opAnswer);
		}

		else if (postfixExp[i] == '+')
		{
			int op1 = st.top();
			st.pop();
			int op2 = st.top();
			st.pop();
			int opAnswer = op1 || op2;
			st.push(opAnswer);
		}
	}
	return st.top();


}

//HANDLING TEST CASES SUCH AS MULTIPLE NEGATION
string multipleNegation(string nFormula)
{
	string newFormula = "";
	int countMinus = 0;
	for (int i = 0; i < nFormula.length(); i++)
	{
		if (nFormula[i] != '-')
		{
			if (countMinus > 0)
			{
				if (countMinus % 2 != 0)
				{
					newFormula += "-";
				}
			}
			newFormula += nFormula[i];
			countMinus = 0;
		}
		else
		{
			countMinus++;
		}
	}
	return newFormula;
}




int main()
{
	// freopen("yinput.txt", "r", stdin);
	// freopen("youtput.txt", "w", stdout);
	string s;
	while (getline(cin, s))
	{


		string ogString = s;
		string ogStringCopy = s;

		//...Remove all spaces from input string...


		s.erase(remove_if(s.begin(), s.end(), checkSpace), s.end());

		//... Seperating Formula and Assignment...
		string formula, assignment;

		auto index = s.find(';');

		if (index != string::npos)
		{
			formula = s.substr(0, index);
			if (formula == "")
			{
				cout << "Error: invalid input" << endl;
			}

			else
			{
				//cout << "Formula " << formula << endl;

				assignment = s.substr(index + 1);
				//cout << assignment << endl;

				//...Assignment Map...
				stringstream ss(assignment);
				vector<string> sub;
				string temp;
				while (getline(ss, temp, ',') )
				{
					sub.push_back(temp);
				}
				// cout << "Assignment " << "{";
				// for (int i = 0; i < sub.size(); i++)
				// {
				// 	cout << sub[i];
				// 	if (i != sub.size() - 1) cout << ", ";
				// }

				// cout << "}";
				// cout << endl;

				if (!checkAssignment(sub, assignment, ogStringCopy))
				{
					// err.push("Error: invalid assignment");
					//cout << "Error: invalid input invalid assignment" << endl;
					cout << "Error: invalid input" << endl;
				}

				else
				{


					//..Actual assignment map...
					map < string , int > mp;
					int flag = 0;
					for (string eachSub : sub)
					{

						auto keyvalue = eachSub.find(':');

						if (keyvalue != string::npos)
						{

							//seperating 2 parts from sub and converting second part into int
							string key = eachSub.substr(0, keyvalue);

							// if (key == "") continue;

							// if (eachSub.length() != 0 && eachSub[eachSub.length() - 1] != '1' || eachSub[eachSub.length() - 1] != '0') continue;


							int value = int(eachSub[eachSub.length() - 1] - '0');


							//mapping value to its key by checking till end of map if same key not found
							if (mp.find(key) == mp.end())
							{
								mp[key] = value;
							}

							//if same key found again while traversing map then check its corresponding value, if value is same do nothing else throw an error of "contradicting assignment".
							else
							{
								if (mp[key] == value)
								{
									//do nothing;
								}
								else
								{
									cout << "Error: contradicting assignment" << endl;
									// err.push("Error: contradicting assignment");
									flag = 1;
									break;

								}

							}

						}


					}

					//...Substitute assignment into formula

					if (!flag)
					{
						ogString = ogString.substr(0, ogString.find(';'));
						//int highLevelFlag = 1;

						//New vector for seperated opeands for handling cases like a+aa;
						vector<string> seperateOperands;
						string myOperands = "";
						for (int i = 0; i < formula.length(); i++)
						{
							if (isalnum(formula[i]))
							{
								myOperands += to_string(formula[i]);
							}
							else
							{
								if (myOperands != "")
								{
									seperateOperands.push_back(myOperands);
								}
								myOperands = "";
							}
						}

						if (myOperands != "")
						{
							seperateOperands.push_back(myOperands);
						}

						// for (auto eachSO : seperateOperands)
						// {
						// 	cout << eachSO << endl;
						// }


						//New map for assigning values to seperated operands
						map <string, int>:: iterator it = mp.begin();
						while (it != mp.end())
						{
							string key = it->first;
							int value = it->second;

							// cout << key;


							// if (findInOG != string::npos)
							// {
							// 	formula = regex_replace(formula, regex(key), to_string(value));
							// }
							auto checkOperandsInd = 0;
							while (1)
							{
								auto findInOG = ogString.find(key);
								int checky = 0;
								checkOperandsInd = formula.find(key, checkOperandsInd);
								if (checkOperandsInd != string::npos)
								{
									if (checkOperandsInd - 1 >= 0 && isalnum(formula[checkOperandsInd - 1]))
									{
										checky = 1;
									}
									if (checkOperandsInd + key.length() < formula.length() && isalnum(formula[checkOperandsInd + key.length()]) )
									{
										checky = 1;
									}
									if (!checky && findInOG != string::npos)
									{
										formula.replace(checkOperandsInd, key.length(), to_string(value));
									}
								}
								else
								{
									break;
								}
								checkOperandsInd += key.length();
							}


							// else
							// {
							// 	cout << "Error: invlid input spaces" << endl;
							// 	highLevelFlag = 0;
							// 	break;
							// }

							it++;
						}



						if (!checkFormula(formula, ogString))
						{
							// err.push("Error: invalid formula");
							// cout << "Error: invalid input invalid formula" << endl;
							cout << "Error: invalid input" << endl;
						}

						else
						{

							for (int i = 0; i < formula.length(); i++)
							{
								if (formula[i] != '0' && formula[i] != '1' && formula[i] != '*' && formula[i] != '(' && formula[i] != ')' && formula[i] != '+' && formula[i] != '-')
								{
									cout << "Error: incomplete assignment" << endl;
									// err.push("Error: incomplete assignment");
									flag = 2;
									break;
								}

							}


							if (!checkAssignment(sub, assignment, ogStringCopy))
							{
								// err.push("Error: invalid assignment");
								if (flag != 2)
								{
									//cout << "Error: invalid input invalid assignment" << endl;
									cout << "Error: invalid input" << endl;
								}

								flag = 3;
							}


							if (flag != 2 && flag != 3)
							{
								//cout << "assigned formula " << formula << endl;


								//For multiple negation sign. If odd number of negation sign then change else remain same.

								formula = multipleNegation(formula);


								string postfixExpression = postfixFormula(formula);
								//cout << "Postfix Formula " << postfixExpression << endl;
								int finalAnswer = postfixEvaluator(postfixExpression);
								// cout << "Final Evaluated Answer " << finalAnswer << endl;
								cout << finalAnswer << endl;

							}
						}

					}
				}
			}

		}

		else
		{
			cout << "Error: invalid input" << endl;
		}

	}
	// system("pause");
	return 0;
}

