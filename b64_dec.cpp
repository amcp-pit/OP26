#include <cstdint>
#include <iostream>
#include <fstream>

void b64encode(uint8_t a[3], uint8_t b[4]){
    b[0] = a[0] >> 2;
    b[1] = ((a[0] & 3) << 4) | (a[1] >> 4);
    b[2] = ((a[1] & 15) << 2) | ( a[2] >> 6);
    b[3] = a[2] & 63;
    /*
    uint8_t table[] = {'A','B', ... ,'/'};
    b[0] = table[b[0]];
    b[1] = table[b[1]];
    b[2] = table[b[2]];
    b[3] = table[b[3]];
*/
    for (int k = 0; k < 4; ++k ){
        if ( b[k] < 26 ){
            b[k] = 'A' + b[k];
        }
        else if ( b[k] < 52 ){
            b[k] = 'a' + b[k] - 26;
        }
        else if ( b[k] < 62 ){
            b[k] = '0' + b[k] - 52;
        }
        else {
            if ( b[k] == 62 ){
                b[k] = '+';
            }
            else if ( b[k] == 63 ){
                b[k] = '/';
            }
            else{
                std::cout << "ACHTUNG!!! Wrong byte";
            }
        }
    }

}
void b64decode(uint8_t b[4], uint8_t a[3] ){
    for ( int k = 0; k < 4; ++k ){
        if ( 'A' <= b[k] && b[k] <= 'Z' ) {
            b[k] -= 'A';
        }
        else if ( 'a' <= b[k] && b[k] <= 'z' ){
            b[k] -= 'a';
        }
        else if ( '0' <= b[k] && b[k] <= '9' ) {
            b[k] -= '0';
        }
        else if ( b[k] == '+' ) {
            b[k] = 62;
        }
        else if ( b[k] == '/' ){
            b[k] = 63;
        }
        else {
            std::cout << "ACHTUNG!!! Wrong byte";
        }
    }

    a[0] = (b[0] << 2) | ( b[1] >> 4 );
    a[1] = (b[1] << 4) | ( b[2] >> 2 );
    a[2] = (b[2] << 6) | b[3];
}




int main(int argc, char* argv[]){
    if ( argc != 3 ){
        std::cout << argc << std::endl;
        std::cout << "prog file2dec fileres" << std::endl;
            return 1;
    }
    std::ifstream ifs(argv[1]);
    if ( !ifs.is_open() ){
        std::cout << "Can't open file" << argv[1] << std::endl;
        return -1;
    }

    std::fstream ofs(argv[2]);
    if ( !ofs.is_open() ){
        std::cout << "Can't open file" << argv[2] << std::endl;
        return -1;
    }
    while ( true /*!ifs.eof()*/){
        uint8_t b[4]={};
        ifs.read(reinterpret_cast<char*>(b),sizeof(b));
        long bytesRead = ifs.gcount();
        if ( bytesRead == 0 ) {
            std::cout << "Finished " << std::endl;
            break;
        }

        for (int k = 0; k < 4; ++k){
            std::cout << b[k] << '|';
        }

        uint8_t a[3]={};
        b64decode(b,a);
        std::cout << "Decoded" << std::endl;
        for (int k = 0; k < 3; ++k){
            std::cout << static_cast<int>(a[k]) << '|';
        }

        std::cout << std::endl;
        ofs.write(reinterpret_cast<char*>(a),sizeof(a));
    }
    ifs.close();
    ofs.close();
    return 0;
    }