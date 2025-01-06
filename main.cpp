#include <iostream>
#include <cstring>
#include <fstream>
#define PRODUCT_ARRAY_SIZE 1024

struct product {
    std::string name="";
    int stock=0;
};

product productArray[PRODUCT_ARRAY_SIZE]; // Allocating a large sum of memory since I can't dynamically enlarge the array later on


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
            std::cout << "Stok adedi: " << productArray[i].stock<< std::endl;
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
    if(updateIndex == -1) {
        if(!confirmation("Bu isimde bir ürün bulunamadı. Yeni bir ürün oluşturmak istiyor musunuz?")) return;

        std::cout << "Ürünün stok adedini giriniz: ";
        std::cin >> productCount;
        for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
            if(productArray[i].name == "" && productArray[i].stock == 0){ // Finding an unused slot
                productArray[i].name = name;
                productArray[i].stock = productCount;
                break;
            }
        }
    } else {
        std::cout << "Ürünün stok adedini giriniz: ";
        std::cin >> productCount;
        productArray[updateIndex].name = name;
        productArray[updateIndex].stock = productCount;
    }
}


void listProducts() {
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if(productArray[i].name == "" && productArray[i].stock == 0) continue; // Skip empty slots
        std::cout << "------------------------------" << std::endl;
        std::cout << "Ürün adı: " << productArray[i].name << std::endl;
        std::cout << "Stok adedi: " << productArray[i].stock<< std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}


void deleteProduct(std::string name) {
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        if(productArray[i].name == name) {
            if(!confirmation("Bu ürünü silmek istediğinizden emin misiniz?")) return;
            productArray[i].name = "";
            productArray[i].stock = 0;
        }
    }
}


void saveProducts(){
    std::ofstream file("data.dat", std::ios::binary);
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        int nameLength = productArray[i].name.size();
        file.write(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));

        // Since strings can't be serialized easily, I'm writing them to the file char by char.
        for(int j = 0; j < nameLength ; j++) {
            char c = productArray[i].name[j];
            file.write(&c, sizeof(char));
        }

        file.write(reinterpret_cast<char*>(&productArray[i].stock), sizeof(productArray[i].stock));
        }

    file.close();
}


void loadProducts(){
    std::ifstream file("data.dat", std::ios::binary);
    if(!file) return;
    for(int i=0 ; i<PRODUCT_ARRAY_SIZE ; i++) {
        int nameLength = productArray[i].name.size();
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        if (file.eof()) break;

        productArray[i].name = ""; // Reset the current string

        for (int j = 0; j < nameLength; ++j) {
            char c;
            file.read(&c, sizeof(char));
            if (file.eof()) break;
            productArray[i].name += c;
        }

        file.read(reinterpret_cast<char*>(&productArray[i].stock), sizeof(productArray[i].stock));
        if (file.eof()) break;

    }

    file.close();
}


void select(bool& quit) {
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
    std::cout << std::endl << "Elektrikçi Otomasyon Sistemi V0.1" << std::endl;

    loadProducts();
    bool quit = false; // This is unironically easier than returning from select(). Took inspiration from SDL2
    while(!quit)
        select(quit);
    return 0;
}
