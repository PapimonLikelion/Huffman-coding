#include<iostream>
#include<fstream> //ifofstream
#include<vector> //vector
#include<algorithm> //sort
#include<string>
using namespace std;

struct node{
	string value;
	char letter[2];
}; 

int total_bits = 0;

int main()
{
	vector<node> v;
	ifstream readFile("compressed.bin", ios::in | ios_base::binary);

	//처음엔 매핑	
	char c;
	char c2;
	//true for letter, false for number
	int check = -1;
	string str = "";
	string binary_string = "";
	int bits_to_read = 0;
	char letter1;
	char letter2;	
	while (readFile.get(c))
	{	
		if(check==-1)
			c2 = readFile.get();
			
//		cout<<"C1--"<<c<<"C2--"<<c2<<endl;
		
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
			newnode.letter[0] = letter1;
			newnode.letter[1] = letter2;
			newnode.value = str;
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
			letter1 = c;
			letter2 = c2;
			check=0;
		}	
		
		if(c == -2){
			char temp = readFile.peek();
			//cout<<temp<<endl;
			if(temp == -2){
				temp = readFile.peek();
				//cout<<"TEMP!"<<temp<<endl;
					if(temp == -2){
						//cout<<"NEWTEMP!"<<temp<<endl;
						break;
					}
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
//	cout<<endl;
	int read_bits = 0; 
	str = "";
	int start = 0;
	int end = v.size();
	int j;
	while (readFile.get(c)){
		for (int i = 7; i >= 0; i--){
			if(read_bits == total_bits)
				goto exit;
			read_bits++;
		    int temp = ((c >> i) & 1);	
//		    str += to_string(temp);
			if(temp)
				str+="1";
			else
				str+="0";
//			if(str.length() == 1)
//				continue;
//		    for(int j=0; j<v.size(); j++){
//		    	if(str == v[j].value){
//		    		writeFile<<v[j].letter[0]<<v[j].letter[1];
//		    		str="";
//				}
//			}
			bool checkcheck = false;
		    for(j=start; j<end; j++){
      int lengthlength = str.length();
//		    	cout<<"The str is : "<<str<<" and the v[j] is "<<v[j].value<<endl;
//		    	cout<<"str[str.length()-1] is "<<str[str.length()-1]<<endl;
		    	if(str[lengthlength-1] == '0'){
			    	if(str[lengthlength-1] != v[j].value[lengthlength-1]){
			    		end = j;
//			    		cout<<"Changed end!"<<end<<endl;
					}
				}
			
				if(str[lengthlength-1] == '1'){
			    	if(str[lengthlength-1] == v[j].value[lengthlength-1]){
			    		start = j;
//			    		cout<<"Changed start!"<<start<<endl;
						checkcheck = true;
					}	
				}
				
				
				if(str == v[j].value){
					if(v[j].letter[1] == EOF)
						writeFile<<v[j].letter[0];
					else
		    		  writeFile<<v[j].letter[0]<<v[j].letter[1];
		    		//cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<v[j].letter<<endl;
		    		str="";
					start = 0;
					end = v.size();
				}
				
		    	
		    	if(checkcheck){
		    		checkcheck = false;
		    		break;
				}
				
		}  
		}
	}
	
	exit:
	//	cout<<"bits?"<<read_bits<<"and "<<total_bits<<endl;
	
	return 0;
}

