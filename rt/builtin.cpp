/**
   Builtin functions that are useful for doing extern with.
 */

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <gc/gc.h>

#ifdef __APPLE__
#include <mach/mach_time.h>
#endif

#include <iostream>
#include <fstream>

using namespace std;

// () -> u64
uint64_t nanotime() {
#ifdef __APPLE__
    uint64_t time = mach_absolute_time();
    mach_timebase_info_data_t info = {0, 0};
    if (info.denom == 0) {
        mach_timebase_info(&info);
    }
    uint64_t time_nano = time * (info.numer / info.denom);
    return time_nano;  
#else
    uint64_t ns_per_s = 1000000000LL;
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * ns_per_s + ts.tv_nsec);
#endif    
}

// ((vector (vector int 2) 2)) -> void
void print_2x2_int_vec(int **v) {
    printf("[ %d %d ]\n"
           "[ %d %d ]\n",
           v[0][0], v[0][1], v[1][0], v[1][1]);
}

// ((vector int 4) int) -> (vector int 4)
int *rotate_4_int_vec(int *v, int j) {
  int *result = (int *)GC_MALLOC(4 * sizeof(int));

  for(int i = 0; i < 4; i++) {
    result[i] = v[(i + j) % 4];
  }

  return result;
}

// (str (vector (vector int 1024) 1024)) -> void
void write_pgm(const char *filename, int **data) {
    ofstream f(filename);

    f << "P2" << endl;
    f << "1024 1024" << endl;
    f << "255" << endl;
    for(int i = 0; i < 1024 * 1024; i++) {
        int p = data[i / 1024][i % 1024];
        if(p < 0) 
            p = 0;
        if(p > 255)
            p = 255;
        f << p << " ";
    }
}

void write_str(const char *msg, std::ofstream *f) {
  *f << msg;
}

void write_int(int num, std::ofstream *f) {
  *f << num;
}

std::ofstream* open_outfile(const char *filename) {
    // TODO: This is leaking files
    std::ofstream *f = new std::ofstream;
    f->open(filename);
    return f;
}

void close_outfile(std::ofstream *f) {
    f->close();
    delete f;
}

// (int u64) -> void
void print_int_u64(int x, uint64_t y) {
  cout << x << "\t" << y << endl;
}
