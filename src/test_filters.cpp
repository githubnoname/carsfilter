#include"cxpipeline.h"
#include"testutils.h"


enum etype{ test_all, test_lada, test_new, test_cheap_old };

static std::pair<std::list<SXCar*>, std::list<SXCar*>> make_cars(etype aType){
    auto lada_granta = new SXCar("Lada", "Granta", 2010, 4000);
    auto lada_priora = new SXCar("Lada", "Priora", 2000, 3000);
    auto toyota_corolla = new SXCar("Toyota", "Corolla", 2005, 6000);
    auto bmw_x42 = new SXCar("BMW", "X42", 2015, 42000);
    std::list<SXCar*> orig = { lada_granta,
                               lada_priora,
                               toyota_corolla,
                               bmw_x42 };
    switch(aType){
    case test_all:
        return { orig, orig };
    case test_lada:
        return { orig, { lada_granta, lada_priora } };
    case test_new:
        return { orig, { lada_granta, bmw_x42 } };
    case test_cheap_old:
        return { orig, { lada_priora } };
    }
    return { };
}


struct CXCollectJob : public AXPipeJob {
    void processCar(SXCar *aCar) override {
        mStore.push_back(aCar);
    }

    std::list<SXCar*> mStore;
};


void print_cars(std::list<SXCar*> const &aCars){
    for(auto const &x : aCars){
        fprintf(stderr, "%p: ", (void*)x);
        x->println(stderr);
    }
}


bool run_test(CXPipeline *p, etype aType){
    auto c = new CXCollectJob;
    p->add(c);
    auto cars = make_cars(aType);
    for(auto const &x : cars.first)
        p->head()->processCar(x);
    if(c->mStore != cars.second){
        fprintf(stderr, "Expected:\n");
        print_cars(cars.second);
        fprintf(stderr, "But got:\n");
        print_cars(c->mStore);
        ERROR("Mismatch");
    }
    return true;
}


bool test_filters_basic(){
    CXPipeline p;
    // First of all, play with wrong filters
    // Bad name
    if(p.addFilter("Bbbrand", "==", "xx") != 0)
        ERROR("Pipeline shouldn't create wrong filters");
    // Wrong operation
    if(p.addFilter("model", "==", "xx") != 0)
        ERROR("Pipeline shouldn't create wrong filters");
    // Wrong operation
    if(p.addFilter("price", "!=", "xx") != 0)
        ERROR("Pipeline shouldn't create wrong filters");

    // And check empty pipeline
    return run_test(&p, test_all);
}


bool test_filters_lada(){
    // Filter Lada cars
    CXPipeline p;
    p.addFilter("brand", "==", "Lada");
    return run_test(&p, test_lada);
}


bool test_filters_new(){
    // Filter new cars
    CXPipeline p;
    p.addFilter("issued", ">=", "2010");
    return run_test(&p, test_new);
}


bool test_filters_cheap(){
    // Two filters. Old and cheap.
    CXPipeline p;
    p.addFilter("issued", "<=", "2006");
    p.addFilter("price", "<", "5000");
    return run_test(&p, test_cheap_old);
}


int main(){
    return !(TEST(test_filters_basic) &&
             TEST(test_filters_lada)  &&
             TEST(test_filters_new)   &&
             TEST(test_filters_cheap));
}
