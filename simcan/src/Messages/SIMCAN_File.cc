#include "SIMCAN_File.h"



SIMCAN_File::~SIMCAN_File(){	
	branchList.clear();	
}


unsigned int SIMCAN_File::getFileSize (){
	return fileSize;
}


void SIMCAN_File::setFileSize (unsigned int newSize){
	fileSize = newSize;
}


void SIMCAN_File::updateFileSize (){
	setFileSize (getTotalSectors()*BYTES_PER_SECTOR);
}


string SIMCAN_File::getFileName (){
	return fileName;
}


void SIMCAN_File::setFileName (string newName){
	fileName = newName;
}


unsigned int SIMCAN_File::getNumberOfBranches (){
	return (branchList.size());
}


size_blockList_t SIMCAN_File::getBranchSize (unsigned int branchNumber){

	if (branchNumber < getNumberOfBranches())
		return (branchList[branchNumber].numBlocks);
	else
		throw cRuntimeError("[SIMCAN_File.getBranchSize] Branch out of bounds!");
}


off_blockList_t SIMCAN_File::getBranchOffset (unsigned int branchNumber){

	if (branchNumber < getNumberOfBranches())
		return (branchList[branchNumber].offset);
	else
		throw cRuntimeError("[SIMCAN_File.getBranchOffset] Branch out of bounds! Existing:%d Requested:%d",
								getNumberOfBranches(), branchNumber);
}


void SIMCAN_File::addBranch (off_blockList_t offset, size_blockList_t size){

	fileBranch newBranch;

		newBranch.offset = offset;
		newBranch.numBlocks = size;
		branchList.push_back (newBranch);
		
		// Update file size
		updateFileSize();
}


void SIMCAN_File::addBlock (off_blockList_t sector, unsigned int blockSize){
	
	size_blockList_t blockNumber;
	size_blockList_t sectorsPerBlock;
	
		sectorsPerBlock = blockSize/BYTES_PER_SECTOR;
		blockNumber = sector/sectorsPerBlock; 
	
	addBranch (blockNumber*sectorsPerBlock, sectorsPerBlock);
	
	// Update file size
	updateFileSize();
}


size_blockList_t SIMCAN_File::getTotalSectors (){

	vector <fileBranch>::iterator file_it;
	size_blockList_t numBlocks;

		// Init
		numBlocks = 0;

		for (file_it=branchList.begin(); file_it!=branchList.end(); ++file_it){
			numBlocks+=(*file_it).numBlocks;
		}

	return numBlocks;
}


bool SIMCAN_File::containThisSector (size_blockList_t sector){
	
	int i;
	bool found;
	
		// init
		found = false;
		i = 0;
	
		while ((i<branchList.size()) && (!found)){
			
			if ((sector >= branchList[i].offset) &&
			   (sector < branchList[i].offset + branchList[i].numBlocks))
			   	found = true;
			else
				i++;			
		}
		
	return found;
}


void SIMCAN_File::clearBranchList(){
	branchList.clear();
	updateFileSize();
}


string SIMCAN_File::contentsToString (bool withBranches, bool printContents){

	std::ostringstream osStream;
	int i;	
		
		if (printContents){
		
			osStream 	<< " FileName:" << getFileName()
						<< " Size:" << getFileSize()
						<< " Sectors:" << getTotalSectors()					
						<< " NumBranches:" << getNumberOfBranches()
						<< endl;
	
			// Detailes info?
			if (withBranches){
				for (i=0; i<getNumberOfBranches(); i++)
					osStream  << "   Branch[" << i << "]  offset:" << branchList[i].offset << "   branchSize:" << branchList[i].numBlocks << endl;
			}
		}

	return osStream.str();
}


