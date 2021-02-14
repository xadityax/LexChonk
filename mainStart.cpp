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
	toktyp["main"] = "Main function declarator";
	toktyp["void"] = "void function declarator";
	toktyp[";"] = "end_of_line representor";
	toktyp["#"] = "character type declarator";
}

// parse till next typespace
string getNextLex(string line, int &ind){
	string temp="";
	
	while(line[ind]!=' ' && line[ind]!='\n'){
		temp += line[ind++];
	}
	// cout << " the char is : " << line[ind] << "\n";
	return temp;
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
			fout << "Token type : " << type_tok << ", Lexeme : " << lexeme << " on line number " << n << "\n";
			// for(int i=0; i<lexeme.length(); i++) 
			// 	putc(lexeme[i],fout);
		}
		else{
			// handle identifiers and stuff that cannot be mapped
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

