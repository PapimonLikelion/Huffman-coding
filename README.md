# Huffman-coding

Implementation of Huffman coding with ***default version*** and ***optimized version***

input.txt is compressed to binary file compressed.bin using compress.cpp / optcompress.cpp 

compressed.bin is decompress to text file decompressed.txt using decompress.cpp / optdecompress.cpp

-How I made compressed.bin file
![compbin 설명](https://user-images.githubusercontent.com/61370901/85908626-bf06a680-b850-11ea-8ec1-f1858bf4b574.PNG)

-How I optimized the default version
![예시](https://user-images.githubusercontent.com/61370901/85908744-579d2680-b851-11ea-9c89-11cd8c0e0c39.png)
The basic idea of optimization is combining 2 letters into a single node. I added a picture of the output result for an example file. 
For compress.cpp, mapping is done on a single letter. For optcompress.cpp, mapping is done on 2 letters. 
Since the 2 letters were mapped to codewords, the text was compressed to about half compared to the original version (The exact reduction percentage depends on the mapping result). But the mapping between characters and codewords increases a lot as the file gets bigger. That is because, for a single letter, it deals with only 128 possible letters when the ASCII code is used. However, for the double letter, it has to deal with a maximum of 128*128 letters when the ASCII code is used. Nevertheless, the size of compressed.bin of optimized version will be smaller than the original. 

-Test result
![이거만하자](https://user-images.githubusercontent.com/61370901/85908625-bdd57980-b850-11ea-9fe2-bebe6ace3a3c.PNG)
