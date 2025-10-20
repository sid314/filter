#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

      int sum = (int)image[i][j].rgbtBlue + (int)image[i][j].rgbtGreen +
                (int)image[i][j].rgbtRed;

      BYTE avgBrigtness = (BYTE)round((sum / 3.0));
      RGBTRIPLE px;
      px.rgbtBlue = avgBrigtness;
      px.rgbtGreen = avgBrigtness;
      px.rgbtRed = avgBrigtness;
      image[i][j] = px;
    }
  }
  return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < (width / 2); j++) {

      RGBTRIPLE pxTemp = image[i][j];
      image[i][j] = image[i][width - j - 1];
      image[i][width - j - 1] = pxTemp;
    }
  }

  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {

  RGBTRIPLE copyImage[height][width];
  // make copy of image
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

      copyImage[i][j] = image[i][j];
    }
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      double n = 0.0;
      double r = 0, g = 0, b = 0;
      int boxSize =
          (i == 0) || (j == 0) || (i == height - 1) || (j == width - 1) ? 3 : 4;

      for (int k = -1; k < boxSize - 1; k++) {
        for (int l = -1; l < boxSize - 1; l++) {
          int y = i + k;
          int x = j + l;

          if ((x > -1) && (x < width) && (y > -1) && (y < height)) {

            n++;
            r += image[y][x].rgbtRed;
            g += image[y][x].rgbtGreen;
            b += image[y][x].rgbtBlue;
          }
        }
      }

      r /= n;
      b /= n;
      g /= n;
      copyImage[i][j] = (RGBTRIPLE){b > 255 ? 255 : (BYTE)round(b),
                                    g > 255 ? 255 : (BYTE)round(g),
                                    r > 255 ? 255 : (BYTE)round(r)};
    }
  }

  // use copy of image
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {

      image[i][j] = copyImage[i][j];
    }
  }
  return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]) {

  int cHeight = height + 2, cWidth = width + 2;

  RGBTRIPLE copyImage[cHeight][cWidth];

  RGBTRIPLE blackPx = (RGBTRIPLE){0, 0, 0};

  // make copy
  for (int i = 0; i < cHeight; i++) {
    for (int j = 0; j < cWidth; j++) {

      if ((i == 0) || (j == 0) || (i == (cHeight - 1)) || j == (cWidth - 1)) {

        copyImage[i][j] = blackPx;
      } else {
        copyImage[i][j] = image[i - 1][j - 1];
      }
    }
  }
  int GxAr[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  int GyAr[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

  for (int i = 1; i < cHeight - 1; i++) {
    for (int j = 1; j < cWidth - 1; j++) {
      int rGx = 0, rGy = 0, gGx = 0, gGy = 0, bGx = 0, bGy = 0;

      for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
          int copyIndexY = i - 1 + k;
          int copyIndexX = j - 1 + l;
          int copyRedVal = copyImage[copyIndexY][copyIndexX].rgbtRed;
          int copyGreenVal = copyImage[copyIndexY][copyIndexX].rgbtGreen;
          int copyBlueVal = copyImage[copyIndexY][copyIndexX].rgbtBlue;

          int GxVal = GxAr[k][l];
          int GyVal = GyAr[k][l];

          rGx += GxVal * copyRedVal;
          rGy += GyVal * copyRedVal;
          bGx += GxVal * copyBlueVal;
          bGy += GyVal * copyBlueVal;
          gGx += GxVal * copyGreenVal;
          gGy += GyVal * copyGreenVal;
        }
      }

      double r = sqrt((rGx * rGx) + (rGy * rGy));
      double g = sqrt((gGx * gGx) + (gGy * gGy));
      double b = sqrt((bGx * bGx) + (bGy * bGy));

      image[i - 1][j - 1].rgbtRed = r > 255 ? 255 : (BYTE)round(r);
      image[i - 1][j - 1].rgbtGreen = g > 255 ? 255 : (BYTE)round(g);
      image[i - 1][j - 1].rgbtBlue = b > 255 ? 255 : (BYTE)round(b);
    }
  }

  return;
}
