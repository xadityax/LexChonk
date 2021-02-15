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

// parse till next typespace
string getNextLex(string line, int &ind){
	string temp="";
	// cout<<"Line recvd:"<<line<<endl;
	//account for semi-colons too? @xadityax
	//Also what if their is no space between lexemes? e.g. some_literal=5 @xadityax
	// cout<<"Ind: "<<ind<<", value: "<<line[ind]<<endl;
	while(line[ind]!='\n'){
		// cout<<"Examining: "<<line[ind]<<endl;
		if(line[ind]==' '){
			if(temp.length()>0){
				return temp;
			}
			else{
				continue;
			}
		}
		else if(stringPossible(temp, line[ind])){
			temp+= line[ind++];
		}
		else{
			ind--;
			return temp;
		}
	}
	while(line[ind]!=' ' && line[ind]!='\n'){
		temp += line[ind++];
	}
	// cout << " the char is : " << line[ind] << "\n";
	// cout<<"Returning: "<<temp<<endl;
	// return temp;
}

// lex line by line
void lexerLine(string line, int &n){
	for(int ind=0;ind<line.length();ind++){
		string lexeme = getNextLex(line,ind);
		
		if(lexeme.length()==0) 
			break;
		//cout << lexeme << "\n";
		
		if(keys.find(lexeme)!=keys.end()){ // handling keywords and special characters
			int toknum = keys[lexeme];
			string type_tok = toktyp[lexeme];
			// fout << "Token type : " << type_tok << ", Token Number : " << toknum << ", Lexeme : " << lexeme << " on line number " << n << "\n";
			fout<<"<"<<type_tok<<", "<<lexeme<<">\n";
			// for(int i=0; i<lexeme.length(); i++) 
			// 	putc(lexeme[i],fout);
		}
		else if(is_string(lexeme)){
			// handle identifiers and stuff that cannot be mapped
			// fout<<"Token: Identifier, Lexeme: "<<lexeme<<" on line number "<< n<< "\n";
			fout<<"<Identifier"<<", "<<lexeme<<">\n";
		}
		else if(is_intLiteral(lexeme)){
			fout<<"<int_literal"<<", "<<lexeme<<">\n";
		}
		else{
			fout << lexeme << " : error on line number : " << n << "\n";
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
    //FILE *fin, *fout;
	//fin = fopen("inp.txt","r");
	//fout = fopen("out.txt","w");
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
	// if(fin!=NULL){
	// 	if(fout!=NULL){
	// 	    while ((c = fgetc(fin)) != EOF) { // standard C I/O file reading loop
	//        		while(c!=";"){
	//        			linbuff += to_string(c);
	//        			c = fgetc(fin);
	//        		}
	//        		n++;
	//        		// parse line buff
	//        		lexLine(linbuff,n,fout);
	//        		linbuff = "";
	//     	}
 //    	}
 //    	else{
 //    		cout << "Error opening output file. Please try again. \n";
 //    	}
	// }
	// else{
	// 	cout << "Error opening input file. Please try again. \n";
	// }
	return 0;
}

