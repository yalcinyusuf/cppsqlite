#include <iostream>
#include "stdint.h"
#include <string> 
using namespace std;

struct VehicleInfo {
    int64_t stationId;
    string carBrand;
    int64_t speed;
};
struct Person {
    int64_t ID;
    string name;
    string email;
    VehicleInfo* Car;
};


