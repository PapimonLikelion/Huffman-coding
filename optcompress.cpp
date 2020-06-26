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
	char letter[2];
	string encoded_bits;
};

bool compare(node a, node b){
	return a.value < b.value;	
}

void compress(node* root, string bits, ofstream &writeFile){			
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
//		cout<<endl;
//		cout<<"THIS SHOULD GO INSIDE : "<<bit_input<<endl;
		//마지막꺼 없는경우 NULL 로! 
//		if(!root->letter[1])
//			writeFile<<root->letter[0]<<root->letter[1]<<root->letter[1];
//		else
		writeFile<<root->letter[0]<<root->letter[1];
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
		
		if(root->letter[0] == '\n'){
			cout<<"\\n";		
		}else if(root->letter[0] == '\r'){
			cout<<"\\r";		
		}else if(root->letter[0] == '\t'){
			cout<<"\\t";		
		}else{
			cout<<root->letter[0];
		}
		
		if(root->letter[1]==-1){
			cout<<":"<<root->encoded_bits<<" ";
		}else if(root->letter[1] == '\n'){
			cout<<"\\n"<<":"<<root->encoded_bits<<" ";		
		}else if(root->letter[1] == '\r'){
			cout<<"\\r"<<":"<<root->encoded_bits<<" ";		
		}else if(root->letter[1] == '\t'){
			cout<<"\\t"<<":"<<root->encoded_bits<<" ";		
		}else{
			cout<<root->letter[1]<<":"<<root->encoded_bits<<" ";	
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
	char letter1;
	char letter2;
	bool check = false;
	ifstream readFile;
	readFile.open("input.txt");
	while(!readFile.eof() && readFile.get(letter1))
	{ 
		letter2 = readFile.get();
		if(letter2 != EOF){
//			cout<<"The letter1 is "<<letter1<<" The letter2 is "<<letter2<<endl;
//			printf("ASCII of letter 2 = %d\n", letter2);
			for(int i=0; i<v.size();i++){
				if(v[i].letter[0] == letter1 && v[i].letter[1] == letter2){
					v[i].value++;
					check = true;
					break;
				}
			}
			if(!check){
				node newnode;
				newnode.value = 1;
				newnode.letter[0] = letter1;
				newnode.letter[1] = letter2;
				v.push_back(newnode);
			}
			check = false;		
		}else{
//			cout<<"The letter1 is "<<letter1<<" The letter2 is "<<letter2<<endl;
//			printf("EOFEOF ASCII of letter 2 = %d\n", letter2);
			node newnode;
			newnode.value = 1;
			newnode.letter[0] = letter1;
			newnode.letter[1] = EOF;
			v.push_back(newnode);
		}
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
//	writeFile<<endl;
	//write 00000000 to detect the end of mapping
	char a1 = '\r';
	char a2 = -2;
	
	writeFile<<a2<<a2<<a2;
	cout<<endl;
	unsigned char acc;
	int bitcount = 0;
	
	writeFile<<total_bits<<endl; 
	while(!readFile.eof() && readFile.get(letter1) )
	{	
		letter2 = readFile.get();
		
		if(letter2 != EOF){		
			for(int i=0; i<v.size(); i++){
				if(letter1==v[i].letter[0] && letter2==v[i].letter[1]){
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
		else{
			for(int i=0; i<v.size(); i++){
				if(letter1==v[i].letter[0]){
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
