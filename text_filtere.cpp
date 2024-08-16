#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

vector<string> readWords() // function to read words from a file into a vector 
{
	vector<string> words;
	ifstream infile("banned.txt");
	
	if(!infile.is_open())
	{
		cout<< "Error cant open file ! " << endl;
		return words;
	}
	
	string word;
	
	while(infile >> word) //Loop reads each word from infile and stores it in word
	{
		words.push_back(word); 
	}
	
	
	cout <<"File is succesfully opened. " << endl;
	return words;
	
}

char toLowerCase(char c) { // function convert a character to lowercase 
    if (c >= 'A' && c <= 'Z') {
        c =  c + ('a' - 'A');
    }
    return c;
}


vector<int> countWords( vector<string> &bannedWords,  string &inputfiles) //  counts occurrences of banned words in a file 
{
	vector<int> counts (bannedWords.size(), 0); // initialize counts vector to zero 
	ifstream file(inputfiles);
	
	if (!file.is_open())
	{
		cout << "Error opening the file " << inputfiles << endl;
		return counts;
	}
	
	string line;
	while(getline(file, line)) // read the file line by line
	{
		for(int j = 0; j<bannedWords.size(); j++)
		{
			string s2 = bannedWords[j];
			int count = 0;
			
			for(int a = 0; a <=(line.length() - s2.length()); a++) // loop to check occurances of the banned words ine the line 
			{
				int b;
				for(b = 0; b< s2.length(); b++)
				{
					if(toLowerCase(line[a+b]) != toLowerCase(s2[b]))
					{
						break;
					}
				}
				if(b == s2.length())
				{
					count++; // increment the count if a banned word is found
				}
			}
			
			counts[j] += count; // update the counts vector 
		}
	}
	
	file.close();
	cout <<"The words are counted succesfully from the file " << inputfiles << "\n";
	 cout <<"---------------------------------------------------------------------\n";
	return counts;
	
	
}


void filterTweets( vector<string> &bannedWords, string &inputfile , string &outputfile) //filters banned words in a file and write to another file 
{
	ifstream infile(inputfile);
	ofstream outfile(outputfile); 
	
	if (!infile.is_open())
	{
		cout <<"Error opening the input file" << endl;
		return;
	}
	
	if(!outfile.is_open())
	{
		cout<<"Error opening the output file" << endl;
		return;
	}
	
	
	string line;
	
	while(getline(infile,line)) // read the file line by line
	{
		string filteredtext = line;
		
		for(int j=0; j<bannedWords.size(); j++)
		{
			string s1 = filteredtext;
			string s2 = bannedWords[j];
			
			for(int a = 0; a <= (s1.length() - s2.length()); a++) // loop to check occurances of the banned word in the line
			{
				int b;
				for(b = 0; b<s2.length(); b++)
				{
					if(toLowerCase(s1[a+b]) != toLowerCase(s2[b]))
					{
						break;
					}
				}
				
				if( b == s2.length()) 
				{
					for(int c = 0; c < s2.length(); c++)
					{
						s1[a+c] = '*';
					}
				}
				
			}
			
			filteredtext = s1;
		}
		
		outfile << filteredtext<< endl; // write the filtered the to the output file
	}
	
	infile.close();
	outfile.close();
	
	
}

void countSwap(int &a , int &b) // function to swap integers 
{
	int temp = a;
	a = b;
	b = temp;
}

void countSwap(string &a , string &b) // function to swap strings 
{
	string temp = a;
	a = b;
	b = temp;
}

void CountSort(vector<string> &words, vector<int> &counts) // function to sort words on their counts (selection sort)
{
	int n = counts.size(); // interate through each element in the counts vector 
	for (int i = 0; i < n-1; i++)
	{
		int max = i;
		for(int j = i+1 ; j < n ; j++) // find the index of the maximum element in the remaining part of the counts vector 
		{
			if(counts[j] > counts[max])
			{
				max = j;
			}
		}
		
		countSwap(counts[i], counts[max]);
		countSwap(words[i], words[max]);  // swaps the  counts and words to sort it in descending order 
		
	}
}

vector<int> CountWordsInFiles( vector<string> &inputfiles, vector<string> &words, vector<string> &bannedWords, vector<string> &outputfiles)
{
    vector<int> counts(words.size(),0);
    
    for(int i=0; i < inputfiles.size(); i++)
    {
    	ifstream tweets(inputfiles[i]);
        if(!tweets.is_open())
        {
            cout<<"failed to open file: " << inputfiles[i]<<endl;
            continue;
        }
        
        string word;
        while (tweets >> word)
        {
        	bool found = false;
        	for(int j = 0; j <words.size(); j++)
        	{
        		if(words[j] == word)
        		{
        			counts[j]++;
        			found = true;
        			break;
        		}
        	}
        	if(!found)
        	{
        		words.push_back(word);
 		        counts.push_back(1); // if a word is found for the first time , the counter is set to 1
        	}
        		
        }
        tweets.close();

        vector<int> tempCounts = countWords(bannedWords, inputfiles[i]);
        for (int j = 0; j < bannedWords.size(); j++) 
        {
            cout << "[" << bannedWords[j] << "] is found = " << tempCounts[j] << " times in " << inputfiles[i] << endl;
            
        }
        
         cout <<"---------------------------------------------------------------------\n";

        filterTweets(bannedWords, inputfiles[i], outputfiles[i]);
        
    }
        
       return counts;
}
  
void printFrequentWords(vector<string> &words,vector<int> &counts, int occuredWords) 
{
    for (int i = 0; i < occuredWords; i++) 
    {
        cout << words[i] << ": " << counts[i] << endl; 
    }
}

int main()
{
	vector<string> bannedWords = readWords(); // read banned words from file 
    	vector<string> inputfiles = {"tweets1.txt","tweets2.txt","tweets3.txt","tweets4.txt"};
    	vector<string> outputfiles = {"tweets1Filtered.txt","tweets2Filtered.txt","tweets3Filtered.txt","tweets4Filtered.txt"};
        
        vector<string> words; // vector to store words and their counts
        vector<int> counts = CountWordsInFiles(inputfiles, words, bannedWords, outputfiles);
        
        CountSort(words, counts); // sort the words based on their count 
        
        int occuredWords = 10;  // prints the top 10 most occured words in the inputfiles
        if(words.size() < occuredWords)
        {
        	occuredWords = words.size();
        }
        
        cout <<"**The top 10 Frequent words in all the Tweets files are as follows**" << endl;
        cout <<"---------------------------------------------------------------------\n";
	printFrequentWords(words, counts, occuredWords);
    	return 0;
}
