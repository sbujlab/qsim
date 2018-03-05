void rootlogon(){
    FileStat_t buf;
    if( !gSystem->GetPathInfo("libqsimroot.so", buf) ){
	gSystem->Load("libqsimroot.so" ) ;
    }
    if( !gSystem->GetPathInfo("libqsimroot.dylib", buf) ){
	gSystem->Load("libqsimroot.dylib" ) ;
    }
}
