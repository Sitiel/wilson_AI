

#include "performances.hpp"
using namespace std;
using namespace std::chrono;

void testPerfFloatVsDouble(){
    high_resolution_clock::time_point start = high_resolution_clock::now();
    float fa = 0;
    float fb = 1000000;
    float fc = 1000000;
    float fd = 1;
    for(int i = 0 ; i < 1000000 ; i++){
        fa += i;
        fb -= i;
        fc /= i;
        fd *= i;
    }
    
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(end - start);
    std::cout << "Float took " << time_span.count() << " seconds." << endl;
    
    /// END FLOAT
    
    start = high_resolution_clock::now();
    double da = 0;
    double db = 1000000;
    double dc = 1000000;
    double dd = 1;
    for(int i = 0 ; i < 1000000 ; i++){
        da += i;
        db -= i;
        dc /= i;
        dd *= i;
    }
    
    end = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(end - start);
    std::cout << "Double took " << time_span.count() << " seconds." << endl;
}
