/*
    Yağız Pekmezcan (2412721011)
    Elektrikçi Otomasyon Sistemi
*/
#include <iostream>
#include <cstring>
#include <fstream>
#define PRODUCT_ARRAY_SIZE 1024

struct product {
    std::string name="";
    int stock=0;
    int unitPrice=0;
};

/* GLOBALS */
product productArray[PRODUCT_ARRAY_SIZE]; // Allocating a large sum of memory since I can't dynamically enlarge the array later on
std::string currency = "TRY";


bool confirmation(std::string message) {
    std::string res;
    std::cout << message << " [e/h] ";
    std::cin >> res;

    if(res == "h" || res == "H") return false;
    if(res == "e" || res == "E") return true;
    return confirmation(message);
}


void searchProducts(std::string name) {
    bool found = false;
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) { // I miss range loops
        if(productArray[i].name == name) {
            std::cout << "------------------------------" << std::endl;
            std::cout << "Ürün adı: " << productArray[i].name << std::endl;
            std::cout << "Stok adedi: " << productArray[i].stock << std::endl;
            std::cout << "Birim fiyatı: " << productArray[i].unitPrice << " " << currency << std::endl;
            std::cout << "------------------------------" << std::endl;
            found = true;
        }
    }
    if(!found) std::cout << "Bu isimde hiçbir ürün bulunamadı." << std::endl;
}


void updateProduct(std::string name) {
    int updateIndex = -1;
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if(productArray[i].name == name) {
            updateIndex = i;
        }
    }

    int productCount=0;
    int productPrice=0;
    if(updateIndex == -1) {
        if(!confirmation("Bu isimde bir ürün bulunamadı. Yeni bir ürün oluşturmak istiyor musunuz?")) return;

        std::cout << "Ürünün stok adedini giriniz: ";
        std::cin >> productCount;
        std::cout << "Ürünün birim fiyatını giriniz: ";
        std::cin >> productPrice;
        for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
            if(productArray[i].name == "" && productArray[i].stock == 0){ // Finding an unused slot
                productArray[i].name = name;
                productArray[i].stock = productCount;
                productArray[i].unitPrice = productPrice;
                break;
            }
        }
    } else {
        std::cout << "Ürünün stok adedini giriniz(değiştirmek istemiyorsanız -1 giriniz): ";
        std::cin >> productCount;
        std::cout << "Ürünün birim fiyatını giriniz(değiştirmek istemiyorsanız -1 giriniz): ";
        std::cin >> productPrice;
        if(productCount != -1) productArray[updateIndex].stock = productCount;
        if(productPrice != -1) productArray[updateIndex].unitPrice = productPrice;
    }
}


void listProducts() {
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if(productArray[i].name == "" && productArray[i].stock == 0) continue; // Skip empty slots
        std::cout << "------------------------------" << std::endl;
        std::cout << "Ürün adı: " << productArray[i].name << std::endl;
        std::cout << "Stok adedi: " << productArray[i].stock << std::endl;
        std::cout << "Birim fiyatı: " << productArray[i].unitPrice << " " << currency << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}


void deleteProduct(std::string name) {
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if(productArray[i].name == name) {
            if(!confirmation("Bu ürünü silmek istediğinizden emin misiniz?")) return;
            productArray[i].name = "";
            productArray[i].stock = 0;
            productArray[i].unitPrice = 0;
        }
    }
}


void saveProducts(){
    /*
    Save and load follows the following order:
    Currency
    Products
        *-> name
        *-> stock
        *-> price
    */
    std::ofstream file("data.dat", std::ios::binary);
    int currencyLength = currency.size();
    file.write(reinterpret_cast<char*>(&currencyLength), sizeof(currencyLength));

    for(int i = 0; i < currencyLength ; i++) {
        char c = currency[i];
        file.write(&c, sizeof(char));
    }

    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        int nameLength = productArray[i].name.size();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

        // Since strings can't be serialized easily, I'm writing them to the file char by char.
        for(int j = 0; j < nameLength ; j++) {
            char c = productArray[i].name[j];
            file.write(&c, sizeof(char));
        }

        file.write(reinterpret_cast<char*>(&productArray[i].stock), sizeof(productArray[i].stock));
        file.write(reinterpret_cast<char*>(&productArray[i].unitPrice), sizeof(productArray[i].unitPrice));
        }

    std::cout << "Dosya kaydedildi!" << std::endl;
    file.close();
}


void loadProducts(){
    std::ifstream file("data.dat", std::ios::binary);
    if(!file) return;
    // Load currency
    currency = "";
    int currencyLength = 0;
    file.read(reinterpret_cast<char*>(&currencyLength), sizeof(currencyLength));
    for (int i = 0; i < currencyLength; ++i) {
        char c;
        file.read(&c, sizeof(char));
        if (file.eof()) break; // How benign
        currency += c;
    }
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if (file.eof()) break;
        int nameLength = productArray[i].name.size();
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

        productArray[i].name = ""; // Reset the current name, just in case

        for (int j = 0; j < nameLength; ++j) {
            char c;
            file.read(&c, sizeof(char));
            if (file.eof()) break;
            productArray[i].name += c;
        }
        // Stock and unit prices
        file.read(reinterpret_cast<char*>(&productArray[i].stock), sizeof(productArray[i].stock));
        file.read(reinterpret_cast<char*>(&productArray[i].unitPrice), sizeof(productArray[i].unitPrice));
        if (file.eof()) break;
    }
    file.close();
}


void findTotalUnits() {
    int total=0;
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if(productArray[i].name == "" && productArray[i].stock == 0) continue;
        total += productArray[i].stock;
    }
    std::cout << "Envanterde toplam " << total << " adet ürün bulunmakta." << std::endl;
}


void findTotalValue() {
    int total=0;
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if(productArray[i].name == "" && productArray[i].stock == 0) continue;
        total += productArray[i].stock * productArray[i].unitPrice;
    }
    std::cout << "Envanterde toplam " << total << " " << currency << " değerinde ürün bulunmakta." << std::endl;
}


void advancedMenu() {
    std::cout << "[1]- Toplam ürün adedi" << std::endl;
    std::cout << "[2]- Toplam envanter değeri" << std::endl;
    std::cout << "[3]- Para birimini değiştir" << std::endl;
    std::cout << "[4]- Envanteri boşalt" << std::endl;
    std::cout << "[0]- Geri dön" << std::endl;
    int sel=0;
    std::cin >> sel;

    std::string temp;
    std::getline(std::cin, temp);
    switch(sel) {
    case 1: // Total unit count
    {
        findTotalUnits();
        break;
    }
    case 2: // Total inventory value
    {
        findTotalValue();
        break;
    }
    case 3: // Change currenct
    {
        std::string target;
        std::cout << "Kullanmak istediğiniz para biriminin adını giriniz(Örnek: TRY, USD, EUR): ";
        std::cin >> target;
        currency = target;
        break;
    }
    case 4: // Reset inventory
    {
        if(!confirmation("Bu işlem envanterdeki her şeyi sıfırlayacaktır. Devam etmek istediğinize emin misiniz?")) break;
        for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
            productArray[i].name = "";
            productArray[i].stock = 0;
            productArray[i].unitPrice = 0;
        }
        std::cout << "Envanter başarıyla sıfırlandı!";
        break;
    }

    }
}


void select(bool& quit) {
    std::cout << std::endl << "------------------------------" << std::endl;
    std::cout << "[1]- Ara" << std::endl;
    std::cout << "[2]- Ekle/Güncelle" << std::endl;
    std::cout << "[3]- Sil" << std::endl;
    std::cout << "[4]- Listele" << std::endl;
    std::cout << "[5]- Kaydet" << std::endl;
    std::cout << "[9]- Gelişmiş" << std::endl;
    std::cout << "[0]- Çıkış" << std::endl;
    int sel=0;
    std::cin >> sel;

    std::string temp;
    std::getline(std::cin, temp); // cin leaves a newline at the end of the buffer. Flush it with a redundant getline

    switch(sel) {
    case 1: // Search
        {
        std::string target;
        std::cout << "Aramak istediğiniz ürünün adını giriniz: ";
        std::getline(std::cin, target);
        searchProducts(target);
        break;
        }


    case 2: // Add/Edit
        {
        std::string target;
        std::cout << "Düzenlemek ya de eklemek istediğiniz ürünün adını giriniz: ";
        std::getline(std::cin, target);
        updateProduct(target);
        break;
        }

    case 3: // Delete
        {
        std::string target;
        std::cout << "Silmek istediğiniz ürünün adını giriniz: ";
        std::getline(std::cin, target);
        deleteProduct(target);
        break;
        }

    case 4: // List
        {
        listProducts();
        break;
        }

    case 5: // Save
        {
        saveProducts();
        break;
        }

    case 9: // Advanced
        {
        advancedMenu();
        break;
        }

    case 0: // Quit
        {
        quit = true;
        break;
        }

    }
}


int main() {
    std::cout << "________________________" << std::endl;
    std::cout << "___  ____/_  __ \\_  ___/" << std::endl;
    std::cout << "__  __/  _  / / /____ \\ " << std::endl;
    std::cout << "_  /___  / /_/ /____/ / " << std::endl;
    std::cout << "/_____/  \\____/ /____/  " << std::endl;
    std::cout << std::endl << "Elektrikçi Otomasyon Sistemi V0.2" << std::endl;

    loadProducts();
    bool quit = false; // This is unironically easier than returning from select(). Took inspiration from SDL2
    while(!quit)
        select(quit);
    return 0;
}
