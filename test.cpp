#include <iostream>
#include <bitset>

int main(){
    char x = 'a';
    char y = 'b';
    int sum = x + y;   
    char a[] = { 'C', 'O', 'D', 'E' };


    std::cout << sizeof(unsigned char) << std::endl;
    std::cout << sizeof(char) << std::endl;
    std::bitset<8> one(8);
    std::bitset<8> two(11);
    std::bitset<16> suma(one+two);
    std::cout << one <<std:: endl;
    std::cout << two <<std:: endl;
    std::cout << suma<<std:: endl;


/*     std::cout << x + y << std::endl; */
/*     std::cout << x << std::endl; */
/*     std::cout << y << std::endl; */
/*     std::bitset<8> mniejsze(x); */
/*     std::bitset<32> beka(sum); */
/*     std::cout << "bitset x: " << mniejsze <<std::endl; */
/*     std::cout << "bitset x+y: " << beka <<std::endl; */
/*     /1* std::cout<<"sum: "<<(double)x +(double)y<<" type: "<<typeid() std::endl; *1/ */
/*     std::cout <<"char(1)="<< a[0] << std::endl; */



    return 0;
}
