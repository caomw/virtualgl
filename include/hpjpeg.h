/* Copyright (C)2004 Landmark Graphics
 *
 * This library is free software and may be redistributed and/or modified under
 * the terms of the wxWindows Library License, Version 3 or (at your option)
 * any later version.  The full license is in the LICENSE.txt file included
 * with this distribution.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * wxWindows Library License for more details.
 */

#if defined(_MSC_VER) && defined(_WIN32) && defined(DLLDEFINE)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

#if defined(_MSC_VER) && defined(_WIN32)
#define DLLCALL __stdcall
#else
#define DLLCALL
#endif

// Subsampling
#define NUMSUBOPT 3
#define HPJ_MINWIDTH 32
#define HPJ_MINHEIGHT 16

enum {HPJ_444=0, HPJ_422, HPJ_411};

// Flags
#define HPJ_BGR       1
#define HPJ_BOTTOMUP  2
#define HPJ_EOLPAD    4
#define HPJ_FORCEMMX  8   // Force IPP to use MMX code even if SSE available
#define HPJ_FORCESSE  16  // Force IPP to use SSE1 code even if SSE2 available
#define HPJ_FORCESSE2 32  // Force IPP to use SSE2 code (useful if auto-detect is not working properly)

typedef void* hpjhandle;

#ifndef max
 #define max(a,b) ((a)>(b)?(a):(b))
#endif
#define HPJBUFSIZE(width,height) (max((max(width,16))*(max(height,16))*3,2048))
#ifdef __cplusplus
extern "C" {
#endif

// API follows


/*
  hpjhandle hpjInitCompress(void)

  Creates a new JPEG compressor instance, allocates memory for the structures,
  and returns a handle to the instance.  Most applications will only
  need to call this once at the beginning of the program or once for each
  concurrent thread.  Don't try to create a new instance every time you
  compress an image, because this will cause performance to suffer.

  RETURNS: NULL on error
*/
DLLEXPORT hpjhandle DLLCALL hpjInitCompress(void);


/*
  int hpjCompress(hpjhandle j,
     unsigned char *srcbuf, int width, int height, int pixelsize,
     unsigned char *dstbuf, unsigned long *size,
     int jpegsubsamp, int jpegqual, int flags)

  [INPUT] j = instance handle previously returned from a call to
     hpjInitCompress()
  [INPUT] srcbuf = pointer to user-allocated image buffer containing pixels in
     RGB(A) or BGR(A) form
  [INPUT] width =  width (in pixels) of the source image
  [INPUT] height = height (in pixels) of the source image
  [INPUT] pixelsize = size (in bytes) of each pixel in the source image
     RGBA and BGRA: 4, RGB and BGR: 3
  [INPUT] dstbuf = pointer to user-allocated image buffer which will receive
     the JPEG image.  This buffer should be allocated to be 3*width*height
     bytes in size, although not all of it will be used.  For extremely small
     images (32x32 or smaller), 3*width*height might not be enough buffer
     space to hold an entire MCU, so allocate the larger of 3*width*height or
     2048 bytes, just to be safe.  The macro HPJBUFSIZE(width, height) can be
     used to automatically give the proper buffer size.
  [OUTPUT] size = pointer to unsigned long which receives the size (in bytes)
     of the compressed image
  [INPUT] jpegsubsamp = Specifies either 4:1:1, 4:2:2, or 4:4:4 subsampling.
     When the image is converted from the RGB to YUV colorspace as part of the
     JPEG compression process, every other U, V (chrominance) pixel can be
     discarded to produce a smaller image with little perceptible loss of
     image clarity (the human eye is more sensitive to small changes in
     brightness than small changes in color.)

     HPJ_411: 4:1:1 subsampling.  Discards every other U, V pixel in both
        horizontal and vertical directions.
     HPJ_422: 4:2:2 subsampling.  Discards every other U, V pixel only in
        the horizontal direction.
     HPJ_444: no subsampling.

  [INPUT] jpegqual = JPEG quality (an integer between 0 and 100 inclusive.)
  [INPUT] flags = the bitwise OR of one or more of the following

     HPJ_BGR: The components of each pixel in the source image are stored in
        B,G,R order, not R,G,B
     HPJ_BOTTOMUP: The source image is stored in bottom-up (Windows) order,
        not top-down
     HPJ_EOLPAD: Each scan line of the source image is padded to the nearest
        4-byte boundary

  RETURNS: 0 on success, -1 on error
*/
DLLEXPORT int DLLCALL hpjCompress(hpjhandle j,
	unsigned char *srcbuf, int width, int height, int pixelsize,
	unsigned char *dstbuf, unsigned long *size,
	int jpegsubsamp, int jpegqual, int flags);


/*
  hpjhandle hpjInitDecompress(void)

  Creates a new JPEG decompressor instance, allocates memory for the
  structures, and returns a handle to the instance.  Most applications will
  only need to call this once at the beginning of the program or once for each
  concurrent thread.  Don't try to create a new instance every time you
  decompress an image, because this will cause performance to suffer.

  RETURNS: NULL on error
*/
DLLEXPORT hpjhandle DLLCALL hpjInitDecompress(void);


/*
  int hpjDecompress(hpjhandle j,
     unsigned char *srcbuf, unsigned long size,
     unsigned char *dstbuf, int width, int height, int pixelsize, int pad,
     int flags)

  [INPUT] j = instance handle previously returned from a call to
     hpjInitDecompress()
  [INPUT] srcbuf = pointer to a user-allocated buffer containing the JPEG image
     to decompress
  [INPUT] size = size of the JPEG image buffer (in bytes)
  [INPUT] dstbuf = pointer to user-allocated image buffer which will receive
     the bitmap image.  This buffer should be (width*pixelsize+pad)*height
     bytes in size.
  [INPUT] width =  width (in pixels) of the destination image
  [INPUT] height = height (in pixels) of the destination image
  [INPUT] pixelsize = size (in bytes) of each pixel in the destination image
     RGBA/RGBx and BGRA/BGRx: 4, RGB and BGR: 3
  [INPUT] flags = the bitwise OR of one or more of the following

     HPJ_BGR: The components of each pixel in the destination image should be
        written in B,G,R order, not R,G,B
     HPJ_BOTTOMUP: The destination image should be stored in bottom-up
        (Windows) order, not top-down
     HPJ_EOLPAD: Each scan line of the destination image should be padded to
        the nearest 4-byte boundary

  RETURNS: 0 on success, -1 on error
*/
DLLEXPORT int DLLCALL hpjDecompress(hpjhandle j,
	unsigned char *srcbuf, unsigned long size,
	unsigned char *dstbuf, int width, int height, int pixelsize,
	int flags);


/*
  int hpjDestroy(hpjhandle h)

  Frees structures associated with a compression or decompression instance
  
  [INPUT] h = instance handle (returned from a previous call to
     hpjInitCompress() or hpjInitDecompress()

  RETURNS: 0 on success, -1 on error
*/
DLLEXPORT int DLLCALL hpjDestroy(hpjhandle h);


/*
  char *hpjGetErrorStr(void)
  
  Returns a descriptive error message explaining why the last command failed
*/
DLLEXPORT char* DLLCALL hpjGetErrorStr(void);

#ifdef __cplusplus
}
#endif
