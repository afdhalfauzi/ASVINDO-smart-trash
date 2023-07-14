#include <Arduino.h>
#include <Fuzzy.h>

Fuzzy *myFuzzy = new Fuzzy();

struct FUZZY
{
    bool begin();
    bool setInput(int index, float input);
    bool fuzify();
    int defuzify();
};

bool FUZZY::begin()
{

    // Inisialisasi Fuzzy Set Input
    FuzzySet *sangattinggi = new FuzzySet(0, 8.5, 8.5, 17);
    FuzzySet *tinggi = new FuzzySet(15, 23.5, 23.5, 32);
    FuzzySet *sedang = new FuzzySet(30, 38.5, 38.5, 47);
    FuzzySet *pendek = new FuzzySet(45, 57.5, 57.5, 70);

    // Inisialisasi Fuzzy Set Output
    FuzzySet *kosong = new FuzzySet(0, 9, 19, 28);
    FuzzySet *sedikit = new FuzzySet(25, 34, 44, 53);
    FuzzySet *sedang_sampah = new FuzzySet(50, 59, 69, 78);
    FuzzySet *penuh = new FuzzySet(75, 83, 92, 100);

    // Instantiating a FuzzyInput object
    FuzzyInput *distance = new FuzzyInput(1);
    // Including the FuzzySet into FuzzyInput
    distance->addFuzzySet(pendek);
    distance->addFuzzySet(sedang);
    distance->addFuzzySet(tinggi);
    distance->addFuzzySet(sangattinggi);
    // Including the FuzzyInput into Fuzzy
    myFuzzy->addFuzzyInput(distance);

    // Instantiating a FuzzyOutput objects
    FuzzyOutput *kategori = new FuzzyOutput(1);
    // Including the FuzzySet into FuzzyOutput
    kategori->addFuzzySet(kosong);
    kategori->addFuzzySet(sedikit);
    kategori->addFuzzySet(sedang_sampah);
    kategori->addFuzzySet(penuh);
    // Including the FuzzyOutput into Fuzzy
    myFuzzy->addFuzzyOutput(kategori);

    //--------------------------MMEMBUAT FUZZY RULE-----------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------------------------------

    // FuzzyRule11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
    FuzzyRuleAntecedent *ifDistancependek = new FuzzyRuleAntecedent();
    // Creating a FuzzyRuleAntecedent with just a single FuzzySet
    ifDistancependek->joinSingle(pendek);
    // Instantiating a FuzzyRuleConsequent objects
    FuzzyRuleConsequent *thenkategorikosong = new FuzzyRuleConsequent();
    // Including a FuzzySet to this FuzzyRuleConsequent
    thenkategorikosong->addOutput(kosong);
    // Instantiating a FuzzyRule objects
    FuzzyRule *fuzzyRule01 = new FuzzyRule(1, ifDistancependek, thenkategorikosong);
    // Including the FuzzyRule into Fuzzy
    myFuzzy->addFuzzyRule(fuzzyRule01);

    // FuzzyRule22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222
    FuzzyRuleAntecedent *ifDistancesedang = new FuzzyRuleAntecedent();
    // Creating a FuzzyRuleAntecedent with just a single FuzzySet
    ifDistancesedang->joinSingle(sedang);
    // Instantiating a FuzzyRuleConsequent objects
    FuzzyRuleConsequent *thenkategorisedikit = new FuzzyRuleConsequent();
    // Including a FuzzySet to this FuzzyRuleConsequent
    thenkategorisedikit->addOutput(sedikit);
    // Instantiating a FuzzyRule objects
    FuzzyRule *fuzzyRule02 = new FuzzyRule(2, ifDistancesedang, thenkategorisedikit);
    // Including the FuzzyRule into Fuzzy
    myFuzzy->addFuzzyRule(fuzzyRule02);

    // FuzzyRule3333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
    FuzzyRuleAntecedent *ifDistancetinggi = new FuzzyRuleAntecedent();
    // Creating a FuzzyRuleAntecedent with just a single FuzzySet
    ifDistancetinggi->joinSingle(tinggi);
    // Instantiating a FuzzyRuleConsequent objects
    FuzzyRuleConsequent *thenkategorisedang_sampah = new FuzzyRuleConsequent();
    // Including a FuzzySet to this FuzzyRuleConsequent
    thenkategorisedang_sampah->addOutput(sedang_sampah);
    // Instantiating a FuzzyRule objects
    FuzzyRule *fuzzyRule03 = new FuzzyRule(3, ifDistancetinggi, thenkategorisedang_sampah);
    // Including the FuzzyRule into Fuzzy
    myFuzzy->addFuzzyRule(fuzzyRule03);

    // FuzzyRule4444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444
    FuzzyRuleAntecedent *ifDistancesangattinggi = new FuzzyRuleAntecedent();
    // Creating a FuzzyRuleAntecedent with just a single FuzzySet
    ifDistancesangattinggi->joinSingle(sangattinggi);
    // Instantiating a FuzzyRuleConsequent objects
    FuzzyRuleConsequent *thenkategoripenuh = new FuzzyRuleConsequent();
    // Including a FuzzySet to this FuzzyRuleConsequent
    thenkategoripenuh->addOutput(penuh);
    // Instantiating a FuzzyRule objects
    FuzzyRule *fuzzyRule04 = new FuzzyRule(4, ifDistancesangattinggi, thenkategoripenuh);
    // Including the FuzzyRule into Fuzzy
    myFuzzy->addFuzzyRule(fuzzyRule04);

    return true;
}

bool FUZZY::setInput(int index, float input)
{
    return myFuzzy->setInput(index, input);
}

bool FUZZY::fuzify()
{
    return myFuzzy->fuzzify();
}

int FUZZY::defuzify()
{
    return myFuzzy->defuzzify(1);
}