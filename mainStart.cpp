#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
using namespace std;

ofstream fout ("out.txt");
map<string,int> keys;
vector<string> arr = {"$","@","#","for","main","if","else","return","(",")","{","}",";","+","-","*","/","%","<",">","==","=","++","--","void"};
map<string,string> toktyp;

// map for keywords
void mapKeywords(){
	for(int i=0;i<arr.size();i++){
		keys[arr[i]] = i+100;
	}
}


// token type for keywords eg. while is while, 42 is int_literal, "hello" is string literal, < is relational_operator
void setTokenTypes(){
	// TO-DO, map lexeme to particular token type
	toktyp["$"] = "Integer type declarator";
	toktyp["@"] = "string type declarator";
	toktyp["for"] = "loop construct";
	toktyp["if"] = "conditional construct";
	toktyp["else"] = "conditional construct";
	toktyp["("] = "parentheses begin";
	toktyp[")"] = "parentheses end";
	toktyp["{"] = "braces begin";
	toktyp["}"] = "braces end";
	toktyp["+"] = "arithmetic operator - addition";
	toktyp["-"] = "arithmetic operator - subtraction";
	toktyp["/"] = "arithmetic operator - division";
	toktyp["*"] = "arithmetic operator - multiplication";
	toktyp["%"] = "arithmetic operator - modulo";
	toktyp[">"] = "relational operator - greater than";
	toktyp["<"] = "relational operator - less than";
	toktyp["=="] = "relational operator - equality";
	toktyp["="] = "assignment operator";
	toktyp["++"] = "increment operator";
	toktyp["--"] = "decrement operator";
	toktyp["main"] = "Main function declarator";
	toktyp["return"] = "function return statement";
	toktyp["void"] = "void function declarator";
	toktyp[";"] = "end_of_line representor";
	toktyp["#"] = "character type declarator";
}

bool isSpecialSymbol(string str){
	if(str=="$" || str=="@" || str=="#"){
		return true;
	}
	return false;
}

bool isBinaryOperator(string str){
	if(str=="+" || str=="-" || str=="*" || str=="/" || str=="%"){
		return true;
	}
	return false;
}

bool isUnaryOperator(string str){
	if(str=="++" || str=="--"){
		return true;
	}	
	return false;
}

bool isAssignmentOperator(string str){
	if(str=="="){
		return true;
	}
	return false;
}

bool isRelationalOperator(string str){
	if(str==">" || str == "<" || str == "=="){
		return true;
	}
	return false;
}

bool is_string(string lexeme){
	int len=lexeme.length();
	int c_ind=0;
	while(c_ind<len){
		if(!isalpha(lexeme[c_ind])){
			return false;
		}
		c_ind++;
	}
	return true;
}

bool is_stringOfDigits(string lexeme){
	int len=lexeme.length();
	int c_ind=0;
	while(c_ind<len){
			if(!(isdigit(lexeme[c_ind]))){
				return false;
			}
			c_ind++;
		}
	return true;
}

bool is_intLiteral(string lexeme){
	int len=lexeme.length();
	int c_ind=0;
	if(lexeme[0]=='0' && len==1)
		return true;
	if(lexeme[0]=='0'){
		return false;
	}
	else{
		while(c_ind<len){
			if(!(isdigit(lexeme[c_ind]))){
				return false;
			}
			c_ind++;
		}
	}
	return true;
}

bool is_alphanumericString(string str){
	int len=str.length();
	int c_ind=0;
	while(c_ind<len){
		if(!isalnum(str[c_ind])){
			return false;
		}
		c_ind++;
	}
	return true;
}

//checks if the next character can be combined with the current string or not
bool stringPossible(string temp, char c){
	if(temp.length()==0){
		return true;
	}
	// else if(is_string(temp) && isalpha(c)){
	// 	return true;
	// }
	else if(is_alphanumericString(temp) && isalnum(c)){
		return true;
	}
	else if(temp=="+" && c=='+'){
		return true;
	}
	else if(temp=="-" && c=='-'){
		return true;
	}
	else if(temp=="=" && c=='='){
		return true;
	}
	// else if(is_stringOfDigits(temp) && isdigit(c)){
	// 	return true;
	// }
	else{
		return false;
	}
}

// parse till next lexeme on current line

string get_name(string line, int len, int &ind){
	string buf;
	buf.push_back(line[ind++]);
	while(ind<len){
		if(!isalnum(line[ind]) && line[ind] != '_'){
				ind--;
				break;
		}
		buf.push_back(line[ind++]);
	}
	return buf;
}

string get_num(string line, int len, int &ind){
	string buf;
	int first_digit_at = ind;
	bool success = true;
	while( ind < len && isdigit(line[ind])) {
		const char c = line[ind];
		switch(c) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
			break;
		}
		buf.push_back(c); 
		++ind;
	}
	if(line[ind]==' ' || line[ind]==';' || isBinaryOperator(""+line[ind]) || isRelationalOperator(""+line[ind])){
		--ind;
		return buf;
	}
	return "";
}

void remove_back_slash(string &s){
	for( auto it = s.begin(); it != s.end(); ++it ) {
		if(* it == '\\'){
			if(it + 1 >= s.end()) 
				continue;
			it = s.erase(it);
			if( * it == 'a' ) * it = '\a';
			else if( * it == 'b' ) * it = '\b';
			else if( * it == 'f' ) * it = '\f';
			else if( * it == 'n' ) * it = '\n';
			else if( * it == 'r' ) * it = '\r';
			else if( * it == 't' ) * it = '\t';
			else if( * it == 'v' ) * it = '\v';
		}
	}
}

string get_const_str(string line, int len, int &ind){
	string buf = "\"";
	const char quote_type = line[ind];
	int starting_at = ind;
	// omit beginning quote
	++ind;
	while(ind<len) {
		if(line[ind] == quote_type && line[ind-1] != '\\') 
			break;
		buf.push_back( line[ind++]);
	}
	if(line[ind] != quote_type) {
		return ""; // error hai ye 
	}
	// omit ending quote
	++ind;
	// check if required
	remove_back_slash(buf);
	buf+="\"";
	return buf;
}

string get_operator(string line,int line_len,int &i){
	int op_type = -1;
	switch(line[i]) {
	case '+':
		if( i < line_len - 1 ) {
			if( line[i+1] == '+' ) {
				++i;
				return "++";
			}
		}
		return "+";
	case '-':
		if( i < line_len - 1 ) {
			if( line[i+1] == '-' ) {
				++i;
				return "--";
			}
		}
		return "-";
	case '*':
		return "*";
	case '/':
		return "/";
	case '%':
		return "%";
	case '&':
		return "&";
	case '|':
		return "|";
	case '~':
		return "~";
	case '=':
		if(i<line_len - 1) {
				if( line[i+1] == '=' ) {
					++i;
					return "==";
				}
		}
		return "="; 
	case '<':
		if( i < line_len - 1 ) {
			if( line[i+1] == '=' ) {
					++i;
					return "<=";
			}
		}
		return "<";
	case '>':
		if( i < line_len - 1 ) {
			if( line[i+1] == '=' ) {
					++i;
					return ">=";
			}
		}
		return ">";
	case '!':
		if( i < line_len - 1 ) {
			if( line[i+1] == '=' ) {
					++i;
					return "!=";
			}
		}
		return "!";
	case '^':
		return "^";
	case '?':
		return "?";
	case ':':
		return ":";
	case ';':
		return ";";
	case '(':
		return "(";
	case ')':
		return ")";
	case '{':
		return "{";
	case '}':
		return "}";
	case '[':
		return "[";
	case ']':
		return "]";
	default:
		return ""; 
	}
	++i;
	return "";
}



string getNextLexOP(string line, int &ind){
	string temp="";
	cout << "Line recvd:" << line << endl;
	// cout<<"Ind: "<<ind<<", value: "<<line[ind]<<endl;

	while(line[ind]!='\n'){
		// cout<<"Examining: "<<line[ind]<<endl;

		// whitespace
		if(line[ind]==' ' || line[ind]=='\t'){
				ind++;
				continue;
		}
		
		// special symbol (int char string)

		if( (line[ind]=='$' || line[ind]=='#' || line[ind]=='@') && (line[ind+1]==' ' || line[ind+1]=='\n')){
			if(line[ind]=='$'){
				return "$";
			}
			else if(line[ind]=='#') return "#";
			return "@";
		}

		// strings
		if(isalpha(line[ind])){
			string str = get_name(line, line.length(), ind);
			// if( str == TokStrs[ TOK_MAIN_SRC_BEG ] ) {
			// 	if( !is_main_src ) main_src_block = true;
			// 	continue;
			// } else if( str == TokStrs[ TOK_MAIN_SRC_END ] ) {
			// 	if( !is_main_src ) main_src_block = false;
			// 	continue;
			// }
			// if( main_src_block ) continue;
			return str;
		}

		// numbers
		if(isdigit(line[ind])){
			string num = get_num(line,line.length(),ind);
			if(num.empty()){
				return "error_Digit";
			}
			return num;
		}

		// const strings
		if(line[ind] == '\"' || line[ind] == '\'' ) {
			string res = get_const_str(line, line.length(),ind);
			if(res.empty()){
				return "error_Const_String";
			}
			return res;
		}

		// operators
		string op = get_operator(line,line.length(),ind);
		if(op.empty()){
			return "error_Operator_OR_Brackets";
		}
		else return op;
	}
	// cout << " the char is : " << line[ind] << "\n";
	// cout<<"Returning: "<<temp<<endl;
	return temp;
}


// lex line by line
void lexerLine(string line, int &n){
	for(int ind=0;ind<line.length();ind++){
		string lexeme = getNextLexOP(line,ind);
		cout << "From lexer line :::: " << lexeme << "\n";
		// end
		if(lexeme.length()==0) 
			break;
		//cout << lexeme << "\n";
		if(lexeme[0]=='\"'){
			cout << "<" << "String Literal" << ", " << lexeme << "> " << " on line number " << n << "\n";
			fout << "<" << "String Literal" << ", " << lexeme << "> " << " on line number " << n << "\n";
		}
		if(keys.find(lexeme)!=keys.end()){ // handling keywords and special characters
			int toknum = keys[lexeme];
			string type_tok = toktyp[lexeme];
			// fout << "Token type : " << type_tok << ", Token Number : " << toknum << ", Lexeme : " << lexeme << " on line number " << n << "\n";
			cout << "<" << type_tok << ", " << lexeme << "> " << " on line number " << n << "\n";
			fout << "<" << type_tok << ", " << lexeme << "> " << " on line number " << n << "\n";
		}
		else if(lexeme.compare("error_Digit")==0){
			cout << lexeme << " : error on line number : " << n << "\n";
			fout << lexeme << " : error on line number : " << n << "\n";
			break;
		}
		else if(lexeme == "error_Const_String"){
			cout << lexeme << " : error on line number : " << n << "\n";
			fout << lexeme << " : error on line number : " << n << "\n";
			break;
		}
		else if(lexeme == "error_Operator_OR_Brackets"){
			cout << lexeme << " : error on line number : " << n << "\n";
			fout << lexeme << " : error on line number : " << n << "\n";
			break;
		}
		else if(is_string(lexeme)){
			// handle identifiers and stuff that cannot be mapped
			cout << "< Identifier" << ", " << lexeme << ">" << " on line number " << n << "\n";
			fout << "< Identifier" << ", " << lexeme << ">" << " on line number " << n << "\n";
		}
		else if(is_intLiteral(lexeme)){
			cout << "< int_literal" << ", " << lexeme <<">" << " on line number " << n << "\n";
			fout << "< int_literal" << ", " << lexeme <<">" << " on line number " << n << "\n";
		}
		else{
			cout << lexeme << " : error on line number : " << n << "\n";
			fout << lexeme << " : error on line number : " << n << "\n";
			break;
		}
	}
}

int main(){
	ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    mapKeywords();
    setTokenTypes();
    string linbuff = "";
	ifstream fin ("inp.txt");
	ofstream fout ("out.txt");
	int c,n=0;
	if (fin.is_open()){
		if(fout.is_open()){
		    while (getline (fin,linbuff)){
		    	cout << linbuff << '\n';
		    	linbuff+="\n";
		    	n++;
		    	lexerLine(linbuff,n);
	       		linbuff = "";
		    }
		    fin.close();
		    fout.close();
		}
		else{
    		cout << "Error opening output file. Please try again. \n";
    	}
	}
	else{
		cout << "Error opening input file. Please try again. \n";
	}
	return 0;
}

