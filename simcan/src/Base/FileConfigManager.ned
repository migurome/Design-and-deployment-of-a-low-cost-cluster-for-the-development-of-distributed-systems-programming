// Module that manages all configuration files.
//
// If any file must not be loaded, set the corresponding parameter to "".
// Note that each node that load this object will share the same configuration.
//
//
// <h2> Servers file </h2>
//
// The <b>servers</b> file has the following syntax:
//
// First line contains the number of entries in the config file. <br>
// Remaining lines contains information as follows:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD>Machine-IP:listenPort:serverID</TD></TR>
// </TABLE></center>
//
// Where:
//
// - <i>Machine-IP</i>: Is the server IP. 
// - <i>listenPort</i>: Is the port where the server is listening for incoming connections.
// - <i>serverID</i>: Is the ID of server connection.
//
// Example:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD> 
// 3<br>
// client[0]:10000:0<br>
// client[1]:10000:1<br>
// client[2]:10000:2<br>
// </TABLE></center>
//
// If file <b>serversFile</b> not exists, then <i>connectAddress</i> and <i>connectPort</i> parameters
// will be used to establish a connection with server.
//
// <br><br>
//
//
//
//
// <h2> File System (pre-load files) </h2>
//
// The <b>preloadFiles</b> file has the following syntax:
//
// First line contains the number of files to preload. <br>
// Rest lines contains information as follows:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD>FileName:FileSize (in KBytes)</TD></TR>
// </TABLE></center>
//
// Where:
//
// - <i>FileName</i>: Is the name of the file to load.
// - <i>FileSize</i>: Is size of the file (in KB) to load.
//
// Example:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD> 
// 3<br>
// dataFile_1.dat:1024<br>
// dataFile_2.dat:2048<br>
// resultsFile.dat:0<br>
// </TABLE></center>
//
// <br><br>
//
//
//
//
//
// <h2> Virtual File system configuration </h2>
//
// The <b>iorConfigFile</b> file has the following syntax:
//
// First line contains the number of entries in the config file. <br>
// Rest lines contains information as follows:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD>
// <b>LOCAL</b>:path:FS_Index<br>
// <b>REMOTE</b>:path:serverID<br>
// </TABLE></center>
//
// Where:
//
// - <i>path</i>: Is the path corresponding of the current entry.
// - <i>FS_Index</i>: Is the index of the file system associated to <i>path</i>.
// - <i>serverID</i>: Is the ID of the corresponding server associated to <i>path</i>. This ID must be the same that the connection ID placed in the servers file.
//
// Example:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD>
// 3<br>
// LOCAL:/local_1:0<br>
// LOCAL:/local_2:1<br>
// REMOTE:/remote:0
// </TABLE></center>
//
// <br><br>
//
//
//
//
//
//
// <h2> MPI Environment file </h2>
//
// The <b>mpiConfig</b> file has the following syntax:
//
// First line contains the number of entries in the config file. <br>
// Rest lines contains information as follows:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD>Machine-IP:listenPort:Rank</TD></TR>
// </TABLE></center>
//
// where:
//
// - <i>Machine-IP</i>: Is the server IP. 
// - <i>listenPort</i>: Is the port where the server is listening for incoming connections.
// - <i>Rank</i>: Rank of current MPI process.
//
// Example:
//
// <center><TABLE BORDER="1" CELLPADDING="5" CELLSPACING="0" BGCOLOR="#f5f5f5" WIDTH="90%">
// <TR><TD> 
// 3<br>
// 192.168.0.1:10000:0<br>
// 192.168.0.1:10000:1<br>
// 192.168.0.1:10000:2<br>
// </TABLE></center>
//
// Each line represent a process.
// Several process can be defined with the same Machine-IP but not with both Machine-IP and Rank.
//
//
// <br><br>
//
// @author Alberto N&uacute;&ntilde;ez Covarrubias
// @date 2010-11-30

package SIMCAN.Base;

simple FileConfigManager{
    
    parameters:
        string mpiEnv = default("");			// MPI Environment file
        string serversNfs = default("");		// Servers file (for NFS)
        string serversPfs = default("");		// Servers file (for parallel FS)
        string preloadFiles = default("");		// Preload FS file
        string vfsConfigFile = default("");		// Virtual File System file       
        @display("i=block/cogwheel");
}

