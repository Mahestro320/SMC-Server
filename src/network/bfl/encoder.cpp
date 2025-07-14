#include <type_traits>
#include "io/console.hpp"
#include "network/bfl/encoder.hpp"

void BFLEncoder::setData(const std::vector<FileInfo>& data) {
    this->data = data;
}

void BFLEncoder::encode() {
    console::out::inf("encoding a list of " + std::to_string(data.size()) + " files");
    pushFileCount();
    allocMemoryForFiles();
    for (uint64_t i{}; i < data.size(); ++i) {
        pushFile(data[i]);
    }
}

void BFLEncoder::pushFileCount() {
    bfl.resize(current_offset + sizeof(uint64_t));
    const uint64_t data_size{data.size()};
    std::memcpy(bfl.data() + current_offset, &data_size, sizeof(uint64_t));
    current_offset += sizeof(uint64_t);
}

void BFLEncoder::allocMemoryForFiles() {
    uint64_t final_size{};
    for (const FileInfo& file : data) {
        final_size += getFileSizeInBuffer(file);
    }
    bfl.resize(current_offset + final_size);
}

void BFLEncoder::pushFile(const FileInfo& file) {
    pushFileTypeOf(file);
    pushFileSizeOf(file);
    pushFileNameSizeOf(file);
    pushFileNameOf(file);
}

void BFLEncoder::pushFileTypeOf(const FileInfo& file) {
    using file_type_t = std::underlying_type_t<FileType>;
    const file_type_t file_type{static_cast<file_type_t>(file.type)};
    std::memcpy(bfl.data() + current_offset, &file_type, sizeof(FileType));
    current_offset += sizeof(FileType);
}

void BFLEncoder::pushFileSizeOf(const FileInfo& file) {
    const uint64_t file_size{file.size};
    std::memcpy(bfl.data() + current_offset, &file_size, sizeof(uint64_t));
    current_offset += sizeof(uint64_t);
}

void BFLEncoder::pushFileNameSizeOf(const FileInfo& file) {
    const uint64_t file_name_size{file.name.size()};
    std::memcpy(bfl.data() + current_offset, &file_name_size, sizeof(uint64_t));
    current_offset += sizeof(uint64_t);
}

void BFLEncoder::pushFileNameOf(const FileInfo& file) {
    std::memcpy(bfl.data() + current_offset, file.name.data(), file.name.size());
    current_offset += file.name.size();
}

uint64_t BFLEncoder::getFileSizeInBuffer(const FileInfo& file) {
    // [file type][file size][filename size][filename content]
    return sizeof(FileType) + sizeof(uint64_t) * 2 + file.name.size();
}

const std::vector<char>& BFLEncoder::getBFL() const {
    return bfl;
}
