//Author: Igor Tryhub, 275235
#include "parse.h"
#include "converse.h"


int parse(char* server_ip, const char* buf, char* path, char* host, bool* connection, bool* binary, int* extension){
  
  *binary=true;
  std::string request(buf);
  std::string temp;
  bool moved_permanently = false;
  
  std::size_t getpos=request.find("GET /");
  if (getpos!=std::string::npos){
    std::size_t httppos=request.find("HTTP/1.");
    if (httppos!=std::string::npos){
      temp = request.substr(getpos+5, httppos-getpos-6);
      
      int differ;
      if((differ = strcmp(server_ip, "127.10.10.1")) == 0) temp = "strony_www/dom1.abc.pl/" + temp;
      else if((differ = strcmp(server_ip, "127.10.10.2")) == 0) temp = "strony_www/dom2.abc.pl/" + temp;
      else if((differ = strcmp(server_ip, "127.10.10.3")) == 0) temp = "strony_www/dom3.abc.pl/" + temp;
      else if((differ = strcmp(server_ip, "127.10.10.4")) == 0) temp = "strony_www/dom4.abc.pl/" + temp;
      
      strcpy(path, strdup(temp.c_str()));
      //std::cout << "PATH_LEN=" << strlen(path) << std::endl;
      char indexpath[PATHSIZE];
      strcpy(indexpath, path);//, strlen(path)-1);
      if(path[strlen(path)-1]=='/') std::strcat(indexpath, "index.html");
      else std::strcat(indexpath, "/index.html");
      path[strlen(path)] = '\0';
      indexpath[strlen(indexpath)] = '\0';
      
      std::ifstream file;
      //std::cout << "Trying to open: " << indexpath << std::endl;
      file.open(indexpath, std::ios::in);
      if (!(file.good())){
	//std::cout << "Trying to open: " << path << std::endl;
	file.open(path, std::ios::in);
	if(!(file.good())) return 3;//neither file, nor catalog
      }
      else{//success at opening '/catalog/index.html'
	file.close();
	strcpy(path, indexpath);
	path[strlen(path)] = '\0';
	moved_permanently = true;
      }
      
      std::size_t cdpos=temp.find("..");
      if (cdpos!=std::string::npos){
	return 2;
      }
    }
    else return 1;
  }
  else return 1;
  
  std::size_t hostpos=request.find("Host: ");
  if (hostpos!=std::string::npos){
    std::size_t useragentpos=request.find("User-Agent: ");
    if (useragentpos!=std::string::npos){
      temp = request.substr(hostpos+6, useragentpos-hostpos-7);
      strcpy(host, strdup(temp.c_str()));
    }
    else return 1;
  }
  else return 1;
  
  std::size_t connectionpos=request.find("Connection: ");
  if (connectionpos!=std::string::npos){
    temp = request.substr(connectionpos+12, 5);
    char tmp[6];
    strcpy (tmp, strdup(temp.c_str()));
    *connection = (bool)strcmp(tmp, "close");
  }
  else return 1;
  
  while(1){
    std::string temp2(path);
    std::size_t ext;
    ext=temp2.rfind(".txt");
    if (ext!=std::string::npos){
      *binary = false;
      *extension = 1;
      break;
    }
    ext=temp2.rfind(".htm");
    if (ext!=std::string::npos){
      *binary = false;
      *extension = 2;
      break;
    }
    ext=temp2.rfind(".css");
    if (ext!=std::string::npos){
      *binary = true;/////////////////////////////////////////////////////////?
      *extension = 3;
      break;
    }
    ext=temp2.rfind(".jpg");
    if (ext!=std::string::npos){
      *extension = 4;
      break;
    }
    ext=temp2.rfind(".jpeg");
    if (ext!=std::string::npos){
      *extension = 4;
      break;
    }
    ext=temp2.rfind(".png");
    if (ext!=std::string::npos){
      *extension = 5;
      break;
    }
    ext=temp2.rfind(".pdf");
    if (ext!=std::string::npos){
      *extension = 6;
      break;
    }
    *extension = 0;
    break;
  }
  
  if(moved_permanently) return 4;
  return 0;
}