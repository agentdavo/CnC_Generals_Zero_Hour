#include "Common/LocalFile.h"
#include "Common/RAMFile.h"
#include <cstring>
#include <cstdlib>

LocalFile::LocalFile() : m_fp(nullptr) {}

LocalFile::~LocalFile() {
    if(m_fp)
        fclose(m_fp);
}

Bool LocalFile::open(const Char *filename, Int access) {
    if(!File::open(filename, access))
        return FALSE;

    char mode[5] = {0};
    char *m = mode;
    if(access & WRITE) {
        if(access & READ)
            { *m++ = 'w'; *m++ = '+'; }
        else
            *m++ = (access & APPEND) ? 'a' : 'w';
    } else {
        *m++ = 'r';
    }
    if(access & BINARY) *m++ = 'b';
    else *m++ = 't';
    *m = 0;

    m_fp = fopen(filename, mode);
    if(!m_fp) {
        File::close();
        return FALSE;
    }
    return TRUE;
}

void LocalFile::close() {
    if(m_fp) {
        fclose(m_fp);
        m_fp = nullptr;
    }
    File::close();
}

Int LocalFile::read(void *buffer, Int bytes) {
    if(!m_fp) return -1;
    return static_cast<Int>(fread(buffer, 1, bytes, m_fp));
}

Int LocalFile::write(const void *buffer, Int bytes) {
    if(!m_fp) return -1;
    return static_cast<Int>(fwrite(buffer, 1, bytes, m_fp));
}

Int LocalFile::seek(Int new_pos, File::seekMode mode) {
    if(!m_fp) return -1;
    int origin = SEEK_SET;
    if(mode == File::CURRENT) origin = SEEK_CUR;
    else if(mode == File::END) origin = SEEK_END;
    if(fseek(m_fp, new_pos, origin) != 0)
        return -1;
    long pos = ftell(m_fp);
    return pos >= 0 ? static_cast<Int>(pos) : -1;
}

void LocalFile::nextLine(Char *buf, Int bufSize) {
    if(!m_fp) return;
    int c, i = 0;
    while((c = fgetc(m_fp)) != EOF) {
        if(buf && i < bufSize-1)
            buf[i] = static_cast<Char>(c);
        if(c == '\n') break;
        ++i;
    }
    if(buf && bufSize > 0)
        buf[(i < bufSize-1)? i : bufSize-1] = 0;
}

Bool LocalFile::scanInt(Int &newInt) {
    if(!m_fp) return FALSE;
    return fscanf(m_fp, "%d", &newInt) == 1 ? TRUE : FALSE;
}

Bool LocalFile::scanReal(Real &newReal) {
    if(!m_fp) return FALSE;
    return fscanf(m_fp, "%f", &newReal) == 1 ? TRUE : FALSE;
}

Bool LocalFile::scanString(AsciiString &newString) {
    if(!m_fp) return FALSE;
    char buffer[512];
    if(fscanf(m_fp, "%511s", buffer) == 1) {
        newString = buffer;
        return TRUE;
    }
    return FALSE;
}

char* LocalFile::readEntireAndClose() {
    Int sz = size();
    char *data = new char[sz];
    read(data, sz);
    close();
    return data;
}

File* LocalFile::convertToRAMFile() {
    RAMFile *ram = new RAMFile();
    if(ram->open(this)) {
        ram->deleteOnClose();
        close();
        return ram;
    }
    delete ram;
    return this;
}

