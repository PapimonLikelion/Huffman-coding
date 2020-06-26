#include<iostream>
#include<fstream> //ifofstream
#include<vector> //vector
#include<algorithm> //sort
#include<string>
using namespace std;

struct node{
	string value;
	char letter;
}; 

int total_bits = 0;

int main()
{
	vector<node> v;
	ifstream readFile("compressed.bin", ios::in | ios_base::binary);

	//처음엔 매핑	
	char c;
	//true for letter, false for number
	int check = -1;
	string str = "";
	string binary_string = "";
	int bits_to_read = 0;
	char letter;	
	while (readFile.get(c))
	{	
		
		if(bits_to_read > 8 && check == 1){
			for (int i=7; i>= 0; i--){
				int temp = ((c >> i) & 1);
				str += to_string(temp);
			}
			bits_to_read -= 8;
      continue;
		}
		
		if(bits_to_read <= 8 && check == 1){
			for (int i=7; i>= (8-bits_to_read); i--){
				int temp = ((c >> i) & 1);
				str += to_string(temp);
			}
			node newnode;
			newnode.letter = letter;
			newnode.value = str;
//			cout<<"PUTPUT! Letter : "<<newnode.letter<<" value : "<<newnode.value<<endl;
			v.push_back(newnode);
			check = -1;
			str = "";
			binary_string = "";
			continue;
		}	
		//how many bits to read
		if(check == 0){
			for (int i = 7; i >= 0; i--){
			    int temp = ((c >> i) & 1);	
			    binary_string += to_string(temp);
			}
			bits_to_read = stoi(binary_string, 0, 2);
			check = 1;
		}
		//check true, letter!
		if(check == -1){
			letter = c;
			check=0;
		}	
		
		if(c == '\n'){
			char temp = readFile.peek();
			if(temp == '\0'){
				break;
			}
		}
	}
	

	//00000000 read!
	readFile>>c;
	//then read total bits
	readFile>>total_bits;
	//cout<<"total bits : "<<total_bits<<endl;
	
	ofstream writeFile("decompressed.txt");

	
	//newline 하나 버리기 
	readFile.get(c);
	//cout<<endl;
	int read_bits = 0; 
	str = "";
	while (readFile.get(c)){
		for (int i = 7; i >= 0; i--){
			if(read_bits == total_bits)
				goto exit;
			read_bits++;
		    int temp = ((c >> i) & 1);	
		    str += to_string(temp);
//		    cout<<"STR IS "<<str<<endl;
		    for(int j=0; j<v.size(); j++){
//		    	cout<<"looking for letter : "<<v[j].value<<endl;
		    	if(str == v[j].value){
//		    		cout<<"???";
		    		writeFile<<v[j].letter;
		    		str="";
				}
			}
		}
		    
	}
	
	exit:
	
	return 0;
}


