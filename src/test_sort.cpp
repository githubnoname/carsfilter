#include"cxpipeline.h"
#include"testutils.h"


enum etype{ test_price_asc, test_brand_asc_price_desc, test_lada_filter_price_asc, test_year_desc_price_asc_no_cheap };

static std::pair<std::list<SXCar*>, std::list<SXCar*>> make_cars(etype aType){
    auto lada_granta = new SXCar("Lada", "Granta", 2010, 4000);
    auto lada_priora = new SXCar("Lada", "Priora", 2000, 3000);
    auto toyota_corolla = new SXCar("Toyota", "Corolla", 2005, 6000);
    auto bmw_x42 = new SXCar("BMW", "X42", 2015, 42000);
    auto lada_xray = new SXCar("Lada", "Xray", 2015, 7000);
    auto toyota_camry = new SXCar("Toyota", "Camry", 2015, 20000);
    std::list<SXCar*> orig = { lada_granta,
                               lada_priora,
                               toyota_corolla,
                               bmw_x42,
                               lada_xray,
                               toyota_camry };
    switch(aType){
    case test_price_asc:
        return { orig, { lada_priora, lada_granta, toyota_corolla, lada_xray, toyota_camry, bmw_x42, 0 } };
    case test_brand_asc_price_desc:
        return { orig, { bmw_x42, lada_xray, lada_granta, lada_priora, toyota_camry, toyota_corolla, 0 } };
    case test_lada_filter_price_asc:
        return { orig, { lada_priora, lada_granta, lada_xray, 0 } };
    case test_year_desc_price_asc_no_cheap:
        return { orig, { lada_xray, toyota_camry, bmw_x42, toyota_corolla, 0 } };
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
        if(x != 0)
            x->println(stderr);
        else
            fprintf(stderr, "<nil>\n");
    }
}


bool run_test(CXPipeline *p, etype aType){
    p->applySorter();
    auto c = new CXCollectJob;
    p->add(c);
    auto cars = make_cars(aType);
    for(auto const &x : cars.first)
        p->head()->processCar(x);
    // Sort works only after data ends
    p->head()->processCar(0);
    if(c->mStore != cars.second){
        fprintf(stderr, "Expected:\n");
        print_cars(cars.second);
        fprintf(stderr, "But got:\n");
        print_cars(c->mStore);
        ERROR("Mismatch");
    }
    return true;
}


bool test_sort_price_asc(){
    // Ascending sort by price
    CXPipeline p;
    p.addSortRule("asc", "price");
    return run_test(&p, test_price_asc);
}


bool test_sort_brand_asc_price_desc(){
    // Group (ascending sort) by brand, descending sort by price
    CXPipeline p;
    p.addSortRule("asc", "brand");
    p.addSortRule("desc", "price");
    return run_test(&p, test_brand_asc_price_desc);
}


bool test_sort_lada_filter_price_asc(){
    // Filter Lada, ascending sort by price
    CXPipeline p;
    p.addFilter("brand", "==", "Lada");
    p.addSortRule("asc", "price");
    return run_test(&p, test_lada_filter_price_asc);
}


bool test_sort_year_desc_price_asc_no_cheap(){
    // Remove cheap cars, group (descending sort) by issued date, ascending sort by price
    CXPipeline p;
    p.addSortRule("desc", "issued");
    p.addFilter("price", ">", "5000");
    p.addSortRule("asc", "price");
    return run_test(&p, test_year_desc_price_asc_no_cheap);
}


int main(){
    return !(TEST(test_sort_price_asc) &&
             TEST(test_sort_brand_asc_price_desc) &&
             TEST(test_sort_lada_filter_price_asc) &&
             TEST(test_sort_year_desc_price_asc_no_cheap));
}
