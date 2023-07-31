#include <iostream>
#include <filesystem>
#include <string>
#include <thread>
#include <mutex>

namespace fs = std::filesystem;
using std::cout;
using std::endl;
using std::string;
using std::thread;
using std::mutex;
using std::lock_guard;

const int MAX_THREADS = 8;
bool fileFound = false;
mutex mtx;

void searchInDirectory(const string& directory_path, const string& filename) {
    for (const auto& entry : fs::directory_iterator(directory_path)) {
        {
            lock_guard<mutex> guard(mtx);
            if (fileFound) {
                return;
            }
        }

        if (entry.is_regular_file() && entry.path().filename().string() == filename) {
            {
                lock_guard<mutex> guard(mtx);
                fileFound = true;
            }
            cout << "File found: " << entry.path() << endl;
            return;
        }

        if (entry.is_directory()) {
            searchInDirectory(entry.path().string(), filename);
        }
    }
}

int main() {
    string filename;

    cout << "Enter the file name to search for: ";
    std::getline(std::cin, filename);

    thread searchThread([&]() {
        searchInDirectory("C:\\", filename);
        });

    searchThread.join();

    return 0;
}
