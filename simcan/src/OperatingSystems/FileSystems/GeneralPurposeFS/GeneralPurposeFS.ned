// Module that implements a GenericFS.
//
// This module translates an I/O file request (SIMCAN_App_IO_Message) to a list of disk blocks (SIMCAN_BlockList_Message)
//
// To calculate that list, this module uses distribution functions like:
//
//	<b>Weibull</b>
//
//		To use the Weibull distribution is neccessary to load the next files:
//
//			- weibull/bunchSize_ext2_Alpha.txt<br>
//			- weibull/bunchSize_ext2_Beta.txt<br>
//			- weibull/bunchSize_reiserFS_Alpha.txt<br>
//			- weibull/bunchSize_reiserFS_Beta.txt<br>
//			- weibull/contiguous_ext2_Alpha.txt<br>
//			- weibull/contiguous_ext2_Beta.txt<br>
//			- weibull/contiguous_reiserFS_Alpha.txt<br>
//			- weibull/contiguous_reiserFS_Beta.txt<br>
//			- weibull/holeSize_ext2_Alpha.txt<br>
//			- weibull/holeSize_ext2_Beta.txt<br>
//			- weibull/holeSize_reiserFS_Alpha.txt<br>
//			- weibull/holeSize_reiserFS_Beta.txt<br>
//
//
// Operation list (from/to IOR):
//
// - <b>Open a file</b><br>
//		- MessageName: SIMCAN_App_IO_Message <br>
// 		- Operation: SM_OPEN_FILE<br>
//		- Result: SIMCAN_OK | SIMCAN_FILE_NOT_FOUND<br>
//
// - <b>Close a file</b><br>
//		- MessageName: SIMCAN_App_IO_Message <br>
// 		- Operation: SM_CLOSE_FILE<br>
//		- Result: SIMCAN_OK<br>
//
// - <b>Read a file</b><br>
//		- MessageName: SIMCAN_App_IO_Message <br>
// 		- Operation: SM_READ_FILE<br>
//		- Result: SIMCAN_OK | SIMCAN_FILE_NOT_FOUND | SIMCAN_DATA_OUT_OF_BOUNDS<br>
//
// - <b>Write a file</b><br>
//		- MessageName: SIMCAN_App_IO_Message <br>
// 		- Operation: SM_WRITE_FILE<br>
//		- Result: SIMCAN_OK | SIMCAN_FILE_NOT_FOUND | SIMCAN_DISK_FULL<br>
//
// - <b>Create a file</b><br>
//		- MessageName: SIMCAN_App_IO_Message <br>
// 		- Operation: SM_CREATE_FILE<br>
//		- Result: SIMCAN_OK | SIMCAN_DISK_FULL<br>
//
// - <b>Delete a file</b><br>
//		- MessageName: SIMCAN_App_IO_Message <br>
// 		- Operation: SM_DELETE_FILE<br>
//		- Result: SIMCAN_OK<br>
//
//
// Operation list (from/to VolumeManager):
//
// - <b>Read a file</b><br>
//		- MessageName: SIMCAN_BlockList_Message <br>
// 		- Operation: SM_READ_FILE<br>
//
// - <b>Write a file</b><br>
//		- MessageName: SIMCAN_BlockList_Message <br>
// 		- Operation: SM_WRITE_FILE<br>
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.OperatingSystems.FileSystems.GeneralPurposeFS;

import SIMCAN.OperatingSystems.FileSystems.IFS;

simple GeneralPurposeFS like IFS
{
    parameters:

        int maxBlocks;						// Maximum number of blocks (Sectors of size = 512 bytes = BYTES_PER_SECTOR bytes)
        double diskRatio;					// Initial filled disk ratio. This parameter may not correspond with the real File System ocupation.
        string fsType;						// File System Type ("ext2" or "reiserFS")
        int fsBlockSize_b;					// File System block size (in bytes)
        bool preLoadFiles;					// PreLoad File System Files?
        string simcanHome = default("");	// Simcan's home folder. This parameter must be left empty unless the simulation is not able to find weibull files


        @display("i=block/table2");
    gates:
        input fromVM;		// Input gate from VM (Volume Manager)
        input fromIOR;		// Input gate from IOR (I/O Redirector)

        output toVM;		// Output gate to VM (Volume Manager)
        output toIOR;		// Output gate to IOR (I/O Redirector)

}

