#include"carstorage.h"

std::list<SXCar> loadCars(char const *){
    return {
        SXCar("Toyota", "corolla", 1990, 9999),
        SXCar("Toyota", "corolla", 2000, 15000),
        SXCar("Lada", "Granta", 2010, 7000),
        SXCar("Mitsubishi", "Lancer", 2010, 20000)
    };
}
