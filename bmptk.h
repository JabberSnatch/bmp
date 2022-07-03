#pragma once

#include <cstdint>

namespace bmptk
{

enum class Result
{
    Success,
    UnknownHeaderField,
    UnknownBitmapVersion,
};

struct BitmapFile
{
    uint8_t const* memory;
    uint8_t const* pixel_data;
    uint32_t header_size;
    int32_t width;
    int32_t height;
};

Result LoadBMP(uint8_t const* _memory, BitmapFile* _bmpFile);


#ifdef BMPTK_IMPLEMENTATION

template <typename T> static inline void const*
AdvancePointer(void const* _source, size_t _count = 1) {
    return ((T const*)_source) + _count;
}

static inline uint8_t
ReadU8(void const*& _ptr) {
    uint8_t v = *(uint8_t const*)_ptr;
    _ptr = AdvancePointer<uint8_t>(_ptr);
    return v;
}

static inline uint16_t
ReadU16_BE(void const*& _ptr) {
    uint8_t const* base = (uint8_t const*)_ptr;
    _ptr = AdvancePointer<uint16_t>(_ptr);
    return (base[0]<<8) | base[1];
}

static inline uint32_t
ReadU32_BE(void const*& _ptr) {
    uint8_t const* base = (uint8_t const*)_ptr;
    _ptr = AdvancePointer<uint32_t>(_ptr);
    return (base[0]<<24) | (base[1]<<16) | (base[2]<<8) | base[3];
}

static inline uint16_t
ReadU16_LE(void const*& _ptr) {
    uint8_t const* base = (uint8_t const*)_ptr;
    _ptr = AdvancePointer<uint16_t>(_ptr);
    return (base[1]<<8) | base[0];
}

static inline uint32_t
ReadU32_LE(void const*& _ptr) {
    uint8_t const* base = (uint8_t const*)_ptr;
    _ptr = AdvancePointer<uint32_t>(_ptr);
    return (base[3]<<24) | (base[2]<<16) | (base[1]<<8) | base[0];
}

Result LoadBMP(uint8_t const* _memory, BitmapFile* _bmpFile)
{
    _bmpFile->memory = _memory;

    void const* ptr = _memory;
    if (ReadU16_BE(ptr) != 0x424D /*BM*/)
        return Result::UnknownHeaderField;

    ReadU32_BE(ptr); // file size
    ReadU32_BE(ptr); // 2x16 reserved bits
    _bmpFile->pixel_data = _bmpFile->memory + ReadU32_LE(ptr);

    uint32_t header_size = ReadU32_LE(ptr);
    _bmpFile->header_size = header_size;

    if (header_size != 40u)
        return Result::UnknownBitmapVersion;

    _bmpFile->width = (int32_t)ReadU32_LE(ptr);
    _bmpFile->height = (int32_t)ReadU32_LE(ptr);

    return Result::Success;
}

#endif

} // namespace bmptk
