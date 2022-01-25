#!/bin/bash

./webserv /etc/missing_args
./webserv /etc/http_missing

./webserv /etc/autoindex_semicolon
./webserv /etc/autoindex_wrong
./webserv /etc/autoindex_missing

./webserv /etc/index_semicolon
./webserv /etc/index_wrong
./webserv /etc/index_missing

./webserv /etc/root_semicolon
# ./webserv /etc/root_wrong
./webserv /etc/root_missing

./webserv /etc/body_size_missing
./webserv /etc/body_size_neg
./webserv /etc/body_size_digit

./webserv /etc/listen_digit
./webserv /etc/listen_port_range
./webserv /etc/listen_semicolon
./webserv /etc/listen_missing

./webserv /etc/server_bracket

./webserv /etc/server_name_missing
./webserv /etc/server_name_semicolon

./webserv /etc/location_bracket
./webserv /etc/location_slash

./webserv /etc/error_page_semicolon
./webserv /etc/error_page_code
./webserv /etc/error_page_uri

./webserv /etc/except_method
./webserv /etc/except_semicolon

./webserv /etc/return_semicolon
./webserv /etc/return_code
./webserv /etc/return_URI