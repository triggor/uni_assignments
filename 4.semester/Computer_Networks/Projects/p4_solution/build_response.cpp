//Author: Igor Tryhub, 275235
#include "build_response.h"
#include "converse.h"

std::string build_response(const char* path, int* msgsize, bool not_implemented, bool forbidden, bool not_found, 
			   bool moved_permanently, bool binary, int extension){
  std::string temp = "";
  std::string msg;
  if(not_implemented){
    msg = "Error 501: Not Implemented";
    temp.append("HTTP/1.1 501 Not Implemented\n");
    temp.append("Content-Type: text/html; charset=utf-8\n");
    temp.append("Content-Length: " + std::to_string(msg.size()) + "\n\r\n");
    std::cout << " + header of " << temp.size() << std::endl;
    std::cout << temp << std::endl;
    temp.append(msg);
    *msgsize = temp.size();
    return temp;
  }
  else if(forbidden){
    msg = "Error 403: Forbidden";
    temp.append("HTTP/1.1 403 Forbidden\n");
    temp.append("Content-Type: text/html; charset=utf-8\n");
    temp.append("Content-Length: " + std::to_string(msg.size()) + "\n\r\n");
    std::cout << " + header of " << temp.size() << std::endl;
    std::cout << temp << std::endl;
    temp.append(msg);
    *msgsize = temp.size();
    return temp;
  }
  else if(not_found){
    msg = "Error 404: Not Found";
    temp.append("HTTP/1.1 404 Not Found\n");
    temp.append("Content-Type: text/html; charset=utf-8\n");
    temp.append("Content-Length: " + std::to_string(msg.size()) + "\n\r\n");
    std::cout << " + header of " << temp.size() << std::endl;
    std::cout << temp << std::endl;
    temp.append(msg);
    *msgsize = temp.size();
    return temp;
  }
  else if(moved_permanently){
    char memblock[MSGSIZE];
    copy_file(path, memblock, binary);
    temp.append("HTTP/1.1 301 Moved Permanently\n");
    temp.append("Content-Type: text/html; charset=utf-8\n");
    temp.append("Content-Length: " + std::to_string(strlen(memblock)) + "\n\r\n");
    std::cout << " + header of " << temp.size() << std::endl;
    std::cout << temp << std::endl;
    temp.append(memblock);
    *msgsize = temp.size();
    return temp;
  }
  else{
    
    char memblock[MSGSIZE];
    std::string content_type;
    switch(extension){
      case 0:
	content_type = "application/octet-stream";
	break;
      case 1:
	content_type = "text/plain";
	break;
      case 2:
	content_type = "text/html";
	break;
      case 3:
	content_type = "text/css";
	break;
      case 4:
	content_type = "image/jpeg";
	break;
      case 5:
	content_type = "image/png";
	break;
      case 6:
	content_type = "application/pdf";
	break;
      default:
	std::cout << "Error: switch-case" << std::endl;
	exit(1); 
    }
    
    struct stat st;
    if(stat(path, &st) != 0) {
        *msgsize = 0;
    }
    *msgsize = st.st_size;
    copy_file(path, memblock, binary);
    temp.append("HTTP/1.1 200 OK\n");
    temp.append("Content-Type: " + content_type + "\n");
    temp.append("Content-Length: " + std::to_string(*msgsize) + "\n\r\n");
    std::cout << " + header of " << temp.size() << std::endl;
    *msgsize += temp.size();
    std::cout << " = " << *msgsize << std::endl << temp << std::endl;
    temp.append(memblock);
  }
  return temp;
}

void copy_file(const char* path, char* memblock, bool binary){
  std::ifstream file;
  if(binary) file.open(path, std::ios::in|std::ios::binary|std::ios::ate);//binary file
  else file.open(path, std::ios::in|std::ios::ate);//txt file
  if (file.is_open())
  {
    std::streampos size = file.tellg();
    std::cout << "Trying to open " << path << " of size " << size;
    file.seekg (0, std::ios::beg);
    file.read (memblock, size);
    file.close();
  }
  else std::cout << "Error: unable to open file" << std::endl; 
}