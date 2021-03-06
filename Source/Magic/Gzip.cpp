/*
 * @File: Gzip.cpp
 * @Author: chafey
 * @Date: 2020-02-13 17:09:39
 * @LastEditTime: 2020-03-25 00:31:23
 */
#include "Gzip.h"
#include <zlib.h>
#include <string>
#include <fstream>

namespace Magic{
namespace Gzip{

    //from https://github.com/chafey/GZipCodec
    #define CHUNK 16384
    #define windowBits 15
    #define GZIP_ENCODING 16

    // GZip Compression
    // @param data - the data to compress (does not have to be string, can be binary data)
    // @param compressedData - the resulting gzip compressed data
    // @param level - the gzip compress level -1 = default, 0 = no compression, 1= worst/fastest compression, 9 = best/slowest compression
    // @return - true on success, false on failure
    bool Compress(const std::string& data, std::string& compressedData, int level) {
        unsigned char out[CHUNK];
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        if (deflateInit2(&strm, level, Z_DEFLATED, windowBits | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        {
            return false;
        }
        strm.next_in = (unsigned char*)data.c_str();
        strm.avail_in = (uInt)data.length();
        do {
            int have;
            strm.avail_out = CHUNK;
            strm.next_out = out;
            if (deflate(&strm, Z_FINISH) == Z_STREAM_ERROR)
            {
                return false;
            }
            have = CHUNK - strm.avail_out;
            compressedData.append((char*)out, have);
        } while (strm.avail_out == 0);
        if (deflateEnd(&strm) != Z_OK)
        {
            return false;
        }
        return true;
    }

    // GZip Decompression
    // @param compressedData - the gzip compressed data
    // @param data - the resulting uncompressed data (may contain binary data)
    // @return - true on success, false on failure
    bool UnCompress(const std::string& compressedData, std::string& data) {
        int ret;
        unsigned have;
        z_stream strm;
        unsigned char out[CHUNK];

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        if (inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK)
        {
            return false;
        }

        strm.avail_in = (uInt)compressedData.length();
        strm.next_in = (unsigned char*)compressedData.c_str();
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            switch (ret) {
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd(&strm);
                return false;
            }
            have = CHUNK - strm.avail_out;
            data.append((char*)out, have);
        } while (strm.avail_out == 0);

        if (inflateEnd(&strm) != Z_OK) {
            return false;
        }

        return true;
    }

    int CompressFile(const char* srcFile, const char * outFileName)
    {
        char buf[BUFSIZ] = { 0 };
        uInt bytes_read = 0;
        gzFile out = gzopen(outFileName, "wb");
        if (!out)
        {
            return -1;
        }

        std::ifstream in(srcFile, std::ios::binary);
        if (!in.is_open()) {
            return -1;
        }

        while (true)
        {
            in.read(buf, BUFSIZ);
            bytes_read = (uInt)in.gcount();
            if (bytes_read == 0)
                break;
            int bytes_written = gzwrite(out, buf, bytes_read);
            if (bytes_written == 0)
            {
                gzclose(out);
                return -1;
            }
            if (bytes_read != BUFSIZ)
                break;
        }
        gzclose(out);

        return 0;
    }

    int UnCompressFile(const char* srcFile, const char * outFileName) {
        char buf[BUFSIZ] = { 0 };
        std::ofstream out(outFileName, std::ios::binary);
        if (!out.is_open()) {
            return -1;
        }

        gzFile fi = gzopen(srcFile, "rb");
        if (!fi)
        {
            return -1;
        }

        gzrewind(fi);
        while (!gzeof(fi))
        {
            int len = gzread(fi, buf, BUFSIZ);
            out.write(buf, len);
        }
        gzclose(fi);

        return 0;
    }
}
}