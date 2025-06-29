/*
 *  LZH-Light algorithm implementation v 1.0
 *  Copyright (C) Sergey Ignatchenko 1998
 *  This  software  is  provided  "as is"  without  express  or implied 
 *  warranty.
 *
 *  Permission to use, copy, modify, distribute and sell this  software 
 *  for any purpose is hereby  granted  without  fee,  subject  to  the 
 *  following restrictions:
 *  1. this notice may not be removed or altered;
 *  2. altered source versions must be plainly marked as such, and must 
 *     not be misrepresented as being the original software.
 */

#include <lzhl/lzhl.h>
#define LZHLINTERNAL
#include "_huff.h"
#include "_lz.h"

extern "C" {
LZHL_CHANDLE LZHL_ABI LZHLCreateCompressor( void )
    {
    return (LZHL_CHANDLE)new LZHLCompressor();
    }

size_t LZHL_ABI LZHLCompressorCalcMaxBuf( size_t sz )
    {
    return LZHLCompressor::calcMaxBuf( sz );
    }

size_t LZHL_ABI LZHLCompress( LZHL_CHANDLE compressor, void* dst, const void* src, size_t srcSz )
    {
    return ((LZHLCompressor*)compressor)->compress( (BYTE*)dst, (const BYTE*)src, srcSz );
    }

void LZHL_ABI LZHLDestroyCompressor( LZHL_CHANDLE compressor )
    {
    delete (LZHLCompressor*)compressor;
    }

//*****************************************************************************

LZHL_DHANDLE LZHL_ABI LZHLCreateDecompressor( void )
    {
    return (LZHL_DHANDLE)new LZHLDecompressor();
    }

int  LZHL_ABI LZHLDecompress( LZHL_DHANDLE decompressor, void* dst, size_t* dstSz, void* src, size_t* srcSz )
    {
    return ((LZHLDecompressor*)decompressor)->decompress( (BYTE*)dst, dstSz, (BYTE*)src, srcSz );
    }

void LZHL_ABI LZHLDecompressReset(LZHL_DHANDLE decompressor)
{
    ((LZHLDecompressor*)decompressor)->reset_decompressor();
}

void LZHL_ABI LZHLDestroyDecompressor( LZHL_DHANDLE decompressor )
    {
    delete (LZHLDecompressor*)decompressor;
    }

}//extern "C"
