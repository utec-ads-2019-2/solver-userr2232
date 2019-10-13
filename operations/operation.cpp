#include "operation.h"
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <cmath>

std::map<char,int> precedence = {
    {'+', 0},
    {'-', 0},
    {'*', 1},
    {'/', 1},
    {'^', 2},
    {'~', 3}
};

Implementation* Implementation::buildFromEquation(std::string equation) {
    equation = clean(equation);
    return new Implementation(equation);
}

float Implementation::operate() {
    shuntingYard();
    std::string p = processPostFix();
    double res = std::stod(p);
    return res;
}

std::string Implementation::operate(std::string operation, std::string operand1, std::string operand2) {
    char op = operation[0];
    switch(op) {
        case '+':
            return to_string(std::stod(operand1) + std::stod(operand2));
        case '-':
            return to_string(std::stod(operand1) - std::stod(operand2));
        case '*':
            return to_string(std::stod(operand1) * std::stod(operand2));
        case '^':
            return to_string(std::pow(std::stoi(operand1), std::stoi(operand2)));
        case '/':
            return to_string(std::stod(operand1) / std::stod(operand2));
    }
}

std::string Implementation::processPostFix() {
    std::stack<string> opStack;
    for(std::string s : expression) {
        if(!(s == "+" || s == "*" || s == "-" || s == "^" || s == "/")) {
            opStack.push(s);
        } else {
            string op1 = opStack.top();
            opStack.pop();
            string op2 = opStack.top();
            opStack.pop();
            string res = operate(s, op2, op1);
            opStack.push(res);
        }
    }
    return opStack.top();
}

bool greaterPrecedenceOrEqualButLeftAssociative(char op1, char op2) {
    if(precedence[op1] > precedence[op2]) return true;
    else if(precedence[op1] < precedence[op2]) return false;
    return true;
}

void Implementation::shuntingYard() {
    std::vector<std::string> output;
    std::string number = "";
    std::stack<char> operatorStack;
    for(int i = 0; i < equation.length(); i++) {
        char c = equation[i];
        if(c >= '0' && c <= '9' || c == '.'){
            number.push_back(c);
        }
        else if(c == '(') {
            if(number != "") output.push_back(number);
            number = "";
            operatorStack.push(c);
        }
        else if(c == ')') {
            if(number != "") output.push_back(number);
            number = "";
            while(!operatorStack.empty()){
                if(operatorStack.top() != '(') {
                    std::string s(1, operatorStack.top());
                    output.push_back(s);
                    operatorStack.pop();
                }
                else {
                    operatorStack.pop();
                    break;
                }
            }
        }
        else if(c >= 'a' && c <= 'z') {
            if(variables.count(c) == 0) {
                cout << "Por favor, ingrese " << c << ": ";
                string var;
                cin >> var;
                variables[c] = var;
            }
            equation.replace(i, 1, variables[c]);
            i--;
        }
        else {
            if(number != "") output.push_back(number);
            number = "";
            if(c == '~') {
                    output.push_back("-1");
                    c = '*';
                }
            while(!operatorStack.empty() && greaterPrecedenceOrEqualButLeftAssociative(operatorStack.top(), c) && operatorStack.top() != '(') {
                std::string s(1, operatorStack.top());
                output.push_back(s);
                operatorStack.pop();
            }
            operatorStack.push(c);
        }
    }
    if(number != "") output.push_back(number);
    while(!operatorStack.empty()) {
        std::string s(1, operatorStack.top());
        output.push_back(s);
        operatorStack.pop();
    }
    expression = output;
}

std::string Implementation::clean(std::string equation) {
    cout << "equation " << equation << endl;
    string new_equation = "";
    int countMinus = 0;
    int countPlus = 0;
    for(char s : equation) {
        if(s == '+' || s == '-'){
            if(s == '-') countMinus++;
            else countPlus++;
        }
        else if(countMinus != 0 || countPlus != 0) {
            new_equation.push_back(((countMinus%2) == 0) ? '+' : '-');
            countMinus = 0; countPlus = 0;
            new_equation.push_back(s);
        } else {
            new_equation.push_back(s);
        }
    }
    return clean2(new_equation);
}

std::string Implementation::clean2(std::string equation) {
    bool lastCharRead = true;
    std::string new_eq = "";
    for(char c : equation) {
        if(c >= '0' && c <= '9' || c <= 'z' && c >= 'a') lastCharRead = false;
        else if (c == '*' || c == '/' || c == '(') lastCharRead = true;
        if(lastCharRead && c == '-') c = '~';
        new_eq.push_back(c);
    }
    return new_eq;
}