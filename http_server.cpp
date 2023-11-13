#include "http_server.hh"
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sstream>
#include<bits/stdc++.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>


using namespace std;
vector<string> split(const string &s, char delim) {
    vector<string> elems;

    stringstream ss(s);
    string item;

    while (getline(ss, item, delim)) {
        if (!item.empty())
            elems.push_back(item);
    }

    return elems;
}

HTTP_Request::HTTP_Request(string request) {
    vector<string> lines = split(request, '\n');
    vector<string> first_line = split(lines[0], ' ');

    this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request
    this->method=first_line[0];
    this->url=first_line[1];
    /*
     TODO : extract the request method and URL from first_line here
    */

    if (this->method != "GET") {
        cerr << "Method '" << this->method << "' not supported" << endl;
        exit(1);
    }
}

HTTP_Response *handle_request(string req) {
    HTTP_Request *request = new HTTP_Request(req);
    // cout<<"This is very nice"<<req<<endl;
    HTTP_Response *response = new HTTP_Response();
    //cout<<"requested url :"<<request->url<<endl;
    string url=string("html_files")+request->url;
    //string url = string("html_files") + request->url;
    // string url = request->url+string("html_files");
    response->HTTP_version = "HTTP/"+request->HTTP_version;
    //cout<<"http version:"<<request->HTTP_version<<endl;
    struct stat sb;

    if (stat(url.c_str(), &sb) == 0) // requested path exists
    {
        //cout<<"----------path exist---------"<<endl;
        response->status_code = "200";
        response->status_text = "OK";
        response->content_type = "text/html";


        string body="";

        if (S_ISDIR(sb.st_mode)) {
            /*
            In this case, requested path is a directory.
            TODO : find the index.html file in that directory (modify the url
            accordingly)
            */
            url=url+string("index.html");

        }
        //cout<<"now actula url="<<url<<endl;
        /*
        TODO : open the file and read its contents
        */
        string st;
        ifstream in(url);
        //in>>st;
        getline(in,st);

        while(in)
        {
            body=body+st;
            body=body+string("\n");
            getline(in,st);
        }

        response->body=body;
        //cout<<body;

    }

    else {
        string body="";
        response->status_code = "404";
        response->status_text = "Not Found";
        response->content_type = "text/html";
        cout<<"----------path does not exist---------"<<endl;

        body=R"(<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>404 Not Found</title>
</head>

<body>
    <h1>Not Found</h1>
   <p>The requested URL  was not found on this server.</p>
   <p>4O4 ERROR</p>
</body>

</html>)";

        response->body=body;
        /*
        TODO : set the remaining fields of response appropriately
        */
    }

    delete request;

    return response;
}

string HTTP_Response::get_string() {
    /*

    TODO : implement this function
    */
    string line="";
    line=this->HTTP_version;
    line=line+string(" ");
    line=line+string(this->status_code);
    line=line+string(" ");
    line=line+string(this->status_text);
    string b=this->body;
    string ans="";
    ans=ans+string(line);

    ans=ans+string("\n");
    ans=ans+string("Content-Length: ");
    string str1=to_string(b.length());
    ans=ans+string(str1);
    ans=ans+string("\n");
    ans=ans+string("Content-type: ");
    ans=ans+string(this->content_type);
    ans=ans+string("\n");
    ans=ans+string("Closed");
    ans=ans+string("\n");
    ans=ans+string(b);

    return ans;
}