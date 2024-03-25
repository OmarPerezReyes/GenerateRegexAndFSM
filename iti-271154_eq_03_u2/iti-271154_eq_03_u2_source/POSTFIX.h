#include <iostream>
#include <stack>
#include <map>
#include <regex>

class POSTFIX {
public:
    std::string regex;
    std::string postfix;

    POSTFIX(std::string regex) : regex(regex) {
        this->postfix = shunt_yard(regex);
    }

    std::string get_postfix() {
        return this->postfix;
    }

private:
    std::string shunt_yard(std::string regex) {
        std::map<char, int> operators = {{'*', 5}, {'+', 4}, {'?', 3}, {'.', 2}, {'|', 1}};
        std::string postfix = "";
        std::stack<char> stack;

        // Replace character classes with alternations
        for (size_t i = 0; i < regex.length(); ++i) {
            char c = regex[i];
            if (c == '[') {
                size_t j = i + 1;
                while (regex[j] != ']') {
                    if (isalnum(regex[j]) && isalnum(regex[j + 1])) {
                        regex.insert(j + 1, "|");
                        ++j;
                    }
                    ++j;
                }
            }
        }

        // Replace hyphens with alternations
        regex = replace_all(regex, '-', '|');

   // Insert concatenation operators
for (size_t i = 0; i < regex.length() - 1; ++i) {
    if ((isalnum(regex[i]) || regex[i] == ')' || regex[i] == '*' || regex[i] == '+' || regex[i] == '?') &&
        (isalnum(regex[i + 1]) || regex[i + 1] == '(' || regex[i + 1] == '[')) {
        regex.insert(i + 1, ".");
        ++i; // Skip the inserted '.' character
    }
}

        // Shunting Yard algorithm
        for (char c : regex) {
            if (c == '(') {
                stack.push(c);
            } else if (c == ')') {
                while (!stack.empty() && stack.top() != '(') {
                    postfix += stack.top();
                    stack.pop();
                }
                if (!stack.empty()) stack.pop();
            } else if (operators.find(c) != operators.end()) {
                while (!stack.empty() && operators[c] <= operators[stack.top()]) {
                    postfix += stack.top();
                    stack.pop();
                }
                stack.push(c);
            } else {
                postfix += c;
            }
        }

        while (!stack.empty()) {
            postfix += stack.top();
            stack.pop();
        }

        return postfix;
    }

    std::string replace_all(std::string str, char from, char to) {
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] == from) {
                str[i] = to;
            }
        }
        return str;
    }
};

bool validateRegex(const std::string& regex) {
    try {
        std::regex re(regex);
    } catch (const std::regex_error&) {
        std::cout << "Invalid regular expression: " << regex << std::endl;
        return false;
    }
    return true;
}
