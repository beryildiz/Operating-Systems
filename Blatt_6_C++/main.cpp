#include <sys/time.h>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "queue.hpp"
#include "reader.hpp"
#include "client.hpp"

using std::cout;
using std::cerr;
using std::stoi;
using std::vector;
using std::setprecision;
using std::fixed;
using std::ifstream;
using std::ofstream;

int main(int argc, char* argv[]){
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <inputfile> [options]\n\n"
             << "Options:\n" 
             << "\t--webreq-delay <uint>\n"
             << "\t--webreq-delay-seed <int>\n"
             << "\t--webreq-path <path>\n"
             << "\t--thread-count <uint>\n";
        return EXIT_FAILURE;
    }

    // generate time stamp
    struct timeval  tv;

    gettimeofday(&tv, NULL);
    double start = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    //-------------------------------------------------------------

    int thread_num = 1;
    int delay = 0;
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            string tmp = argv[i];
            if ( (tmp == "--thread-count") && (i < argc-1) ) {
                thread_num = stoi(argv[i+1]);  
            }
            if ( (tmp == "--webreq-delay") && (i < argc-1) ) {
                delay = stoi(argv[i+1]);  
            }
        }
    }

    Queue fifo;

    Reader r(fifo, argv[1]);
    vector<Client*> c;

    WebRequest req(argc, argv);

    for (int i=0; i<thread_num; i++) {
        c.push_back( new Client(fifo, req, i+1) );
    }

    r.getThread()->join();
    for (int i=0; i<thread_num; i++) {
        c[i]->getThread()->join();
    }

    for (auto p : c) {
        delete p;
    } 
    c.clear();

    // measure runtime
    gettimeofday(&tv, NULL);
    double end = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
    double diff = (end - start) / 1000;

    cout << "Duration: " << fixed << setprecision(2) << diff << " s\n";

    //-------------------------------------------------------------

    bool found = false;

    string data;
    int data1 = 0;
    int data2 = 0;
    double data3 = 0.0f;
    int n = 1;

    ifstream r_file("performance_measurements.txt");
    ofstream w_file("tmp.txt");
    while( getline(r_file, data) ) {
        int start = 9;
        int end = data.find(" ms")-1;
        data1 = stoi( data.substr(start, end) );

        start = data.find("Threads")+8;
        end = data.find(" ->")-1;
        data2 = stoi( data.substr(start, end) );

        start = data.find("Duration")+10;
        end = data.find(" s")-1;
        data3 = stod( data.substr(start, end) );

        start = data.find("Avg. of")+8;
        end = data.find(" values")-1;
        n = stoi( data.substr(start, end) );

        if ( (data1 == delay) && (data2 == thread_num)) {
            n++;
            data3 -= data3 / n;
            data3 += diff / n;
            found = true;
        }
        w_file << "Delay of " << data1 << " ms: Threads: " << data2 << " -> Duration: " << data3 << " s (Avg. of " << n << " values)\n";
    }
    r_file.close();

    if (!found) {
        w_file << "Delay of " << delay << " ms: Threads: " << thread_num << " -> Duration: " << diff << " s (Avg. of 1 value)\n";
    }
    w_file.close();

    remove("performance_measurements.txt");
    rename("tmp.txt", "performance_measurements.txt");

    return EXIT_SUCCESS;
}

