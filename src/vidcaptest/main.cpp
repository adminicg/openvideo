

#include "../vidcapture/VidCapture.h"

#include <stdio.h>


CVVidCapture* vidCap = NULL;
int counter = 0;
char fileNameTempl[128];

CVImage::CVIMAGE_TYPE imageType = CVImage::CVIMAGE_RGB24;

bool
capCallback(CVRES status, CVImage* imagePtr, void* userParam)
{
	int* ctr = (int*)userParam;

	if(*ctr>=3)
		return false;

	// Only try to work with the image pointer if the
	// status is successful!
	if (CVSUCCESS(status))
	{
		char fileName[128];
		sprintf(fileName, fileNameTempl, *ctr);

		printf("  saving image %s\n", fileName);

		if(imagePtr->GetImageType() == CVImage::CVIMAGE_RGB24)
			CVImage::Save(fileName, imagePtr, true);
		else
			if(FILE* fp = fopen(fileName, "wb"))
			{
				fwrite(imagePtr->GetRawDataPtr(), 1, imagePtr->Width()*imagePtr->Height()*imagePtr->GetBytesPerPixel(), fp);
				fclose(fp);
			}
	}
	else
	{
		printf("ERROR: could not retrieve video image\n");
	}

	(*ctr)++;
	return true;
}


void initVidCap()
{
	vidCap = CVPlatform::GetPlatform()->AcquireVideoCapture();

	if(!CVSUCCESS(vidCap->Init()))
	{
		CVPlatform::GetPlatform()->Release(vidCap);
		printf("ERROR: initializing video capture object failed.\n");
		return;
	}

	printf("Initialized video capture object.\n");

/*	if(!CVSUCCESS(vidCap->Connect(0)))
	{
		vidCap->Uninit();
		CVPlatform::GetPlatform()->Release(vidCap);      
		printf("ERROR: connecting to camera failed.\n");
		return;
	}*/
}


bool testDevice(int nWhich, int nStartMode, bool nListOnly)
{
	CVVidCapture::VIDCAP_MODE modeInfo;
	int numModes = 0;
	vidCap->GetNumSupportedModes(numModes);

	printf("Found %d supported capture modes\n", numModes);
	if(numModes==0)
		return false;

	// Check each mode 
	//
	for(int curmode=nStartMode; curmode<numModes; curmode++)
		if(CVSUCCESS(vidCap->GetModeInfo(curmode, modeInfo)))
		{
			printf("MODE: %d, %dx%d @ %d frames/sec (%s)\n", curmode, modeInfo.XRes, modeInfo.YRes, modeInfo.EstFrameRate, vidCap->GetFormatModeName(modeInfo.InputFormat));
		
			if(!nListOnly)
			{
				if(CVFAILED(vidCap->SetMode(curmode)))
				{
					printf("  ERROR: failed to set camera mode %d.\nIf the application crashes please restart with the next mode!\n", curmode);
					continue;
				}

				if(imageType == CVImage::CVIMAGE_RGB24)
					sprintf(fileNameTempl, "device%d_mode%02d_img%%d.ppm", nWhich, curmode);
				else
					sprintf(fileNameTempl, "device%d_mode%02d_img%%d.raw", nWhich, curmode);

				counter = 0;
				if(!CVSUCCESS(vidCap->StartImageCap(imageType, capCallback, &counter)))
				{
					printf("  ERROR: failed to start capturing.\nIf the application crashes please restart with the next mode!\n");
					continue;
				}

				// wait until 3 image where captured
				//
				while(counter<3)
					Sleep(100);

				vidCap->Stop();
			}
		}

	return true;
}


void main(int argc, char** argv)
{
	printf("VidCapTest 1.0a by Daniel Wagner (using the VidCapture Library)\n\n");
	printf("This programm will try to access all capture modes on all attached cameras.\nFor each mode 5 images will be recorded as RGB24 PPM file.\n\n");

	initVidCap();
	int startMode = 0, startDevice = 0;

	if(argc>1)
		startMode = atoi(argv[1]);

	if(argc>2)
		startDevice = atoi(argv[2]);


	int numDev = 0;
	vidCap->GetNumDevices(numDev);
	printf("Found %d capturing devices.\n", numDev);

	for(int i=startDevice; i<numDev; i++)
	{
		CVVidCapture::VIDCAP_DEVICE deviceInfo;

		vidCap->GetDeviceInfo(i, deviceInfo);
		printf("\n\nUsing capture device %d: %s\n", i, deviceInfo.DeviceString);


/*		int devNameLen = 0;
		vidCap->GetDeviceName(0,devNameLen);
		devNameLen++;
		char* cameraName = new char[devNameLen];
		vidCap->GetDeviceName(cameraName,devNameLen);*/

		if(!CVSUCCESS(vidCap->Connect(i)))
		{
			printf("ERROR: connecting to camera device %d failed.\n", i);
			continue;
		}

		printf("\nListing all available modes:\n");
		bool ok = testDevice(i, 0, true);

		if(ok)
		{
			printf("\nTesting all modes (starting with mode %d)\n", startMode);
			testDevice(i, startMode, false);
		}
		//delete cameraName;
	}
}
