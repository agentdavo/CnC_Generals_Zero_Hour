#pragma once

#include "common/file.h"
#include <cstdio>
#include <cstdlib>

class LocalFile : public File
{
private:
    FILE *m_fp;

public:
    LocalFile();
    virtual ~LocalFile();

    static void* operator new(size_t s) { return std::malloc(s); }
    static void operator delete(void* p) { std::free(p); }

    virtual Bool open(const Char *filename, Int access = 0) override;
    virtual void close() override;
    virtual Int read(void *buffer, Int bytes) override;
    virtual Int write(const void *buffer, Int bytes) override;
    virtual Int seek(Int new_pos, File::seekMode mode = File::CURRENT) override;
    virtual void nextLine(Char *buf = NULL, Int bufSize = 0) override;
    virtual Bool scanInt(Int &newInt) override;
    virtual Bool scanReal(Real &newReal) override;
    virtual Bool scanString(AsciiString &newString) override;
    virtual char* readEntireAndClose() override;
    virtual File* convertToRAMFile() override;
};

