#include "SIMCAN_MemoryBlock.h"


string SIMCAN_MemoryBlock::getFileName (){
	return fileName;
}
		
	   
void SIMCAN_MemoryBlock::setFileName (string newName){
	fileName = newName;
}		
	
	   
unsigned int SIMCAN_MemoryBlock::getOffset (){
	return offset;
}
		
  
void SIMCAN_MemoryBlock::setOffset (unsigned int newOffset){
	offset = newOffset;
}
		
	   
unsigned int SIMCAN_MemoryBlock::getBlockSize (){
	return blockSize;
}
		
	   
void SIMCAN_MemoryBlock::setBlockSize (unsigned int newBlockSize){
	blockSize = newBlockSize;
}
	
	   
bool SIMCAN_MemoryBlock::getIsPending (){
	return isPending;
}
		
	   
void SIMCAN_MemoryBlock::setIsPending (bool newIsPending){
	isPending = newIsPending;
}				
		
	   
string SIMCAN_MemoryBlock::memoryBlockToString (){
	
	std::ostringstream info;
	
		info << "FileName:" << fileName.c_str()
			 << " - Offset:" << offset
			 << " - Size:" << blockSize
			 << " - isPending:" << isPending;
			 
	return info.str();	
}


