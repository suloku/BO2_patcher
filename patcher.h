#define BUFFER_SIZE 4096  // Adjust buffer size based on system memory

int search_binary_pattern(const char *filename, const unsigned char *pattern, size_t pattern_size, long *ret_offset);
int replace_data_in_file(const char *filename, long offset, const unsigned char *new_data, size_t data_size);
int read_4_bytes_from_file(const char *filename, long offset, unsigned char *buffer);
