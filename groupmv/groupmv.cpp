#include <iostream>
#include <filesystem>
#include <map>
#include <iomanip>
#include <unistd.h>
#include <vector>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::filesystem::exists;
using std::filesystem::directory_iterator;
using std::filesystem::rename;
using std::filesystem::path;
using std::filesystem::is_directory;
using std::setw;
using std::left;

bool dry_mode;

void print_help();



int check_args(const vector<string> &args){

     if(args.size() == 6){
        string dry = args.at(5);
        if(dry == "-u")
            dry_mode = true;
        else{
            cout<<"Last argument invalid: expected -u or none"<<endl;
            return -1;}}

    if(args.size() == 2){
        string help = args.at(1);
        if(help == "-h"){
            print_help();
            return 1;
        }
    }

    
    if(args.size() < 5){
        
           cerr<<"Too little arguments, type groupmv -h for help"<<endl;
            return -1;
    }

    else if(!exists(args.at(1))){
        cerr<<"File does not exist"<<endl;
        return -1;
    }

    else if(!is_directory(args.at(1))){
        cerr<<"File exists but is not a directory"<<endl;
    }

    else if(access(args.at(1).c_str(),W_OK)  != 0 && access(args.at(1).c_str(),R_OK) !=0){
        cerr<<"File found and is a directory but you dont have read nor write premissions"<<endl;
        return -1;
    }

    else if(((access(args.at(1).c_str(),R_OK))) != 0){
        cerr<<"File found and is a directory but you do not have read premissions"<<endl;
        return -1;
    }

    else if(((access(args.at(1).c_str(),W_OK))) != 0){
        cerr<<"File found and is a directory but you do not have write premissions"<<endl;
        return -1;
    }

    
    return 0;
    

}

void print_paths(const map<string,string> &file_paths){

    cout<<left<<setw(25)<<"Original:"
    <<setw(25) <<"Changed(preview only)"<<endl;
    cout << string(50, '-') << endl;
    for(const auto &pair : file_paths){
        cout << left << setw(25) << pair.first
        << setw(25) << pair.second
        << endl;

    }

}



void trailing_slash(path &directory_path){

    if(directory_path.string().back() != '/')
        directory_path = directory_path / "";


}


void append_on_beginning(const path &directory_path,const string &string_to_change){


    

    auto get_file_paths = [&](){

        map<string,string> file_paths;

        for(const auto &entity : directory_iterator(directory_path)){


            string filename = entity.path().filename().string();
            string changed_path = directory_path.string() + string_to_change + filename;
            if(exists(changed_path))
                continue;
            file_paths.emplace(entity.path(), changed_path);   
            }
            return file_paths;
            
        };






            map<string,string> file_paths = get_file_paths();

            if(dry_mode)
                print_paths(file_paths);
                

            else{
                for(const auto &pair : file_paths)
                    rename(pair.first, pair.second);
            }


}
























void append_on_end(const path &directory_path, const string &string_to_change){


    auto get_file_paths = [&](){
        map<string,string> file_paths;
        for(const auto &entity : directory_iterator(directory_path)){

        string filename = entity.path().filename().string();
        string changed_path = directory_path.string() +  filename + string_to_change;
        if(exists(changed_path))
                continue;
        file_paths.emplace(entity.path(),changed_path);
        }
        return file_paths;

    };




    map<string,string> file_paths = get_file_paths();

    if(dry_mode)
         print_paths(file_paths);

    else{
        for(const auto &pair: file_paths){
            rename(pair.first,pair.second);
        }
    }

    

    }






void delete_on_end(const path &directory_path, const string &string_to_change){

    auto get_file_paths = [&](){
        map<string,string> file_paths;
        for(const auto &entity : directory_iterator(directory_path)){

            string filename = entity.path().filename().string();
            string changed_path = directory_path.string();      
            size_t index = filename.rfind(string_to_change);
            if(index == std::string::npos || filename.substr(index,filename.size()) != string_to_change)
                continue;

            string changed_filename = filename.substr(0,index);
            changed_path = changed_path + changed_filename;

            if(exists(changed_path))
                continue;

            file_paths.emplace(entity.path(), changed_path);
            
         }  
         return file_paths;    
    
    };



    map<string,string> file_paths = get_file_paths();

    if(dry_mode)
         print_paths(file_paths);
        

    else{
        for(const auto &pair : file_paths)
            rename(pair.first,pair.second);
        
         }
    }


















void delete_on_beginning(const path &directory_path, const string &string_to_change){

    auto get_file_paths = [&](){
        map<string,string> file_paths;
        for(const auto &entity : directory_iterator(directory_path)){

            string filename = entity.path().filename().string();
            string changed_path = directory_path.string();
            int index = string_to_change.size();

            if(filename.find(string_to_change) == std::string::npos || filename.find(string_to_change) != 0)
                continue;

            string changed_filename = filename.substr(index, filename.size());
            changed_path = changed_path + changed_filename;
            
            if(exists(changed_path))
                continue;

            file_paths.emplace(entity.path(),changed_path);

        }
    
            return file_paths;
            
    };



    map<string,string> file_paths = get_file_paths();

    if(dry_mode)
        print_paths(file_paths);

    else{
        for(const auto &pair : file_paths)
            rename(pair.first,pair.second);
        
        }

    }


    void delete_middle(const path &directory_path, const string &string_to_change){

        auto get_file_paths = [&](){
        map<string,string> file_paths;
        for(const auto &entity : directory_iterator(directory_path)){

            string filename = entity.path().filename().string();
            string changed_path = directory_path.string();

            if(filename.find(string_to_change) == std::string::npos)
                continue;                                                   
                                                                            
            string first_part = filename.substr(0,filename.find(string_to_change));
            string second_part = filename.substr((first_part.size() + string_to_change.size()),filename.size());
            changed_path = changed_path + first_part + second_part;
            
            if(exists(changed_path))
                continue;

            file_paths.emplace(entity.path(),changed_path);

        }
    
            return file_paths;
            
    };

    map<string,string> file_paths = get_file_paths();

    if(dry_mode)
        print_paths(file_paths);

    else{
        for(const auto &pair : file_paths)
            rename(pair.first,pair.second);
        
        }

    }



    








void print_help(){

    cout<<"Usage: groupmv [directory path] [ -b || -e || -m] [ -a || -d] [string to change] (-d)"<<endl;
    cout<< "-b:       modify string at the beginning of the filename"<<endl;
    cout<< "-e:       modify string at the end of the filename"<<endl;
    cout<< "-m        delete a string in the middle of a filename(Does NOT work with append flag. Do NOT substitute for -b or -e)"<<endl;
    cout<< "-a:       append a string to the filename"<<endl;
    cout<< "-d:       delete a string from the filename"<<endl<<endl;
    cout<< "-u        dry run(display changes but dont actually make them(optional)"<<endl;
    cout<< "-h        display this help page"<<endl;
    cout<<"Options in brackets [] are required, options in parentheses () are optional"<<endl;






}




int main(int argc, char* argv[]){

    vector<string> args;

    for(int i = 0; i < argc;i++){
        args.push_back(argv[i]);
    }
    

    if(check_args(args) != 0){
        return -1;
    }


    else{

    path directory_path = args.at(1);
    string beginning_or_end = args.at(2);
    string append_or_delete = args.at(3);
    string string_to_change = args.at(4);



   

    trailing_slash(directory_path);

    


    
    if(beginning_or_end == "-b" && append_or_delete == "-a")
        append_on_beginning(directory_path, string_to_change);
        


    

    else if(beginning_or_end=="-e" && append_or_delete == "-a")
        append_on_end(directory_path, string_to_change);


        
        


    else if(beginning_or_end=="-e" && append_or_delete == "-d")
        delete_on_end(directory_path, string_to_change);


        
        
    
     else if(beginning_or_end=="-b" && append_or_delete == "-d")
        delete_on_beginning(directory_path,string_to_change);

    else if(beginning_or_end=="-m" && append_or_delete == "-d")
        delete_middle(directory_path,string_to_change);

    else{
        cerr<<"Invalid argument"<<endl;
        return -1;
    }


        
            
    
    
    
    
    
    
    
    

    


    return 0;
}


}
