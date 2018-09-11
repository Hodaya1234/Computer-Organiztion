
typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;


/* Compute min and max of two integers, respectively */
#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#define calcIndex(i, j, n) ((i)*(n)+(j))


/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than 3/2 and/or
* column index smaller than 3/2
*/
void smoothBlur(int dim, pixel *src, pixel *dst) {

	// 	int row, col;
	// for (row = 0 ; row < m ; row++) {
	// 	for (col = 0 ; col < n ; col++) {

	// 		pixels[row*n + col].red = image->data[3*row*n + 3*col];
	// 		pixels[row*n + col].green = image->data[3*row*n + 3*col + 1];
	// 		pixels[row*n + col].blue = image->data[3*row*n + 3*col + 2];
	// 	}
	// }


	int ii, jj;

	int R, G, B;
	pixel current_pixel, p;

	register int i, j;
	for (i = 1 ; i < dim - 1; i++) {
		for (j =  1 ; j < dim - 1 ; j++) {

			R = G = B = 0;

			for(ii = i-1; ii <= i+1; ii++) {
				for(jj = j-1; jj <= j+1; jj++) {

					p = src[calcIndex(ii, jj, dim)];
					R += ((int) p.red);
					G += ((int) p.green);
					B += ((int) p.blue);
				}
			}

			R =	R / 9;
			G = G / 9;
			B =	B / 9;

			// truncate each pixel's color values to match the range [0,255]
			current_pixel.red = (unsigned char) (min(max(R, 0), 255));
			current_pixel.green = (unsigned char) (min(max(G, 0), 255));
			current_pixel.blue = (unsigned char) (min(max(B, 0), 255));

			dst[calcIndex(i, j, dim)] = current_pixel;

		}
	}
}


void smoothSharp(int dim, pixel *src, pixel *dst) {

	int ii, jj;
	int R, G, B;
	pixel current_pixel, p;

	register int i, j;
	for (i = 1 ; i < dim - 1; i++) {
		for (j =  1 ; j < dim - 1 ; j++) {

			R = G =	B = 0;

			for(ii = i-1; ii <= i+1; ii++) {
				for(jj = j-1; jj <= j+1; jj++) {

					p = src[calcIndex(ii, jj, dim)];

					if((ii != i) || (jj != j)){
						R -= ((int) p.red);
						G -= ((int) p.green);
						B -= ((int) p.blue);
					} else {
						R += 9 * ((int) p.red);
						G += 9 * ((int) p.green);
						B += 9 * ((int) p.blue);
					}

				}
			}

			// truncate each pixel's color values to match the range [0,255]
			current_pixel.red = (unsigned char) (min(max(R, 0), 255));
			current_pixel.green = (unsigned char) (min(max(G, 0), 255));
			current_pixel.blue = (unsigned char) (min(max(B, 0), 255));


			dst[calcIndex(i, j, dim)] = current_pixel;

		}
	}
}


void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			pixels[row*n + col].red = image->data[3*row*n + 3*col];
			pixels[row*n + col].green = image->data[3*row*n + 3*col + 1];
			pixels[row*n + col].blue = image->data[3*row*n + 3*col + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {

			image->data[3*row*n + 3*col] = pixels[row*n + col].red;
			image->data[3*row*n + 3*col + 1] = pixels[row*n + col].green;
			image->data[3*row*n + 3*col + 2] = pixels[row*n + col].blue;
		}
	}
}




void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName) {


	int size = m*n*sizeof(pixel);
	pixel* pixelsImg = malloc(size);
	pixel* backupOrg = malloc(size);

	charsToPixels(image, pixelsImg);
	memcpy(backupOrg, pixelsImg, size);

	smoothBlur(m, backupOrg, pixelsImg);

	pixelsToChars(pixelsImg, image);

	// write result image to file
	writeBMP(image, srcImgpName, blurRsltImgName);

	charsToPixels(image, pixelsImg);
	memcpy(backupOrg, pixelsImg, size);

	smoothSharp(m, backupOrg, pixelsImg);

	pixelsToChars(pixelsImg, image);

	// write result image to file
	writeBMP(image, srcImgpName, sharpRsltImgName);
	
	free(pixelsImg);
	free(backupOrg);
}
