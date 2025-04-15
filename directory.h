char get_path_separator();
void append_directory(char *base_path, const char *directory, char *result, size_t result_size);
int file_exists(const char *filename);
const char *get_filename(const char *path);
char* remove_extension(const char *filename);
