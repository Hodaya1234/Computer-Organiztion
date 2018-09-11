#define min(a, b) ((a) < (b) ? a : b)
#define max(a, b) ((a) > (b) ? a : b)

void myfunction(Image *image, char* backupOrgImgpName, char* blurRsltImgName, char* sharpRsltImgName) {

	int size = 3 * n * n * sizeof(char);

	int i, j, ii, jj, blurR, blurG, blurB, place, sharpR, sharpG, sharpB;

	unsigned char* helper;
	unsigned char* currentChar;

	//TODO malloc one big chunck and divide inside
	unsigned char* blurImg = malloc(size);
	unsigned char* sharpImg = malloc(size);
	unsigned char* backupOrg = malloc(size);
	

	for (i = 0 ; i < size ; i++) {
		backupOrg[i] = image->data[i];
	}
	memcpy(blurImg, backupOrg, size);
	memcpy(sharpImg, backupOrg, size);

	/**
	SHARP AND BLUR FUNCTION:
	*/

	for (i = 1 ; i <= n - 1; i++) {
		for (j =  1 ; j <= n - 1 ; j++) {

			blurR = blurG = blurB = 0;

			for (ii = i-1; ii <= i+1; ii++) {
				for(jj = j-1; jj <= j+1; jj++) {

					place = (ii*3*n) + (3*jj);
					blurR += ((int) backupOrg[place]) / 9;
					blurG += ((int) backupOrg[place+1]) / 9;
					blurB += ((int) backupOrg[place+2]) / 9;
				}
			}

			blurR =	(min(max(blurR, 0), 255));
			blurG = (min(max(blurG, 0), 255));
			blurB =	(min(max(blurB, 0), 255));

			place = (i*3*n) + (3*j);

			blurImg[place] = (unsigned char) blurR;
			blurImg[place + 1] = (unsigned char) blurG;
			blurImg[place + 2] = (unsigned char) blurB;
		}

	}
	for (i = 0 ; i < size ; i++) {
		image->data[i] = blurImg[i];
	}
	writeBMP(image, backupOrgImgpName, blurRsltImgName);

	for (i = 1 ; i <= n - 1; i++) {
		for (j =  1 ; j <= n - 1 ; j++) {

			sharpR = sharpG = sharpB = 0;

			for (ii = i-1; ii <= i+1; ii++) {
				for(jj = j-1; jj <= j+1; jj++) {

					place = (ii*3*n) + (3*jj);
					if((ii!=i)||(jj!=j)){
						sharpR -= (int)blurImg[place];
						sharpG -= (int)blurImg[place+1];
						sharpB -= (int)blurImg[place+2];					
					} else {
						sharpR += (9 * (int)blurImg[place]);
						sharpG += (9 * (int)blurImg[place+1]);
						sharpB += (9 * (int)blurImg[place+2]);
					}
				}
			}

			sharpR = (min(max(sharpR, 0), 255));
			sharpG = (min(max(sharpG, 0), 255));
			sharpB = (min(max(sharpB, 0), 255));
			
			place = (i*3*n) + (3*j);
			sharpImg[place] = (unsigned char) sharpR;
			sharpImg[place + 1] = (unsigned char) sharpG;
			sharpImg[place + 2] = (unsigned char) sharpB;

		}
	}


	for (i = 0 ; i < size ; i++) {
		image->data[i] = sharpImg[i];
	}

	writeBMP(image, backupOrgImgpName, sharpRsltImgName);
	
	free(blurImg);
	free(sharpImg);
	free(backupOrg);
}
