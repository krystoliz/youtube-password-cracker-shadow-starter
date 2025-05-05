#include <iostream>
#include <crypt.h>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>

const std::string CHARSET = "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "0123456789";

std::vector<std::string> getHashSalt(const std::string &username){
    std::string filename = "shadow";
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> hash_and_salt;

    if(!file.is_open()){
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return hash_and_salt;
    }

    while(std::getline(file,line)){
        if (line.rfind(username,0) == 0){
            std::cout << "Found user: " << username << std::endl;

            size_t first_colon_pos = line.find(':');
            size_t second_colon_pos = line.find(':', first_colon_pos + 1);

            std::string full_hash = line.substr(first_colon_pos + 1, second_colon_pos - first_colon_pos - 1);

            std::cout << "Full hash: " << full_hash << std::endl;

            size_t third_dollar_pos = full_hash.find('$', full_hash.find('$', full_hash.find('$') + 1) + 1);

            if (third_dollar_pos == std::string::npos){
                std::cerr << "Error: could not find third $ symbol." << std::endl;
                break;
            }

            std::string salt = full_hash.substr(0, full_hash.find('$', third_dollar_pos +1));

            std::cout << "Extracted salt: " << salt << std::endl;

            hash_and_salt.push_back(salt);
            hash_and_salt.push_back(full_hash);

            break;
        }
    }
    file.close();
    return hash_and_salt;

}

bool check_pass(const char *password, const char *salt,const char *combined_salt_hash){
    char *generated_hash = crypt(password, salt);

    if (strcmp(generated_hash, combined_salt_hash) == 0){
        std::cout << "Password: " << password << " matched the provided hash!" << std::endl;
        return true;
    }
    else{
        return false;
    }
}

bool brute_force_crack(const std::string &current, int password_length, const char *salt, const char *combined_salt_hash){

    bool result=false;

    if(current.length() == password_length){
        result = check_pass(current.c_str(), salt, combined_salt_hash);
        return result;
    }

    for(char c : CHARSET){
        result = brute_force_crack(current + c, password_length,salt, combined_salt_hash);

        if (result){
            return result;
        }
    }
    return false;
}

bool password_list_crack(std::string password_file_name, const char *salt, const char *combined_salt_hash){
    std::ifstream password_file(password_file_name);
    std::string password;
    bool result {false};

    if(!password_file.is_open()){
        std::cerr << "Error: Unable to open file: " << password_file_name << std::endl;
        
    }
    while(std::getline(password_file, password)){
        result = check_pass(password.c_str(), salt, combined_salt_hash);

        if (result){
            return result;
        }
    }

    return result;
}

int main(){

    std::vector<std::string> hash_and_salt = getHashSalt("victim1");

    const char *salt = hash_and_salt.at(0).c_str();
    const char *combined_salt_hash = hash_and_salt.at(1).c_str();
    
    // std::cout << salt << std::endl;
    // std::cout << combined_salt_hash << std::endl;

    // check_pass("aab", salt, combined_salt_hash);

    //Brute force attack
    // int max_password_length = 16;

    // for(int i = 0; i < max_password_length; i++){
    //     std::cout << "Starting password crack for length "<< i << std::endl;
    //     bool result = brute_force_crack("", i, salt, combined_salt_hash);

    //     if(result){
    //         break;
    //     }
    // }

    // bool result = brute_force_crack("", 1, salt, combined_salt_hash);

    // std::cout << result << std::endl;

    //Password list

    password_list_crack("10-million-password-list-top-1000000.txt", salt, combined_salt_hash);

    return 0;
}