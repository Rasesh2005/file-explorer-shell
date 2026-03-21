#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) return 1;
    struct stat st;
    if (stat(argv[1], &st) != 0) return 2;
    
    std::string volpath = "/.vol/" + std::to_string(st.st_dev) + "/" + std::to_string(st.st_ino);
    std::cout << "Original: " << argv[1] << std::endl;
    std::cout << "Inode: " << st.st_ino << ", Dev: " << st.st_dev << std::endl;
    std::cout << "Volfs path: " << volpath << std::endl;
    
    char pathbuf[4096];
    int fd = open(volpath.c_str(), O_RDONLY);
    if (fd < 0) {
        perror("open volpath");
        return 0; // Don't fail the command, just show output
    }
    
    if (fcntl(fd, F_GETPATH, pathbuf) != -1) {
        std::cout << "Resolved path: " << pathbuf << std::endl;
    } else {
        perror("fcntl F_GETPATH");
    }
    close(fd);
    return 0;
}
