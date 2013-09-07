#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string.h>
#include <stdlib.h> 
#include <list>
#include <algorithm>
#include <testing.h>

static const char *DELIMITERS=" ,.-/:?!@#$^*()+\"\'\n";

using namespace std;

//convert a string to its lowercase representation
void makeLowerCase(char *string){
	for(size_t i=0; i<strlen(string); i++){
		string[i]=tolower(string[i]);
	}
}

//parses a string into word tokens
void parseString(string s, map<string, list<int> >& postingslists, map<string, int> docids, string currfile){
	char *tokenized=strtok((char*)s.c_str(), DELIMITERS);
	while(tokenized!=NULL){
		makeLowerCase(tokenized);
		map<string, list<int> >::iterator itr=postingslists.find(tokenized);
		int filedocid=docids[currfile];
		if(itr==postingslists.end()){
			postingslists[tokenized]=list<int>(1,filedocid);

		}else{
			list<int> alist=postingslists[tokenized];
			list<int>::iterator itr=find(alist.begin(), alist.end(), filedocid);
			if(itr==alist.end()){//list does not contain docid
				postingslists[tokenized].push_back(filedocid);
			}
		}
		tokenized=strtok(NULL, DELIMITERS);
	}
}
//reads and tokenizes from an input stream and populates a map with tokens
void readInFile(ifstream& infile, map<string, list<int> >& postingslists, map<string, int> docids, string currfile){
	string temp;
	while(getline(infile, temp)){
		parseString(temp, postingslists, docids, currfile);
	}
}

//gets names of input files
void getNamesOfInputfiles(map<string,int>& docids, map<int, string>& reversedocids){
	cout<<"Please enter names of corpus files, hitting the 'Enter' key after each filename. Enter 'DONE' when you are done."<<endl;
	string filename;
	ifstream temp;
	int docid=1;
	while(true){
		getline(cin, filename);
		if(filename=="DONE") {
			temp.close();
			break;
		}
		temp.open(filename.c_str());
		if(!temp.is_open()){
			cout<<"File could not be opened. Please input the name of another file."<<endl;
		}else{
			docids[filename]=docid;
			reversedocids[docid]=filename;
			docid++;
		}
		temp.close();
	}
}

map<string, list<int> >  readInAllFiles(ifstream& infile, map<string, int> docids){
	map<string, list<int> > postingslists;
	for(map<string,int>::iterator itr=docids.begin(); itr!=docids.end(); ++itr){
		infile.open(itr->first);
		readInFile(infile, postingslists, docids, itr->first);
		infile.close();
	}
	for(map<string,list<int> >::iterator itr=postingslists.begin(); itr!=postingslists.end(); ++itr){
		(itr->second).sort(); //sort docids in each list by increasing order
	}
	return postingslists;
}
 	 	
//request query from user and parse into words for search
vector<string> requestQuery(){
	cin.clear();
	string temp;
	cout<<"Please enter a query:"<<endl;
	getline(cin,temp);
	vector<string> wordsinquery;
	char *tokenized=strtok((char*)temp.c_str(), DELIMITERS);
	while(tokenized!=NULL){
		wordsinquery.push_back((string)tokenized);
		tokenized=strtok(NULL, DELIMITERS);
	}
	return wordsinquery;
}

//gets the results list from processing two lists
list<int> processTwoLists(list<int> list1, list<int> list2){
	list<int> sharedIDs;
	list<int>::iterator itr1=list1.begin();
	list<int>::iterator itr2=list2.begin();
	while(itr1!=list1.end() && itr2!=list2.end()){
		if((*itr1)==(*itr2)){
			sharedIDs.push_back(*itr1);
			itr1++;
			itr2++;
		}else if((*itr1)>(*itr2)){
			itr2++;
		}else{
			itr1++;
		}
	}
	if(!sharedIDs.size()){//no common IDs so add the merge of the two original lists
		list1.merge(list2);
		sharedIDs=list1;
	}
	sharedIDs.sort(); //sort IDs
	return sharedIDs;
}

//get doc IDs of the appropriate articles
list<int> processQuery(map<string, list<int> >postingslists, vector<string> wordsinquery){
	list<int> merged;
	if(wordsinquery.size()==1){
		if(postingslists.find(wordsinquery[0])!=postingslists.end()){
			return postingslists[wordsinquery[0]];
		}else{
			cout<<"No entries found to match your query"<<endl;
			return list<int>();
		}
	}else{
		size_t i=1;
		if(postingslists.find(wordsinquery[0])!=postingslists.end()){
			if(postingslists.find(wordsinquery[1])!=postingslists.end()){
				list<int> entry1=postingslists[wordsinquery[0]];
				list<int> entry2=postingslists[wordsinquery[1]];
				merged=processTwoLists(entry1, entry2);
				i++;
			}
		}
		while(i!=wordsinquery.size()){//process remaining words
			if(postingslists.find(wordsinquery[i])!=postingslists.end()){
				merged=processTwoLists(merged, postingslists[wordsinquery[i]]);
				i++;
			}
			else
				i++;
		}
	}
	return merged;
}
int main(){
	ifstream inputfile;
	map<string, int> docids;
	map<int, string> reversedocids;

	getNamesOfInputfiles(docids,reversedocids);
	map<string, list<int> > postingslists=readInAllFiles(inputfile, docids);
	vector<string> wordsinquery=requestQuery();
	list<int> processed=processQuery(postingslists, wordsinquery);
	vector<string> filesfound;
	for(list<int>::iterator itr=processed.begin(); itr!=processed.end(); ++itr){
		filesfound.push_back(reversedocids[*itr]);
	}
	if(filesfound.size()!=0){
		cout<<"Documents matching your query:"<<endl;
	}else{
		cout<<"No documents found matching your query."<<endl;
	}
	printVector(filesfound);
	return 0;
}