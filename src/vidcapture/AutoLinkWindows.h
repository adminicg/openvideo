

// is this VC6?
#if _MSC_VER<=1200
//   Strmiids_old.lib is located in folder build/vc6/support
#    pragma comment ( lib, "Strmiids_old.lib" )
#else
#    pragma comment ( lib, "Strmiids.lib" )
#endif //_MSC_VER<=1200

#pragma comment ( lib, "Quartz.lib" )

// select between release & debug version of the library
#ifdef _DEBUG
#  pragma comment ( lib, "VidCaptured.lib" )
#else
#  pragma comment ( lib, "VidCapture.lib" )
#endif //_DEBUG
