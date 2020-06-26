#include<iostream>
#include<fstream> //ifofstream
#include<vector> //vector
#include<algorithm> //sort
#include<bitset>
using namespace std;

int total_bits=0;

struct node{
	node *left = NULL; 
	node *right = NULL;
	int value;
	char letter;
	string encoded_bits;
};


bool compare(node a, node b){
	return a.value < b.value;	
}

void compress(node* root, string bits , ofstream &writeFile){			
	if(!root)
		return;
	
	if(!root->left && !root->right){
		//final bit
		root->encoded_bits = bits;
		//OPTIMIZE!
		int number_of_bits = root->encoded_bits.length();
		string bit_input = bitset<8>(number_of_bits).to_string();
		int read_number = number_of_bits/8 + 1;
		//write letter
		//cout<<endl;
//		cout<<"THIS SHOULD GO INSIDE : "<<bit_input<<endl;
		writeFile<<root->letter;
		//write number of bits to read in 0~255 (length of encoded bits to read!)
		char write;
		int bitcount = 0;
		for (int j=0; j<8; j++){
			if(bit_input[j]=='0')
				write<<=1;
			else
				write = write<<1|1;
			if(++bitcount == 8){
				writeFile<<write;
//				cout<<"???"<<write<<endl;
				bitcount = 0;
			}
		}
		//Encoded bits send!
		
		for(int k=0; k<number_of_bits; k++){
			if(root->encoded_bits[k] == '0')
				write<<=1;
			else
				write = write<<1|1;
			if(++bitcount == 8){
				writeFile<<write;
				bitcount = 0;
			}
		}
		
		if(bitcount>0 && 8>bitcount){
			for(int i=bitcount; i<8; i++){
				//0추가 
				write<<=1;
			} 
			writeFile<<write;	
		}
		
		
		if(root->letter == '\n'){
			cout<<"\\n"<<":"<<root->encoded_bits<<" ";		
		}else if(root->letter == '\r'){
			cout<<"\\r"<<":"<<root->encoded_bits<<" ";		
		}else if(root->letter == '\t'){
			cout<<"\\t"<<":"<<root->encoded_bits<<" ";		
		}else{
			cout<<root->letter<<":"<<root->encoded_bits<<" ";	
		}
		total_bits += (root->value * root->encoded_bits.length());
		return;
	}
	
	compress(root->left, bits+"0", writeFile);
	compress(root->right, bits+"1", writeFile);
}

int main()
{
	vector<node> v;
	char letter;
	bool check = false;
	ifstream readFile;
	readFile.open("input.txt");
	while(!readFile.eof() && readFile.get(letter))
	{	
//		cout<<"The letter is "<<letter<<endl;
		for(int i=0; i<v.size();i++){
			if(v[i].letter == letter){
				v[i].value++;
				check = true;
				break;
			}
		}
		if(!check){
			node newnode;
			newnode.value = 1;
			newnode.letter = letter;
			v.push_back(newnode);
		}
		check = false;		
	}	
	
	v.reserve(v.size()*2);
	
	sort(v.begin(), v.end(), compare);
	
	for(int i=0 ; i < v.size()-1; i+=2){
		node newnode;
		newnode.value = v[i].value;
		newnode.right = &v[i];
		newnode.value += v[i+1].value;
		newnode.left = &v[i+1];
		v.push_back(newnode);
		sort(v.begin()+i, v.end(), compare);
	}

	ofstream writeFile("compressed.bin", ios::out | ios_base::binary);
	
	//Write Data mapping char:codeword
	compress(&v.back(), "", writeFile);


	//Go to first of the file
	readFile.clear();
	readFile.seekg(0);	
	writeFile<<endl;
	//write 00000000 to detect the end of mapping
	char a = '\0';
	writeFile<<a;
  cout<<endl;
	unsigned char acc;
	int bitcount = 0;
	
	writeFile<<total_bits<<endl; 
	while(!readFile.eof() && readFile.get(letter))
	{	
		for(int i=0; i<v.size(); i++){
			if(letter==v[i].letter){
				cout<<v[i].encoded_bits;				
				int length = v[i].encoded_bits.length();
				for (int j=0; j<length; j++){
					if(v[i].encoded_bits[j]=='0')
						acc<<=1;
					else
						acc = acc<<1|1;
					if(++bitcount == 8){
						writeFile<<acc;
						bitcount = 0;
					}
				}
			}
		}
	}	
	
//	cout<<"LEFT BIT "<<bitcount<<endl;
	if(bitcount>0 && 8>bitcount){
		for(int i=bitcount; i<8; i++){
			//0추가 
			acc<<=1;
		} 
		writeFile<<acc;	
	}
	return 0;
}
